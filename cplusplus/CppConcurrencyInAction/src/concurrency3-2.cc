#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <list>
#include <mutex>
//////////////1//////////////
std::list<int> some_list;
std::mutex some_mutex;

void add_to_list(int new_value) {
  std::lock_guard<std::mutex> guard(some_mutex);
  some_list.push_back(new_value);
}

bool list_contains(int value_to_find) {
  std::lock_guard<std::mutex> guard(some_mutex);
  return std::find(some_list.begin(),
                   some_list.end(),
                   value_to_find) != some_list.end();
}


//////////////2//////////////
// 以下代码可能是恶意的访问，并没有达到预期效果
class some_data {
  int a;
  std::string b;
 public:
  void do_something() {}
};

class data_wrapper {
  some_data data;
  std::mutex m;
 public:
  template<typename Function>
  void process_data(Function func) {
    std::lock_guard<std::mutex> guard(some_mutex);
    func(data);
  }
};

some_data* unprotected;

void malicious_function(some_data& protected_data) {
  unprotected = &protected_data;
}

data_wrapper x;

void foo() {
  x.process_data(malicious_function);
  unprotected->do_something(); // 在无保护的情况下保护数据
}

//////////////3//////////////
#include <deque>
template<typename T,typename Container=std::deque<T> >
class stack {
 public:
  explicit stack(const Container&);
  explicit stack(Container&& = Container());
  template <class Alloc> explicit stack(const Alloc&);
  template <class Alloc> stack(const Container&, const Alloc&);
  template <class Alloc> stack(Container&&, const Alloc&);
  template <class Alloc> stack(stack&&, const Alloc&);

  bool empty() const;
  size_t size() const;
  T& top();
  T const& top() const;
  void push(T const&);
  void push(T&&);
  void pop();
  void swap(stack&&);
};

#include <exception>
#include <memory>

struct empty_stack: std::exception {
  const char* what() const throw() {
    return "empty stack!";
  }
};

template<typename T>
class threadsafe_stack {
 private:
  std::stack<T> data;
  mutable std::mutex m;
 public:
  threadsafe_stack()
      : data(std::stack<T>()) {}
  threadsafe_stack(const threadsafe_stack& other) {
    std::lock_guard<std::mutex> lock(other.m);
    data = other.data;
  }

  threadsafe_stack& operator=(const threadsafe_stack&) = delete;

  void push(T new_value) {
    std::lock_guard<std::mutex> lock(m);
    daa.push_back(new_value);
  }

  std::shared_ptr<T> pop() {
    std::lock_guard<std::mutex> lock(m);
    if (data.empty()) {
      throw empty_stack();
    }

    std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
    data.pop();
    return res;
  }

  void pop(T& value) {
    std::lock_guard<std::mutex> lock(m);
    if (data.empty()) {
      throw empty_stack();
    }

    value = data.top();
    data.pop();
  }

  bool empty() const {
    std::lock_guard<std::mutex> lock(m);
    return data.empty();
  }
};

//////////////4//////////////
class some_big_object {}
void swap(some_big_object& lhs, some_big_object& rhs) {}

class X {
 private:
  some_big_object some_detail;
  std::mutex m;
 public:
  X(some_big_object const& sd) : some_detail(sd) {}

  friend void swap(X& lhs, X& rhs) {
    if (&lhs == &rhs) {
      return;
    }

    std::lock(lhs.m, rhs.m);
    // std::adopt_lock 表示std::lock_guard
    // 可以获取锁之外，还讲锁交由std::lock_guard管理，不需要构建新的锁对象
    std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);
    std::lock_guard<std::mutex> lock_a(rhs.m, std::adopt_lock);
    swap(lhs.some_detail, rhs.some_detail);
  }
};

//////////////5//////////////
#include <mutex>
#include <stdexcept>

class hierarchical_mutex {
  std::mutex internal_mutex;
  unsigned long const hierarchy_value;
  unsigned long previous_hierarchy_value;
  static thread_local unsigned long this_thread_hierarchy_value;

  void check_for_hierarchy_violation()
  {
    if(this_thread_hierarchy_value <= hierarchy_value)
    {
      throw std::logic_error("mutex hierarchy violated");
    }
  }
  void update_hierarchy_value()
  {
    previous_hierarchy_value=this_thread_hierarchy_value;
    this_thread_hierarchy_value=hierarchy_value;
  }
 public:
  explicit hierarchical_mutex(unsigned long value):
      hierarchy_value(value),
      previous_hierarchy_value(0)
  {}
  void lock()
  {
    check_for_hierarchy_violation();
    internal_mutex.lock();
    update_hierarchy_value();
  }
  void unlock()
  {
    this_thread_hierarchy_value=previous_hierarchy_value;
    internal_mutex.unlock();
  }
  bool try_lock()
  {
    check_for_hierarchy_violation();
    if(!internal_mutex.try_lock())
      return false;
    update_hierarchy_value();
    return true;
  }
};
thread_local unsigned long
hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);       

hierarchical_mutex high_level_mutex(10000);
hierarchical_mutex low_level_mutex(5000);

int do_low_level_stuff() {
  return 42;
}


int low_level_func() {
  std::lock_guard<hierarchical_mutex> lk(low_level_mutex);
  return do_low_level_stuff();
}

void high_level_stuff(int some_param) {}


void high_level_func() {
  std::lock_guard<hierarchical_mutex> lk(high_level_mutex);
  high_level_stuff(low_level_func());
}

void thread_a() {
  high_level_func();
}

hierarchical_mutex other_mutex(100);
void do_other_stuff() {}


void other_stuff() {
  high_level_func();
  do_other_stuff();
}

void thread_b() {
  std::lock_guard<hierarchical_mutex> lk(other_mutex);
  other_stuff();
}

//////////////6//////////////
class Y {
 private:
  some_big_object some_detail;
  std::mutex m;
 public:
  Y(some_big_object const& sd) : some_detail(sd) {}

  friend void swap(X& lhs, X& rhs) {
    if (&lhs == &rhs) {
      return;
    }

    //std::unique_lock比它的小兄弟std::lock_guard更强大 。 
    //它在lock_guard的基础上还能： 
    //—— 没有关联互斥体时创建 
    //—— 没有锁定的互斥体时创建 
    //—— 显式和重复设置或释放关联互斥锁 
    //—— 移动互斥体 move 
    //—— 尝试锁定互斥体 
    //—— 延迟锁定关联互斥体
    std::unique_lock<std::mutex> lock_a(lhs.m, std::defer_lock);
    std::unique_lock<std::mutex> lock_b(rhs.m, std::defer_lock);
    // std::lock试图在原子步骤中获得guard1和guard2。
    // 但是，它要么失败，或者得到了全部
    std::lock(lock_a, lock_b);
    swap(lhs.some_detail, rhs.some_detail);
  }
};

//////////////7//////////////
std::un<std::mutex> get_lock() {
  extern std::mutex some_mutex;
  std::unique_lock<std::mutex> lk(some_mutex);
  prepare_data();
  return lk;
}

void process_data() {
  std::unique_lock<std::mutex> lk(get_lock());
  do_something();
}


//////////////8//////////////
void get_and_process_data() {
  std::unique_ptr<std::mutex> my_lock(the_mutex);
  some_class data_to_process = get_next_data_chunk();
  my_lock.unlock();

  result_type result = process(data_to_process);
  my_lock.lock();
  write_result(data_to_process, result);
}

class Z {
 private:
  int some_detail;
  mutable std::mutex m;

  int get_detail() const {
    std::lock_guard<std::mutex> lock_a(m);
    return some_detail;
  }
 public:
  Z(int sd):some_detail(sd){}

  friend bool operator==(Z const& lhs, Z const& rhs) {
    if(&lhs==&rhs)
      return true;
    int const lhs_value=lhs.get_detail();
    int const rhs_value=rhs.get_detail();
    return lhs_value==rhs_value;
  }
};
int main() {
  std::cout <<"End\n";
}
