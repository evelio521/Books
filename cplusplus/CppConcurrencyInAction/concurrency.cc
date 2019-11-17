#include "concurrency.h"

#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <list>
#include <mutex>
#include <future>
#include <queue>
#include <stack>

#include <boost/thread/shared_mutex.hpp>

/////////////////////////1-1////////////////////
#if 0
void hello() {
  std::cout << "Hello Concurrent World\n";
}

void start() {
  std::thread t(hello);
  t.join();
}

int main(int argc, char** argv) {
  start();
  return 0;
}
#endif
/////////////////////////1-1////////////////////


/////////////////////////2-1////////////////////////
#if 0
class background_task {
 public:
  void operator()() const {
    std::cout <<"background_task \n";
  }
};

class thread_guard {
  std::thread& t;
 public:
  explicit thread_guard(std::thread& t_) : t(t_) {}

  ~thread_guard() {
    if (t.joinable()) {
      t.join();
    }
  }

  thread_guard(thread_guard const&)=delete;
  thread_guard& operator=(thread_guard const&)=delete;
};

struct func {
  int i;
  func(int i_) : i(i_) {}
  void operator()() {
    std::cout << i <<" func\n";
    i++;
  }
};

void f() {
  int some_local_state = 0;
  func my_fun(some_local_state);
  std::thread t(my_fun);
  thread_guard g(t);
}

int main(int argc, char** argv) {
  background_task bf;
  std::thread my_thread(bf);
  my_thread.join();
  std::thread my_thread2((background_task()));
  my_thread2.join();
  std::thread my_thread3{background_task()};
  my_thread3.detach();
  //成员函数joinable用于检测thread是否需要join。
  //joinable为false的情况有3种:
  //1.thread调用过detach;
  //2.thread已经join过;
  //3.thread为空。
  assert(!my_thread3.joinable());

  f();
  std::cout <<"End\n";
  return 0;
}
#endif
/////////////////////////2-1////////////////////////


/////////////////////////2-2////////////////////////
#if 0
void f(int i, std::string const& s) {
  std::cout << s << std::endl;
}

void oops(int some_param) {
  char buffer[1024];
  sprintf(buffer, "%i", some_param);
  std::thread t(f, 3, std::string(buffer));
  // std::string tt;
  // std::thread t(f, 3, std::ref(tt));
  t.detach();
}

class X {
 public:
  void do_lengthy_work(int) {}
};

int main() {
  oops(5);

  X my_x;
  int num(0);
  std::thread t(&X::do_lengthy_work, &my_x, num);
  t.join();
}
#endif
/////////////////////////2-2////////////////////////


/////////////////////////2-3////////////////////////
#if 0
#include <vector>
void some_function() {}

void some_other_function () {}


void f(std::thread t) {
  t.join();
}

void g() {
  f(std::thread(some_function));
  std::thread t (some_function);
  f(std::move(t));
}

class scoped_thread {
  std::thread t;
 public:
  explicit scoped_thread(std::thread t_)
      : t (std::move(t_)) {
        if (!t.joinable()) {
          throw std::logic_error("NO thread");
        }
      }
  ~scoped_thread() {
    t.join();
  }

  scoped_thread(scoped_thread const&)=delete;
  scoped_thread& operator=(scoped_thread const&)=delete;
};

void f2() {
  scoped_thread t((std::thread(some_function)));
}


void do_work(unsigned id) {
  std::cout << id << std::endl;
}

void f3() {
  std::vector<std::thread> threads;
  for (unsigned i = 0; i < 20; i++) {
    threads.push_back(std::thread(do_work, i));
  }

  std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}
int main() {

  std::thread t1(some_function);
  std::thread t2 = std::move(t1);
  t1 = std::thread(some_other_function);

  std::thread t3;
  t3 = std::move(t2);
  // t1 = std::move(t3);
  //
  t1.detach();
  // t2.detach();
  t3.detach();

  g();
  f2();
  f3();
  std::cout <<"End\n";
}
#endif
/////////////////////////2-3////////////////////////


/////////////////////////2-4////////////////////////
#if 0
#include <vector>
#include <algorithm>
#include <numeric>

template <typename Iterator, typename T>
struct accumulate_block {
  void operator()(Iterator first, Iterator last, T& result) {
    result = std::accumulate(first, last, result);
  }
};

template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init) {
  unsigned long const length = std::distance(first, last);

  if (!length) return init;

  unsigned long const min_per_thread = 25;
  unsigned long const max_threads = 
      (length + min_per_thread - 1) / min_per_thread;
  unsigned long const hardware_threads = 
      std::thread::hardware_concurrency();

  unsigned long const num_threads = 
      std::min(hardware_threads != 0 ? 2 : hardware_threads, max_threads);

  unsigned long const block_size =
      length / num_threads;

  std::vector<T> result(num_threads);
  std::vector<std::thread> threads(num_threads - 1);

  Iterator block_start = first;
  for (unsigned long i = 0; i < num_threads - 1; ++i) {
    Iterator block_end = block_start;
    std::advance(block_end, block_size);
    threads[i] = std::thread(
        accumulate_block<Iterator, T>(),
        block_start, block_end, std::ref(result[i]));

    block_start = block_end;
  }

  accumulate_block<Iterator, T>() (block_start,
                                   last,
                                   result[num_threads - 1]);

  std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

  return std::accumulate(result.begin(),
                         result.end(),
                         init);

}

int main() {
  std::vector<int> vec;
  for (int i = 0; i < 20000000; ++i) {
    vec.push_back(i);
  }
  std::cout << parallel_accumulate<std::vector<int>::iterator, int>(vec.begin(), vec.end(), 0) << std::endl;
  std::cout <<"End\n";
}
#endif
/////////////////////////2-4////////////////////////


/////////////////////////2-5////////////////////////
#if 0
void f() {
  std::cout << std::this_thread::get_id() << std::endl;
}
int main() {
  std::thread t(f);
  t.join();
  std::cout << std::this_thread::get_id() << std::endl;
  std::cout <<"End\n";
}
#endif
/////////////////////////2-5////////////////////////



/////////////////////////3-2////////////////////////
#if 0
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

#include <exception>
#include <memory>
#include <stack>

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
    data.push_back(new_value);
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

class some_big_object {};
void Swap(some_big_object& lhs, some_big_object& rhs) {}

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
    std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
    Swap(lhs.some_detail, rhs.some_detail);
  }
};

class Y {
 private:
  some_big_object some_detail;
  std::mutex m;
 public:
  Y(some_big_object const& sd) : some_detail(sd) {}

  friend void swap(Y& lhs, Y& rhs) {
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
    Swap(lhs.some_detail, rhs.some_detail);
  }
};


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
int main() {
  std::cout <<"End\n";
}
#endif
/////////////////////////3-2////////////////////////
/////////////////////////3-3////////////////////////
#if 0
class some_resource {
 public:
  void do_something() {}
};
std::shared_ptr<some_resource> resource_ptr;
std::once_flag resource_flag;

void init_resource() {
  resource_ptr.reset(new some_resource);
}

void foo() {
  std::call_once(resource_flag, init_resource);
  resource_ptr->do_something();
}

struct connection_info {};

struct data_packet {};

struct connection_handle {
  void send_data(data_packet const&) {}
  data_packet receive_data() {
    return data_packet();
  }
};

struct remote_connection_manager {
  connection_handle open(connection_info const&) {
    return connection_handle();
  }
} connection_manager;


class X {
 private:
  connection_info connection_details;
  connection_handle connection;
  std::once_flag connection_init_flag;

  void open_connection() {
    connection=connection_manager.open(connection_details);
  }
 public:
  X(connection_info const& connection_details_):
      connection_details(connection_details_) {}
  void send_data(data_packet const& data) {
    std::call_once(connection_init_flag,&X::open_connection,this);
    connection.send_data(data);
  }
  data_packet receive_data() {
    std::call_once(connection_init_flag,&X::open_connection,this);
    return connection.receive_data();
  }
};

#include <map>
#include <string>
#include <mutex>
#include <boost/thread/shared_mutex.hpp>

class dns_entry {};

class dns_cache {
  std::map<std::string,dns_entry> entries;
  boost::shared_mutex entry_mutex;
 public:
  dns_entry find_entry(std::string const& domain) {
    boost::shared_lock<boost::shared_mutex> lk(entry_mutex);
    std::map<std::string,dns_entry>::const_iterator const it=
        entries.find(domain);
    return (it==entries.end())?dns_entry():it->second;
  }
  void update_or_add_entry(std::string const& domain,
                           dns_entry const& dns_details) {
    std::lock_guard<boost::shared_mutex> lk(entry_mutex);
    entries[domain]=dns_details;
  }
};

#include <iostream>
#include <memory>
#include <mutex>

class Singleton {
public:
  static Singleton& GetInstance() {
    static std::once_flag s_flag;
    std::call_once(s_flag, [&]() {
      instance_.reset(new Singleton);
    });

    return *instance_;
  }

  ~Singleton() = default;

  void PrintAddress() const {
    std::cout << this << std::endl;
  }

private:
  Singleton() = default;

  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;

private:
  static std::unique_ptr<Singleton> instance_;
};

std::unique_ptr<Singleton> Singleton::instance_;

int main() {
  Singleton& s1 = Singleton::GetInstance();
  s1.PrintAddress();

  Singleton& s2 = Singleton::GetInstance();
  s2.PrintAddress();

  return 0;
}

#include <iostream>

class Singleton {
public:
  static Singleton& GetInstance() {
    static Singleton intance;
    return intance;
  }

  ~Singleton() = default;

private:
  Singleton() = default;

  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;
};

int main() {
  Singleton& s1 = Singleton::GetInstance();
  std::cout << &s1 << std::endl;

  Singleton& s2 = Singleton::GetInstance();
  std::cout << &s2 << std::endl;

  return 0;
}
#endif
/////////////////////////3-3////////////////////////

/////////////////////////4-1////////////////////////
#if 0
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <list>
#include <mutex>
#include <queue>
#include <condition_variable>

class data_chunk {};
data_chunk prepare_data() {
  return data_chunk();
}
void process(data_chunk& dc) {};

bool is_last_chunk(data_chunk& dc) {
  return true;
}

std::mutex mut;
std::queue<data_chunk> data_queue;
std::condition_variable daya_cond;

void data_preparation_thread() {
  data_chunk const data = prepare_data();
  std::lock_guard<std::mutex> lk(mut);
  data_queue.push(data);
  daya_cond.notify_one();
}

void data_processing_thread() {
  while(true) {
    std::unique_lock<std::mutex> lk(mut);
    // wait 成功返回才会持有锁，
    // 这里比用std::lock_guard 要灵活
    daya_cond.wait(lk, []{return !data_queue.empty();});
    data_chunk data = data_queue.front();
    data_queue.pop();
    lk.unlock(); // prcoess可能是个耗时操作，不应该过长持有锁
    process(data);
    if (is_last_chunk(data)) {
      break;
    }
  }
}

template<typename T>
class threadsafe_queue {
 private:
  mutable std::mutex mut;
  std::queue<T> data_queue;
  std::condition_variable data_cond;
 public:
  threadsafe_queue() {}
  threadsafe_queue(threadsafe_queue const& other) {
    std::lock_guard<std::mutex> lk(other.mut);
    data_queue=other.data_queue;
  }

  void push(T new_value) {
    std::lock_guard<std::mutex> lk(mut);
    data_queue.push(new_value);
    data_cond.notify_one();
  }

  void wait_and_pop(T& value) {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk,[this]{return !data_queue.empty();});
    value=data_queue.front();
    data_queue.pop();
  }

  std::shared_ptr<T> wait_and_pop() {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk,[this]{return !data_queue.empty();});
    std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
    data_queue.pop();
    return res;
  }

  bool try_pop(T& value) {
    std::lock_guard<std::mutex> lk(mut);
    if(data_queue.empty())
      return false;
    value=data_queue.front();
    data_queue.pop();
  }

  std::shared_ptr<T> try_pop() {
    std::lock_guard<std::mutex> lk(mut);
    if(data_queue.empty())
      return std::shared_ptr<T>();
    std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
    data_queue.pop();
    return res;
  }

  bool empty() const {
    std::lock_guard<std::mutex> lk(mut);
    return data_queue.empty();
  }
};
int main() {
  std::cout <<"End\n";
}
#endif
/////////////////////////4-1////////////////////////

/////////////////////////4-2////////////////////////
#if 0
#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <list>
#include <mutex>
#include <condition_variable>
#include <future>
int find_the_answer_to_ltuaw() {
  return 0;
}

void do_other_stuff() {}

void test_future() {
  std::future<int> the_answer = 
      std::async(find_the_answer_to_ltuaw);
  do_other_stuff();
  std::cout<<the_answer.get() << std::endl;
}

struct X {
  void foo(int,std::string const&) {
    return;
  }
  std::string bar(std::string const&) {
    return "";
  }
};

struct Y {
  double operator()(double) {
    return 0.0f;
  }
};

class move_only {
 public:
  move_only() {}
  move_only(move_only&&) {}
  move_only(move_only const&) = delete;
  move_only& operator=(move_only&&);
  move_only& operator=(move_only const&) = delete;
  void operator()() {}
};

X baz(X&) {
  return X();
}

void test_shared_future() {
  std::future<int> fut = std::async(do_get_value);
  std::shared_future<int> shared_fut = fut.share();

  // 共享的 future 对象可以被多次访问.
  std::cout << "value: " << shared_fut.get() << '\n';
  std::cout << "its double: " 
      << shared_fut.get()*2 << '\n';
}

int main() {
  test_future();

  X x;
  auto f1=std::async(&X::foo,&x,42,"hello");
  auto f2=std::async(&X::bar,x,"goodbye");

  Y y;
  auto f3=std::async(Y(),3.141);
  auto f4=std::async(std::ref(y),2.718);
  auto f6=std::async(baz,std::ref(x));

  auto f5=std::async(move_only());

  // defered 表明在wait或get函数调用才执行
  // async   函数调用立即执行
  auto f7 = std::async(std::launch::deferred, baz, std::ref(x));
  auto f8 = std::async(std::launch::deferred | std::launch::async , baz, std::ref(x));

  f7.wait();
  std::cout <<"End\n";
  return 0;
}

std::mutex m;
std::deque<std::packaged_task<void()> > tasks;

bool gui_shutdown_message_received();
void get_and_process_gui_message();

void gui_thread() {
  while(!gui_shutdown_message_received()) {
    get_and_process_gui_message();
    std::packaged_task<void()> task;
    {
      std::lock_guard<std::mutex> lk(m);
      if(tasks.empty())
        continue;
      task=std::move(tasks.front());
      tasks.pop_front();
   }
   task();
  }
}

//packaged_task和promise在某种程度上有点像，promise保存了一个共享状态的值，而packaged_task保存的是一个函数
std::thread gui_bg_thread(gui_thread);

template<typename Func>
std::future<void> post_task_for_gui_thread(Func f) {
  std::packaged_task<void()> task(f);
  std::future<void> res=task.get_future();
  std::lock_guard<std::mutex> lk(m);
  tasks.push_back(std::move(task));
  return res;
}



void initiazer(std::promise<int>* promObj){
   std::cout<<"Inside Thread"<<std::endl;
   promObj->set_value(35);
}

void test_promise() {
   std::promise<int> promiseObj;
   std::future<int> futureObj = promiseObj.get_future();
   std::thread th(initiazer, &promiseObj);
   std::cout<<futureObj.get()<<std::endl;
   th.join();
}

#endif
/////////////////////////4-2////////////////////////

/////////////////////////4-3////////////////////////
#if 0
#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <list>
#include <mutex>
#include <condition_variable>
#include <future>
int some_task() { return 0;}
void do_something_with(int) {}

#include <condition_variable>
#include <mutex>
#include <chrono>
std::condition_variable cv;
bool done;
std::mutex m;
bool wait_loop() {
  auto const timeout =
      std::chrono::steady_clock::now() +
      std::chrono::milliseconds(500);
  std::unique_lock<std::mutex> lk(m);
  while(!done) {
    if(cv.wait_until(lk,timeout)
       == std::cv_status::timeout)
      break;
  }
  return done;
}

void test_duration () {
  typedef std::chrono::duration<int> seconds_type;
  typedef std::chrono::duration<int,std::milli> milliseconds_type;
  typedef std::chrono::duration<int,std::ratio<60*60>> hours_type;

  hours_type h_oneday (24);                  // 24h
  seconds_type s_oneday (60*60*24);          // 86400s
  milliseconds_type ms_oneday (s_oneday);    // 86400000ms

  seconds_type s_onehour (60*60);            // 3600s
  //hours_type h_onehour (s_onehour); // NOT VALID (type truncates), use:
  hours_type h_onehour (std::chrono::duration_cast<hours_type>(s_onehour));
  milliseconds_type ms_onehour (s_onehour);  // 3600000ms (ok, no type truncation)

  std::cout << ms_onehour.count() << "ms in 1h" << std::endl;

}

void test_time_point() {
  using namespace std::chrono;

  system_clock::time_point tp_epoch;    // epoch value

  time_point <system_clock,duration<int>> tp_seconds (duration<int>(1));

  system_clock::time_point tp (tp_seconds);

  std::cout << "1 second since system_clock epoch = ";
  std::cout << tp.time_since_epoch().count();
  std::cout << " system_clock periods." << std::endl;

  // display time_point:
  std::time_t tt = system_clock::to_time_t(tp);
  std::cout << "time_point tp is: " << ctime(&tt);
}

void test_duration_time_point() {
  using std::chrono::system_clock;

  std::chrono::duration<int,std::ratio<60*60*24> > one_day (1);

  system_clock::time_point today = system_clock::now();
  system_clock::time_point tomorrow = today + one_day;

  std::time_t tt;

  tt = system_clock::to_time_t ( today );
  std::cout << "today is: " << ctime(&tt);

  tt = system_clock::to_time_t ( tomorrow );
  std::cout << "tomorrow will be: " << ctime(&tt);
}
int main() {
  std::chrono::milliseconds ms(54802);
  std::chrono::seconds s =
      std::chrono::duration_cast<std::chrono::seconds>(ms);

  std::future<int> f = std::async(some_task);
  if (f.wait_for(std::chrono::milliseconds(35)) == 
      std::future_status::ready) {
    do_something_with(f.get());
  }
  std::cout <<"End\n";
}
#endif 
/////////////////////////4-3////////////////////////

/////////////////////////4-4////////////////////////
#if 0
#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <list>
#include <mutex>
#include <condition_variable>
#include <future>
template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input) {
  if(input.empty()) {
    return input;
  }
  std::list<T> result;
  result.splice(result.begin(),input,input.begin());
  T const& pivot=*result.begin();
  auto divide_point=std::partition(
      input.begin(),input.end(),
      [&](T const& t){return t<pivot;});
  std::list<T> lower_part;
  lower_part.splice(lower_part.end(),input,
                    input.begin(),
                    divide_point);
  auto new_lower(
      sequential_quick_sort(std::move(lower_part)));
  auto new_higher(
      sequential_quick_sort(std::move(input)));
  result.splice(result.end(),new_higher);
  result.splice(result.begin(),new_lower);
  return result;
}

template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input) {
  if(input.empty()) {
    return input;
  }
  std::list<T> result;
  result.splice(result.begin(),input,input.begin());
  T const& pivot=*result.begin();
  auto divide_point=std::partition(
      input.begin(),input.end(),
      [&](T const& t){return t<pivot;});
  std::list<T> lower_part;
  lower_part.splice(lower_part.end(),
                    input,input.begin(),
                    divide_point);
  std::future<std::list<T> > new_lower(
      std::async(&parallel_quick_sort<T>,
                 std::move(lower_part)));
  auto new_higher(
      parallel_quick_sort(std::move(input)));
  result.splice(result.end(),new_higher);
  result.splice(result.begin(),new_lower.get());
  return result;
}

template<typename F,typename A>
std::future<typename std::result_of<F(A&&)>::type>
spawn_task(F&& f,A&& a) {
  typedef typename std::result_of<F(A&&)>::type result_type;
  std::packaged_task<result_type(A&&)>
      task(std::move(f));
  std::future<result_type> res(task.get_future());
  std::thread t(std::move(task),std::move(a));
  t.detach();
  return res;
}

int main() {
  std::cout <<"End\n";
}
#endif
/////////////////////////4-4////////////////////////

/////////////////////////5-2////////////////////////
#if 0
#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <list>
#include <mutex>
#include <condition_variable>
#include <future>
#include <atomic>

class spinlock_mutex {
  std::atomic_flag flag;
 public:
  spinlock_mutex() : flag(ATOMIC_FLAG_INIT) {}

  void lock() {
    while(flag.test_and_set(std::memory_order_acquire));
  }

  void unlock() {
    flag.clear(std::memory_order_release);
  }
};


#include <iostream>  // std::cout
#include <atomic>    // std::atomic,
// std::atomic_flag,
// ATOMIC_FLAG_INIT
#include <thread>    // std::thread,
// std::this_thread::yield
#include <vector>    // std::vector

std::atomic<bool> ready(false);    // can be checked without being set
std::atomic_flag winner = ATOMIC_FLAG_INIT;    // always set when checked

// test_and_set() 函数检查 std::atomic_flag 标志，
// 如果 std::atomic_flag 之前没有被设置过，则设置
// std::atomic_flag 的标志，并返回先前该
// std::atomic_flag 对象是否被设置过，
// 如果之前 std::atomic_flag 对象已被设置，
// 则返回 true，否则返回 false。
void count1(int id) {
  while (!ready) {
    std::this_thread::yield();
  } // 等待主线程中设置 ready 为 true.

  for (int i = 0; i < 1000000; ++i) {
  } // 计数.

  // 如果某个线程率先执行完上面的计数过程，
  // 则输出自己的 ID.  此后其他线程执行
  // test_and_set 是 if 语句判断为 false，
  // 因此不会输出自身 ID.
  if (!winner.test_and_set()) {
    std::cout << "thread #" << id << " won!\n";
  }
}
int test_atomic_flag () {
  std::vector<std::thread> threads;
  std::cout << "spawning 10 threads that count to 1 million...\n";
  for (int i = 1; i <= 10; ++i)
    threads.push_back(std::thread(count1, i));
  ready = true;

  for (auto & th:threads)
    th.join();
  return 0;
}

std::atomic<bool> b;
bool x;

void valwout() {
  std::cout << "x:" << x
      << " b:" << b <<std::endl;
}
void test_compare_exchange_weak() {
  x = b.load(std::memory_order_acquire);
  valwout();
  b.store(true);
  //Replaces the contained value by val and returns the value it had immediately before.
  x = b.exchange(false, std::memory_order_acq_rel);

  valwout();

  bool expected = false;
  while (!b.compare_exchange_weak(expected, true));
  valwout();
}

#include <atomic>
#include <iostream>

std::atomic<int>  ai;

int  tst_val= 4;
int  new_val= 5;
bool exchanged= false;

void valsout() {
  std::cout << "ai= " << ai
      << "  tst_val= " << tst_val
      << "  new_val= " << new_val
      << "  exchanged= " << std::boolalpha << exchanged
      << "\n";
}

void test_compare_exchange_strong() {
  ai= 3;
  valsout();

  // tst_val != ai   ==>  tst_val is modified
  exchanged= ai.compare_exchange_strong( tst_val, new_val );
  valsout();

  // tst_val == ai   ==>  ai is modified
  exchanged= ai.compare_exchange_strong( tst_val, new_val );
  valsout();
}

class my_data {};
void process_data(std::shared_ptr<my_data> local) {}
std::shared_ptr<my_data> p;
void process_global_data() {
  std::shared_ptr<my_data> local = std::atomic_load(&p);
  process_data(local);
}

void update_global_data() {
  std::shared_ptr<my_data> local(new my_data);
  std::atomic_store(&p, local);
}

class Foo {};

int main() {
  test_atomic_flag();

  test_compare_exchange_weak();
  test_compare_exchange_strong();


  Foo some_array[5];

  std::atomic<Foo*> p(some_array);

  Foo* x = p.fetch_add(2); // 返回第零个值
  assert(x == some_array);
  assert(p.load() == &some_array[2]);

  x = (p-=1);
  assert(x == &some_array[1]);
  assert(p.load() == &some_array[1]);
  
  std::cout <<"End\n";
}
#endif
/////////////////////////5-2////////////////////////

/////////////////////////5-3////////////////////////
#if 0
#include <vector>
#include <atomic>
#include <iostream>
#include <chrono>
#include <thread>

// 这两种操作是低效的操作
// 应该是内存模型防止无效的等待
/*
std::vector<int> data;
std::atomic_bool data_ready(false);

void reader_thread() {
  while(!data_ready.load()) {
    std::this_thread::sleep_for(
        std::chrono::milliseconds(1));
  }
  std::cout<<"The answer="<<data[0]<<"\n";
}
void writer_thread() {
  data.push_back(42);
  data_ready=true;
}

void foo(int a,int b) {
  std::cout<<a<<","<<b<<std::endl;
}

int get_num() {
  static int i=0;
  return ++i;
}
*/

//排序一致序列(memory_order_seq_cst)，
//获取-释放序列(memory_order_consume, 
//              memory_order_acquire, 
//              memory_order_release,
//              memory_order_acq_rel)
//自由序列(memory_order_relaxed)
//
//
//memory_order_seq_cst
//序列一致是最简单、直观的序列，但是他也是
//最昂贵的内存序列，因为它需要对所有线程进
//行全局同步。在一个多处理系统上，这就需要
//处理期间进行大量并且费时的信息交换。
/*
#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x,y;
std::atomic<int> z;

void write_x() {
  x.store(true,std::memory_order_seq_cst);
}

void write_y() {
  y.store(true,std::memory_order_seq_cst);
}

void read_x_then_y() {
  while(!x.load(std::memory_order_seq_cst));
  if(y.load(std::memory_order_seq_cst)) ++z;
}

void read_y_then_x() {
  while(!y.load(std::memory_order_seq_cst));
  if(x.load(std::memory_order_seq_cst)) ++z;
}
int main() {
  x=false;
  y=false;
  z=0;
  std::thread a(write_x);
  std::thread b(write_y);
  std::thread c(read_x_then_y);
  std::thread d(read_y_then_x);
  a.join();
  b.join();
  c.join();
  d.join();
  assert(z.load()!=0); // 永远不会发生
}
*/

/*
// memory_order_relaxed
#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x,y;
std::atomic<int> z;

void write_x_then_y() {
  x.store(true,std::memory_order_relaxed);
  y.store(true,std::memory_order_relaxed);
}

void read_y_then_x() {
  while(!y.load(std::memory_order_relaxed));
  if(x.load(std::memory_order_relaxed)) ++z;
}

int main() {
  x=false;
  y=false;
  z=0;
  std::thread a(write_x_then_y);
  std::thread b(read_y_then_x);
  a.join();
  b.join();
  assert(z.load()!=0); // 可能发生
}
*/

/*
// memory_order_consume
// memory_order_acquire
// memory_order_release
// memory_order_acq_rel

#include <atomic>
#include <thread>
#include <assert.h>
std::atomic<bool> x,y;
std::atomic<int> z;

void write_x_then_y() {
  x.store(true,std::memory_order_relaxed);
  y.store(true,std::memory_order_release);
}

void read_y_then_x() {
  while(!y.load(std::memory_order_acquire));
  if(x.load(std::memory_order_relaxed)) ++z;
}

int main() {
  x=false;
  y=false;
  z=0;
  std::thread a(write_x_then_y);
  std::thread b(read_y_then_x);
  a.join();
  b.join();
  assert(z.load()!=0); // 不会触发
}
*/

/*
//使用非原子操作执行序列
#include <atomic>
#include <thread>
#include <assert.h>

bool x = false;  // x现在是一个非原子变量
std::atomic<bool> y;
std::atomic<int> z;

void write_x_then_y() {
  x = true;  // 1 在栅栏前存储x
  std::atomic_thread_fence(std::memory_order_release);
  y.store(true,std::memory_order_relaxed);  // 2 在栅栏后存储y
}

void read_y_then_x() {
  while(!y.load(std::memory_order_relaxed));  // 3 在#2写入前，持续等待
  std::atomic_thread_fence(std::memory_order_acquire);
  if(x)  // 4 这里读取到的值，是#1中写入
    ++z;
}
int main() {
  x=false;
  y=false;
  z=0;
  std::thread a(write_x_then_y);
  std::thread b(read_y_then_x);
  a.join();
  b.join();
 assert(z.load()!=0);  // 5 断言将不会触发
}
*/

#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x,y;
std::atomic<int> z;

void write_x_then_y() {
  x.store(true,std::memory_order_relaxed);
  std::atomic_thread_fence(std::memory_order_release);
  y.store(true,std::memory_order_relaxed);
}

void read_y_then_x() {
  while(!y.load(std::memory_order_relaxed));
  std::atomic_thread_fence(std::memory_order_acquire);
  if(x.load(std::memory_order_relaxed))
    ++z;
}

int main() {
  x=false;
  y=false;
  z=0;
  std::thread a(write_x_then_y);
  std::thread b(read_y_then_x);
  a.join();
  b.join();
  assert(z.load()!=0);  // 7
}

#endif
/////////////////////////5-3////////////////////////

/////////////////////////6-2////////////////////////
#if 0
#include <exception>
#include <stack>
#include <queue>

struct empty_stack: std::exception {
  const char* what() const throw() {
    return "empty stack!";
  }
};

template<typename T>
class threadsafe_stack
{
 private:
  std::stack<T> data;
  mutable std::mutex m;
 public:
  threadsafe_stack(){}
  threadsafe_stack(const threadsafe_stack& other)
  {
    std::lock_guard<std::mutex> lock(other.m);
    data=other.data;
  }

  threadsafe_stack& operator=(const threadsafe_stack&) = delete;

  void push(T new_value)
  {
    std::lock_guard<std::mutex> lock(m);
    data.push(std::move(new_value));  // 1
  }
  std::shared_ptr<T> pop()
  {
    std::lock_guard<std::mutex> lock(m);
    if(data.empty()) {
      std::cout << "Empty Stack" << std::endl;
      return std::shared_ptr<T>();
    };  // 2
    std::shared_ptr<T> const res(
        std::make_shared<T>(std::move(data.top())));  // 3
    data.pop();  // 4
    return res;
  }
  bool pop(T& value)
  {
    std::lock_guard<std::mutex> lock(m);
    if(data.empty()) {
      return false;
    };
    value = std::move(data.top());  // 5
    data.pop();  // 6
    return true;
  }
  bool empty() const
  {
    std::lock_guard<std::mutex> lock(m);
    return data.empty();
  }
};

//lock_guard 通常用来管理一个 std::mutex 类型的对象。
// unique_lock  如果上锁不成功，并不会阻塞当前线程。
template<typename T>
class threadsafe_queue_cond {
 private:
  mutable std::mutex mut;
  std::queue<std::shared_ptr<T> > data_queue;
  std::condition_variable data_cond;
 public:
  threadsafe_queue_cond() {}

  void wait_and_pop(T& value) {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk,[this]{return !data_queue.empty();});
    value=std::move(*data_queue.front());  // 1
    data_queue.pop();
  }

  bool try_pop(T& value) {
    std::lock_guard<std::mutex> lk(mut);
    if(data_queue.empty())
      return false;
    value=std::move(*data_queue.front());  // 2
    data_queue.pop();
    return true;
  }

  std::shared_ptr<T> wait_and_pop() {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk,[this]{return !data_queue.empty();});
    std::shared_ptr<T> res=data_queue.front();  // 3
    data_queue.pop();
    return res;
  }

  std::shared_ptr<T> try_pop() {
    std::lock_guard<std::mutex> lk(mut);
    if(data_queue.empty()) {
      std::cout << "Empty Queue" <<std::endl;
      return std::shared_ptr<T>();
    }
    std::shared_ptr<T> res=data_queue.front();  // 4
    data_queue.pop();
    return res;
  }

  void push(T new_value) {
    std::shared_ptr<T> data(
        std::make_shared<T>(std::move(new_value)));  // 5
    std::lock_guard<std::mutex> lk(mut);
    data_queue.push(data);
    data_cond.notify_one();
  }

  bool empty() const {
    std::lock_guard<std::mutex> lk(mut);
    return data_queue.empty();
  }
};

template<typename T>
class threadsafe_queue_cond_link
{
 private:
  struct node
  {
    std::shared_ptr<T> data;
    std::unique_ptr<node> next;
  };

  std::mutex head_mutex;
  std::unique_ptr<node> head;
  std::mutex tail_mutex;
  node* tail;
  std::condition_variable data_cond;
 public:
  threadsafe_queue_cond_link():
      head(new node),tail(head.get())
  {}
  threadsafe_queue_cond_link(const threadsafe_queue_cond_link& other)=delete;
  threadsafe_queue_cond_link& operator=(const threadsafe_queue_cond_link& other)=delete;

  //std::shared_ptr<T> try_pop();
  //bool try_pop(T& value);
  //std::shared_ptr<T> wait_and_pop();
  //void wait_and_pop(T& value);
  void push(T new_value) {
    std::shared_ptr<T> new_data(
        std::make_shared<T>(std::move(new_value)));
    std::unique_ptr<node> p(new node);
    {
      std::lock_guard<std::mutex> tail_lock(tail_mutex);
      tail->data=new_data;
      node* const new_tail=p.get();
      tail->next=std::move(p);
      tail=new_tail;
    }
    data_cond.notify_one();
  }
  //bool empty();
 private:
  node* get_tail()
  {
    std::lock_guard<std::mutex> tail_lock(tail_mutex);
    return tail;
  }

  std::unique_ptr<node> pop_head()  // 1
  {
    std::unique_ptr<node> old_head=std::move(head);
    head=std::move(old_head->next);
    return old_head;
  }

  std::unique_lock<std::mutex> wait_for_data()  // 2
  {
    std::unique_lock<std::mutex> head_lock(head_mutex);
    data_cond.wait(head_lock,[&]{return head.get()!=get_tail();});
    return std::move(head_lock);  // 3
  }

  std::unique_ptr<node> wait_pop_head()
  {
    std::unique_lock<std::mutex> head_lock(wait_for_data());  // 4
    return pop_head();
  }

  std::unique_ptr<node> wait_pop_head(T& value)
  {
    std::unique_lock<std::mutex> head_lock(wait_for_data());  // 5
    value=std::move(*head->data);
    return pop_head();
  }
 public:
  std::shared_ptr<T> wait_and_pop()
  {
    std::unique_ptr<node> const old_head=wait_pop_head();
    return old_head->data;
  }

  void wait_and_pop(T& value)
  {
    std::unique_ptr<node> const old_head=wait_pop_head(value);
  }
 private:
  std::unique_ptr<node> try_pop_head()
  {
    std::lock_guard<std::mutex> head_lock(head_mutex);
    if(head.get()==get_tail())
    {
      return std::unique_ptr<node>();
    }
    return pop_head();
  }

  std::unique_ptr<node> try_pop_head(T& value)
  {
    std::lock_guard<std::mutex> head_lock(head_mutex);
    if(head.get()==get_tail())
    {
      return std::unique_ptr<node>();
    }
    value=std::move(*head->data);
    return pop_head();
  }
 public:
  std::shared_ptr<T> try_pop()
  {
    std::unique_ptr<node> old_head = try_pop_head();
    return old_head?old_head->data:std::shared_ptr<T>();
  }

  bool try_pop(T& value)
  {
    std::unique_ptr<node> const old_head=try_pop_head(value);
    return old_head;
  }

  bool empty()
  {
    std::lock_guard<std::mutex> head_lock(head_mutex);
    return (head.get()==get_tail());
  }
};

threadsafe_stack<int> s;
void testStack() {
  s.pop();
}
threadsafe_queue_cond_link<int> q;
void testQueue() {
  q.try_pop();
}
int main(int argc, char** argv) {
  /*
  for (int i = 0; i < 10; ++i) {s.push(i);}

  std::vector<std::thread> threads(20);
  for (unsigned long i = 0; i < 20; ++i) {
    threads[i] = std::thread(testStack);
  }

  std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
  */
  for (int i = 0; i < 10; ++i) {q.push(i);}

  std::vector<std::thread> threads(20);
  for (unsigned long i = 0; i < 20; ++i) {
    threads[i] = std::thread(testQueue);
  }

  std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
  return 0;
}
#endif
/////////////////////////6-2////////////////////////

/////////////////////////6-3////////////////////////
#if 0
#include <boost/thread/thread.hpp>
#include <list>
#include <thread>
#include <map>
template<typename Key,typename Value,typename Hash=std::hash<Key> >
class threadsafe_lookup_table {
 private:
  class bucket_type {
   public:
    typedef std::pair<Key,Value> bucket_value;
    typedef std::list<bucket_value> bucket_data;
    typedef typename bucket_data::iterator bucket_iterator;
    typedef typename bucket_data::const_iterator bucket_const_iterator;

    bucket_data data;
    mutable boost::shared_mutex mutex;  // 1

    bucket_const_iterator find_entry_for(Key const& key) const {
      return std::find_if(data.begin(),data.end(),
                          [&](bucket_value const& item)
                          {return item.first==key;});
    }
    bucket_iterator find_entry_for_update(Key const & key) {
      return std::find_if(data.begin(),data.end(),
                          [&](bucket_value const& item)
                          {return item.first==key;});
    }
   public:
    Value value_for(Key const& key,Value const& default_value) const {
      boost::shared_lock<boost::shared_mutex> lock(mutex);  // 3
      bucket_const_iterator found_entry = find_entry_for(key);
      return (found_entry == data.end())?
          default_value:found_entry->second;
    }

    void add_or_update_mapping(Key const& key,Value const& value) {
      std::unique_lock<boost::shared_mutex> lock(mutex);  // 4
      bucket_iterator found_entry=find_entry_for_update(key);
      if(found_entry==data.end()) {
        data.push_back(bucket_value(key,value));
      } else {
        found_entry->second=value;
      }
    }

    void remove_mapping(Key const& key) {
      std::unique_lock<boost::shared_mutex> lock(mutex);  // 5
      bucket_iterator found_entry=find_entry_for(key);
      if(found_entry!=data.end()) {
        data.erase(found_entry);
      }
    }
  };

  std::vector<std::unique_ptr<bucket_type> > buckets;  // 6
  Hash hasher;

  bucket_type& get_bucket(Key const& key) const  {
    std::size_t const bucket_index=hasher(key)%buckets.size();
    return *buckets[bucket_index];
  }

 public:
  typedef Key key_type;
  typedef Value mapped_type;

  typedef Hash hash_type;
  threadsafe_lookup_table(
      unsigned num_buckets=19,Hash const& hasher_=Hash()):
      buckets(num_buckets),hasher(hasher_) {
        for(unsigned i=0;i<num_buckets;++i) {
          buckets[i].reset(new bucket_type);
        }
      }

  threadsafe_lookup_table(threadsafe_lookup_table const& other)=delete;
  threadsafe_lookup_table& operator=(
      threadsafe_lookup_table const& other)=delete;

  Value value_for(Key const& key,
                  Value const& default_value=Value()) const {
    return get_bucket(key).value_for(key,default_value);
  }

  void add_or_update_mapping(Key const& key,Value const& value) {
    get_bucket(key).add_or_update_mapping(key,value);  // 9
  }

  void remove_mapping(Key const& key) {
    get_bucket(key).remove_mapping(key);  // 10
  }
  std::map<Key,Value> get_map() const {
    std::vector<std::unique_lock<boost::shared_mutex> > locks;
    for(unsigned i=0;i<buckets.size();++i) {
      locks.push_back(
          std::unique_lock<boost::shared_mutex>((*buckets[i]).mutex));
    }
    std::map<Key,Value> res;
    for(unsigned i=0;i<buckets.size();++i) {
      for(typename bucket_type::bucket_iterator it = (*buckets[i]).data.begin();
          it != (*buckets[i]).data.end();
          ++it) {
        res.insert(*it);
      }
    }
    return res;
  }
};

// 线程安全的链表
template<typename T>
class threadsafe_list {
  struct node  // 1
  {
    std::mutex m;
    std::shared_ptr<T> data;
    std::unique_ptr<node> next;
    node():  // 2
        next()
    {}

    node(T const& value):  // 3
        data(std::make_shared<T>(value))
    {}
  };

  node head;

 public:
  threadsafe_list()
  {}

  ~threadsafe_list()
  {
    remove_if([](node const&){return true;});
  }

  threadsafe_list(threadsafe_list const& other)=delete;
  threadsafe_list& operator=(threadsafe_list const& other)=delete;

  void push_front(T const& value)
  {
    std::unique_ptr<node> new_node(new node(value));  // 4
    std::lock_guard<std::mutex> lk(head.m);
    new_node->next=std::move(head.next);  // 5
    head.next=std::move(new_node);  // 6
  }

  template<typename Function>
  void for_each(Function f)  // 7
  {
    node* current=&head;
    std::unique_lock<std::mutex> lk(head.m);  // 8
    while(node* const next=current->next.get())  // 9
    {
      std::unique_lock<std::mutex> next_lk(next->m);  // 10
      lk.unlock();  // 11
      f(*next->data);  // 12
      current=next;
      lk=std::move(next_lk);  // 13
    }
  }

  template<typename Predicate>
  std::shared_ptr<T> find_first_if(Predicate p)  // 14
  {
    node* current=&head;
    std::unique_lock<std::mutex> lk(head.m);
    while(node* const next=current->next.get())
    {
      std::unique_lock<std::mutex> next_lk(next->m);
      lk.unlock();
      if(p(*next->data))  // 15
      {
        return next->data;  // 16
      }
      current=next;
      lk=std::move(next_lk);
    }
    return std::shared_ptr<T>();
  }

  template<typename Predicate>
  void remove_if(Predicate p)  // 17
  {
    node* current=&head;
    std::unique_lock<std::mutex> lk(head.m);
    while(node* const next=current->next.get())
    {
      std::unique_lock<std::mutex> next_lk(next->m);
      if(p(*next->data))  // 18
      {
        std::unique_ptr<node> old_next=std::move(current->next);
        current->next=std::move(next->next);
        next_lk.unlock();
      }  // 20
      else
      {
        lk.unlock();  // 21
        current=next;
        lk=std::move(next_lk);
      }
    }
  }
};

threadsafe_lookup_table<int,int> s;
void testlookuptable(int i) {
  int res = 0;
  res = s.value_for(i, res);
  s.add_or_update_mapping(i,i + 1);
}

void f(int i) { i++;}
bool g(int i) { return i == 4;}
int main(int argc, char** argv) {
  for (int i = 0; i < 10; ++i) {
    s.add_or_update_mapping(i,i);
  }

  std::vector<std::thread> threads(20);
  for (unsigned long i = 0; i < 20; ++i) {
    threads[i] = std::thread(testlookuptable, i);
  }

  std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

  std::cout << s.get_map().size();
  threadsafe_list<int> listt;
  listt.push_front(1);
  listt.push_front(2);
  listt.push_front(3);
  listt.push_front(4);

  listt.for_each(f);

  std::shared_ptr<int> tr = listt.find_first_if(g);
  std::cout << (*tr.get());
  return 0;
}
#endif
/////////////////////////6-3////////////////////////

/////////////////////////7-2////////////////////////
#if 1
// 基于引用计数和松散原子操作的无锁栈
template<typename T>
class lock_free_stack {
 private:
  struct node;
  struct counted_node_ptr {
    int external_count;
    node* ptr;
  };

  struct node {
    std::shared_ptr<T> data;
    std::atomic<int> internal_count;
    counted_node_ptr next;

    node(T const& data_):
        data(std::make_shared<T>(data_)),
        internal_count(0) {}
  };

  std::atomic<counted_node_ptr> head;
  std::atomic<int> stack_size;

  void increase_head_count(counted_node_ptr& old_counter) {
    counted_node_ptr new_counter;

    do {
      new_counter = old_counter;
      ++new_counter.external_count;
    }
    // compare_exchange_strong能够自动比较head与old_counter的值，
    // 如果二者相等,会将head的值修改为new_counter的值(执行read-modify-write操作)，
    // 否则将old_counter的值修改为head的值。
    while(!head.compare_exchange_strong(old_counter,new_counter,
                                        std::memory_order_acquire,
                                        std::memory_order_relaxed));
    old_counter.external_count = new_counter.external_count;
  }
 public:
  ~lock_free_stack() {
    while(pop());
  }

  lock_free_stack() : stack_size(0) {
  }

  int size() {
    return stack_size;
  } 
  bool empty() {
     counted_node_ptr old_head=
        head.load(std::memory_order_relaxed);
     if (!old_head.ptr) {
       return true;
     }

     return false;
  }

  std::shared_ptr<T> top() {
    counted_node_ptr old_head=
        head.load(std::memory_order_relaxed);
    increase_head_count(old_head);
    node* const ptr = old_head.ptr;
      if(!ptr) {
        return std::shared_ptr<T>();
    }
    return ptr->data;
  }

  void push(T const& data) {
    counted_node_ptr new_node;
    new_node.ptr = new node(data);
    new_node.external_count = 1;
    new_node.ptr->next = head.load(std::memory_order_relaxed);
    while(!head.compare_exchange_weak(new_node.ptr->next,new_node,
                                      std::memory_order_release,
                                      std::memory_order_relaxed));
    ++stack_size;
  }
  std::shared_ptr<T> pop() {
    counted_node_ptr old_head=
        head.load(std::memory_order_relaxed);
    for(;;) {
      increase_head_count(old_head);
      node* const ptr = old_head.ptr;
      if(!ptr) {
        return std::shared_ptr<T>();
      }
      if(head.compare_exchange_strong(old_head,ptr->next,
                                      std::memory_order_relaxed)) {
        std::shared_ptr<T> res;
        res.swap(ptr->data);

        int const count_increase = old_head.external_count - 2;

        if(ptr->internal_count.fetch_add(count_increase,
                   std::memory_order_release) == -count_increase) {
          delete ptr;
        }
        --stack_size;
        return res;
      }
      else if(ptr->internal_count.fetch_add(-1,
                      std::memory_order_relaxed) == 1) {
        ptr->internal_count.load(std::memory_order_acquire);
        --stack_size;
        delete ptr;
      }
    }
  }
};

// 线程安全的无锁队列
template<typename T>
class lock_free_queue {
 private:
  struct node;
  struct counted_node_ptr {
    int external_count;
    node* ptr;
  };
  struct node_counter {
    unsigned internal_count:30;
    unsigned external_counters:2;  // 2
  };

  std::atomic<counted_node_ptr> head;
  std::atomic<counted_node_ptr> tail;  // 1
 private:
  static void increase_external_count(
      std::atomic<counted_node_ptr>& counter,
      counted_node_ptr& old_counter) {
    counted_node_ptr new_counter;
    do {
      new_counter=old_counter;
      ++new_counter.external_count;
    }
    while(!counter.compare_exchange_strong(
            old_counter,new_counter,
            std::memory_order_acquire,std::memory_order_relaxed));

    old_counter.external_count=new_counter.external_count;
  }
 private:
  static void free_external_counter(counted_node_ptr &old_node_ptr) {
    node* const ptr=old_node_ptr.ptr;
    int const count_increase=old_node_ptr.external_count-2;

    node_counter old_counter=
        ptr->count.load(std::memory_order_relaxed);
    node_counter new_counter;
    do {
      new_counter=old_counter;
      --new_counter.external_counters;  // 1
      new_counter.internal_count+=count_increase;  // 2
    }
    while(!ptr->count.compare_exchange_strong(  // 3
            old_counter,new_counter,
            std::memory_order_acquire,std::memory_order_relaxed));

    if(!new_counter.internal_count &&
       !new_counter.external_counters) {
      delete ptr;  // 4
    }
  }
 private:
  struct node {
    std::atomic<T*> data;
    std::atomic<node_counter> count;
    std::atomic<counted_node_ptr> next;  // 1
    void release_ref() {
      node_counter old_counter=
          count.load(std::memory_order_relaxed);
      node_counter new_counter;
      do {
        new_counter=old_counter;
        --new_counter.internal_count;  // 1
      }
      while(!count.compare_exchange_strong(  // 2
              old_counter,new_counter,
              std::memory_order_acquire,std::memory_order_relaxed));
      if(!new_counter.internal_count &&
         !new_counter.external_counters) {
        delete this;  // 3
      }
    }
  };
 public:
  std::unique_ptr<T> pop() {
    counted_node_ptr old_head=head.load(std::memory_order_relaxed);
    for(;;) {
      increase_external_count(head,old_head);
      node* const ptr=old_head.ptr;
      if(ptr==tail.load().ptr) {
        return std::unique_ptr<T>();
      }
      counted_node_ptr next=ptr->next.load();  // 2
      if(head.compare_exchange_strong(old_head,next)) {
        T* const res=ptr->data.exchange(nullptr);
        free_external_counter(old_head);
        return std::unique_ptr<T>(res);
      }
      ptr->release_ref();
    }
  }
 private:
  void set_new_tail(counted_node_ptr &old_tail,  // 1
                    counted_node_ptr const &new_tail) {
    node* const current_tail_ptr=old_tail.ptr;
    while(!tail.compare_exchange_weak(old_tail,new_tail) &&  // 2
          old_tail.ptr==current_tail_ptr);
    if(old_tail.ptr==current_tail_ptr)  // 3
      free_external_counter(old_tail);  // 4
    else
      current_tail_ptr->release_ref();  // 5
  }
 public:
  void push(T new_value) {
    std::unique_ptr<T> new_data(new T(new_value));
    counted_node_ptr new_next;
    new_next.ptr=new node;
    new_next.external_count=1;
    counted_node_ptr old_tail=tail.load();

    for(;;) {
      increase_external_count(tail,old_tail);

      T* old_data=nullptr;
      if(old_tail.ptr->data.compare_exchange_strong(  // 6
              old_data,new_data.get())) {
        counted_node_ptr old_next={0};
        if(!old_tail.ptr->next.compare_exchange_strong(  // 7
                old_next,new_next)) {
          delete new_next.ptr;  // 8
          new_next=old_next;  // 9
        }
        set_new_tail(old_tail, new_next);
        new_data.release();
        break;
      }
      else  // 10
      {
        counted_node_ptr old_next={0};
        if(old_tail.ptr->next.compare_exchange_strong(  // 11
                old_next,new_next))
        {
          old_next=new_next;  // 12
          new_next.ptr=new node;  // 13
        }
        set_new_tail(old_tail, old_next);  // 14
      }
    }
  }
};

lock_free_stack<int> s;
std::mutex m;
void testStack() {
  static int i = 0;
  if (s.empty()) {
    std::cout <<"+" << std::endl;
  }
  std::shared_ptr<int> res = s.top();
  std::lock_guard<std::mutex> g(m);
  s.pop();
  s.pop();
  if (res)
    std::cout << *res.get() << std::endl;
  std::cout << s.size();
}
lock_free_queue<int> q;
void testQueue() {
  q.pop();
}
int main(int argc, char** argv) {
  
  for (int i = 0; i < 10; ++i) {s.push(i);}

  std::vector<std::thread> threads(20);
  for (unsigned long i = 0; i < 20; ++i) {
    threads[i] = std::thread(testStack);
  }

  std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
  


 /*
  for (int i = 0; i < 10; ++i) {q.push(i);}

  std::vector<std::thread> threads(20);
  for (unsigned long i = 0; i < 20; ++i) {
    threads[i] = std::thread(testQueue);
  }

  std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
*/
  return 0;
}
#endif
/////////////////////////7-2////////////////////////

/////////////////////////8-1////////////////////////
#if 0
#include <exception>
#include <memory>
#include <stack>

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
    data.push(std::move(new_value));
  }

  std::shared_ptr<T> pop() {
    std::lock_guard<std::mutex> lock(m);
    if (data.empty()) {
      return NULL;
      throw empty_stack();
    }

    std::shared_ptr<T> const res(std::make_shared<T>(std::move(data.top())));
    data.pop();
    return res;
  }

  void pop(T& value) {
    std::lock_guard<std::mutex> lock(m);
    if (data.empty()) {
      return;
      throw empty_stack();
    }

    value = std::move(data.top());
    data.pop();
  }

  bool empty() const {
    std::lock_guard<std::mutex> lock(m);
    return data.empty();
  }
};

template<typename T>
struct sorter {
  struct chunk_to_sort {
    std::list<T> data;
    std::promise<std::list<T> > promise;
  };

  threadsafe_stack<chunk_to_sort> chunks;  // 2
  //lock_free_stack<chunk_to_sort> chunks;  // 2
  std::vector<std::thread> threads;  // 3
  unsigned const max_thread_count;
  std::atomic<bool> end_of_data;

  sorter():
      max_thread_count(std::thread::hardware_concurrency()-1),
      end_of_data(false) {}

  ~sorter() {
    end_of_data=true;  // 5

    for(unsigned i = 0; i < threads.size(); ++i) {
      threads[i].join();  // 6
    }
  }

  void try_sort_chunk() {
    if (chunks.empty()) {
      return;
    }
    std::shared_ptr<chunk_to_sort > chunk = chunks.pop();  // 7
    if(chunk) {
      sort_chunk(chunk);  // 8
    }
  }

  std::list<T> do_sort(std::list<T>& chunk_data) {
    if(chunk_data.empty()) {
      return chunk_data;
    }

    std::list<T> result;
    result.splice(result.begin(),chunk_data,chunk_data.begin());
    T const& partition_val=*result.begin();

    typename std::list<T>::iterator divide_point =
        std::partition(chunk_data.begin(),chunk_data.end(),
                       [&](T const& val){return val<partition_val;});

    chunk_to_sort new_lower_chunk;
    new_lower_chunk.data.splice(new_lower_chunk.data.end(),
                                chunk_data,chunk_data.begin(),
                                divide_point);

    std::future<std::list<T> > new_lower=
        new_lower_chunk.promise.get_future();
    chunks.push(std::move(new_lower_chunk));  // 11
    if(threads.size()<max_thread_count) {
      threads.push_back(std::thread(&sorter<T>::sort_thread,this));
    }


    std::list<T> new_higher(do_sort(chunk_data));

    result.splice(result.end(),new_higher);
    while(new_lower.wait_for(std::chrono::seconds(0)) !=
          std::future_status::ready) {
      try_sort_chunk();  // 14
    }

    result.splice(result.begin(),new_lower.get());
    return result;
  }

  void sort_chunk(std::shared_ptr<chunk_to_sort> const& chunk) {
    chunk->promise.set_value(do_sort(chunk->data));  // 15
  }

  void sort_thread() {
    while(!end_of_data) {
      try_sort_chunk();  // 17
      std::this_thread::yield();  // 18
    }
  }
};

template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input) {
  if(input.empty()) {
    return input;
  }
  sorter<T> s;

  return s.do_sort(input);  // 20
}

int main(int argc, char** argv) {
  std::list<int> l{9,8,7,6,5,1,2,3,4};
  //std::cout << l.size() << std::endl;;
  std::list<int> result = parallel_quick_sort<int>(l);

  for (std::list<int>::iterator it = result.begin();
       it != result.end(); ++it)
  {
    std::cout << *it << std::endl;
  }
  return 0;
}
#endif
/////////////////////////8-1////////////////////////

/////////////////////////8-2////////////////////////
#if 0
#endif
/////////////////////////8-2////////////////////////

/////////////////////////8-3////////////////////////
#if 0
#endif
/////////////////////////8-3////////////////////////

/////////////////////////8-4////////////////////////
#if 0
template <typename Iterator, typename T>
struct accumulate_block {
  void operator()(Iterator first, Iterator last, T& result) {
    result = std::accumulate(first, last, result);
  }
};

template<typename Iterator,typename T>
T accumulate_block2(Iterator begin, Iterator end) {
  return std::accumulate(begin, end, T());
}

class join_threads
{
  std::vector<std::thread>& threads;
 public:
  explicit join_threads(std::vector<std::thread>& threads_):
      threads(threads_)
  {}
  ~join_threads()
  {
    for(unsigned long i=0;i<threads.size();++i)
    {
      if(threads[i].joinable())
        threads[i].join();
    }
  }
};

template<typename Iterator,typename T>
T parallel_accumulate(Iterator first,Iterator last,T init)
{
  unsigned long const length=std::distance(first,last);

  if(!length)
    return init;

  unsigned long const min_per_thread=25;
  unsigned long const max_threads=
      (length+min_per_thread-1)/min_per_thread;

  unsigned long const hardware_threads=
      std::thread::hardware_concurrency();

  unsigned long const num_threads=
      std::min(hardware_threads!=0?hardware_threads:2,max_threads);

  unsigned long const block_size=length/num_threads;

  std::vector<std::future<T> > futures(num_threads-1);
  std::vector<std::thread> threads(num_threads-1);
  join_threads joiner(threads);  // 1

  Iterator block_start=first;
  for(unsigned long i=0;i<(num_threads-1);++i)
  {
    Iterator block_end=block_start;
    std::advance(block_end,block_size);
    std::packaged_task<T(Iterator,Iterator)> task(accumulate_block2<Iterator,T>);
    futures[i]=task.get_future();
    threads[i]=std::thread(std::move(task),block_start,block_end);
    block_start=block_end;
  }
  T last_result = accumulate_block2<Iterator, T>(block_start,last);
  T result=init;
  for(unsigned long i=0;i<(num_threads-1);++i)
  {
    result+=futures[i].get();  // 2
  }
  result += last_result;
  return result;
}


// async
template<typename Iterator,typename T>
T parallel_accumulate_2(Iterator first,Iterator last,T init)
{
  unsigned long const length=std::distance(first,last);  // 1
  unsigned long const max_chunk_size=25;
  if(length<=max_chunk_size)
  {
    return std::accumulate(first,last,init);  // 2
  }
  else
  {
    Iterator mid_point=first;
    std::advance(mid_point,length/2);  // 3
    std::future<T> first_half_result=
        std::async(parallel_accumulate<Iterator,T>,  // 4
                   first,mid_point,init);
    T second_half_result=parallel_accumulate_2(mid_point,last,T());  // 5
    return first_half_result.get()+second_half_result;  // 6
  }
}

int main(int argc, char** argv) {
  std::vector<int> vec;
  for (int i = 0; i < 11; ++i) {
    vec.push_back(i);
  }
  std::cout << parallel_accumulate<std::vector<int>::iterator, int>(vec.begin(), vec.end(), 0) << std::endl;
  std::cout << parallel_accumulate_2<std::vector<int>::iterator, int>(vec.begin(), vec.end(), 0) << std::endl;
  std::cout <<"End\n";
  return 0;
}
#endif
/////////////////////////8-4////////////////////////

/////////////////////////8-5////////////////////////
#if 0
//并行版std::for_each
class join_threads
{
  std::vector<std::thread>& threads;
 public:
  explicit join_threads(std::vector<std::thread>& threads_):
      threads(threads_)
  {}
  ~join_threads()
  {
    for(unsigned long i=0;i<threads.size();++i)
    {
      if(threads[i].joinable())
        threads[i].join();
    }
  }
};
template<typename Iterator,typename Func>
void parallel_for_each(Iterator first,Iterator last,Func f)
{
  unsigned long const length=std::distance(first,last);

  if(!length)
    return;

  unsigned long const min_per_thread=25;
  unsigned long const max_threads=
      (length+min_per_thread-1)/min_per_thread;

  unsigned long const hardware_threads=
      std::thread::hardware_concurrency();

  unsigned long const num_threads=
      std::min(hardware_threads!=0?hardware_threads:2,max_threads);

  unsigned long const block_size=length/num_threads;

  std::vector<std::future<void> > futures(num_threads-1);  // 1
  std::vector<std::thread> threads(num_threads-1);
  join_threads joiner(threads);

  Iterator block_start=first;
  for(unsigned long i=0;i<(num_threads-1);++i)
  {
    Iterator block_end=block_start;
    std::advance(block_end,block_size);
    std::packaged_task<void(void)> task(  // 2
        [=]()
        {
        std::for_each(block_start,block_end,f);
        });
    futures[i]=task.get_future();
    threads[i]=std::thread(std::move(task));  // 3
    block_start=block_end;
  }
  std::for_each(block_start,last,f);
  for(unsigned long i=0;i<(num_threads-1);++i)
  {
    futures[i].get();  // 4
  }
}

//使用std::async实现std::for_each

template<typename Iterator,typename Func>
void parallel_for_each_2(Iterator first,Iterator last,Func f)
{
  unsigned long const length=std::distance(first,last);

  if(!length)
    return;

  unsigned long const min_per_thread=25;

  if(length<(2*min_per_thread))
  {
    std::for_each(first,last,f);  // 1
  }
  else
  {
    Iterator const mid_point=first+length/2;
    std::future<void> first_half=  // 2
        std::async(&parallel_for_each_2<Iterator,Func>,
                   first,mid_point,f);
    parallel_for_each_2(mid_point,last,f);  // 3
    first_half.get();  // 4
  }
}

//并行find算法实现

template<typename Iterator,typename MatchType>
Iterator parallel_find(Iterator first,Iterator last,MatchType match)
{
  struct find_element  // 1
  {
    void operator()(Iterator begin,Iterator end,
                    MatchType match,
                    std::promise<Iterator>* result,
                    std::atomic<bool>* done_flag)
    {
      try
      {
        for(;(begin!=end) && !done_flag->load();++begin)  // 2
        {
          if(*begin==match)
          {
            result->set_value(begin);  // 3
            done_flag->store(true);  // 4
            return;
          }
        }
      }
      catch(...)  // 5
      {
        try
        {
          result->set_exception(std::current_exception());  // 6
          done_flag->store(true);
        }
        catch(...)  // 7
        {}
      }
    }
  };

  unsigned long const length=std::distance(first,last);

  if(!length)
    return last;

  unsigned long const min_per_thread=25;
  unsigned long const max_threads=
      (length+min_per_thread-1)/min_per_thread;

  unsigned long const hardware_threads=
      std::thread::hardware_concurrency();

  unsigned long const num_threads=
      std::min(hardware_threads!=0?hardware_threads:2,max_threads);

  unsigned long const block_size=length/num_threads;

  std::promise<Iterator> result;  // 8
  std::atomic<bool> done_flag(false);  // 9
  std::vector<std::thread> threads(num_threads-1);
  {  // 10
    join_threads joiner(threads);

    Iterator block_start=first;
    for(unsigned long i=0;i<(num_threads-1);++i)
    {
      Iterator block_end=block_start;
      std::advance(block_end,block_size);
      threads[i]=std::thread(find_element(),  // 11
                             block_start,block_end,match,
                             &result,&done_flag);
      block_start=block_end;
    }
    find_element()(block_start,last,match,&result,&done_flag);  // 12
  }
  if(!done_flag.load())  //13
  {
    return last;
  }
  return result.get_future().get();  // 14
}

//使用std::async实现的并行find算法

template<typename Iterator,typename MatchType>  // 1
Iterator parallel_find_impl(Iterator first,Iterator last,MatchType match,
                            std::atomic<bool>& done) {
  try {
    unsigned long const length=std::distance(first,last);
    unsigned long const min_per_thread=25;  // 2
    if(length<(2*min_per_thread)) {
      for(;(first!=last) && !done.load();++first) {
        if(*first==match) {
          done=true;  // 5
          return first;
        }
      }
      return last;  // 6
    } else {
      Iterator const mid_point=first+(length/2);  // 7
      std::future<Iterator> async_result=
          std::async(&parallel_find_impl<Iterator,MatchType>,  // 8
                     mid_point,last,match,std::ref(done));
      Iterator const direct_result=
          parallel_find_impl(first,mid_point,match,done);  // 9
      return (direct_result==mid_point)?
          async_result.get():direct_result;  // 10
    }
  }
  catch(...) {
    done=true;
    throw;
  }
}

template<typename Iterator,typename MatchType>
Iterator parallel_find_2m(Iterator first,Iterator last,MatchType match) {
  std::atomic<bool> done(false);
  return parallel_find_impl(first,last,match,done);  // 12
}

//通过两两更新对的方式实现partial_sum
struct barrier {
  std::atomic<unsigned> count;
  std::atomic<unsigned> spaces;
  std::atomic<unsigned> generation;

  barrier(unsigned count_):
      count(count_),spaces(count_),generation(0) {}

  void wait() {
    unsigned const gen=generation.load();
    if(!--spaces) {
      spaces=count.load();
      ++generation;
    }
    else {
      while(generation.load()==gen) {
        std::this_thread::yield();
      }
    }
  }

  void done_waiting() {
    --count;
    if(!--spaces) {
      spaces=count.load();
      ++generation;
    }
  }
};

template<typename Iterator>
void parallel_partial_sum(Iterator first,Iterator last) {
  typedef typename Iterator::value_type value_type;

  struct process_element {
    void operator()(Iterator first,Iterator last,
                    std::vector<value_type>& buffer,
                    unsigned i, barrier& b) {
      value_type& ith_element=*(first+i);
      bool update_source=false;

      for(unsigned step=0,stride=1;stride<=i;++step,stride*=2) {
        value_type const& source=(step%2)?  // 2
            buffer[i]:ith_element;

        value_type& dest=(step%2)?
            ith_element:buffer[i];

        value_type const& addend=(step%2)?  // 3
            buffer[i-stride]:*(first+i-stride);

        dest=source+addend;  // 4
        update_source=!(step%2);
        b.wait();  // 5
      }

      if(update_source) {
        ith_element=buffer[i];
      }
      b.done_waiting();
    }
  };

  unsigned long const length = std::distance(first,last);

  if(length <= 1) {
    return;
  }

  std::vector<value_type> buffer(length);
  barrier b(length);

  std::vector<std::thread> threads(length-1);  // 8
  join_threads joiner(threads);

  for(unsigned long i=0;i<(length-1);++i) {
    threads[i]=std::thread(process_element(),first,last,  // 9
                           std::ref(buffer),i,std::ref(b));
  }
  process_element()(first,last,buffer,length-1,b);  // 10
}

struct Add {
  void operator () (int i) {
    std::cout<< i << std::endl;
  }
};


int main(int argc, char** argv) {
   std::vector<int> vec;
  for (int i = 0; i < 11; ++i) {
    vec.push_back(i);
  }

  parallel_for_each(vec.begin(), vec.end(), Add());
  parallel_for_each_2(vec.begin(), vec.end(), Add());

  if (parallel_find(vec.begin(), vec.end(), 5) != vec.end()) {
    std::cout <<"Yes\n";
  }

  if (parallel_find_2m(vec.begin(), vec.end(), 5) != vec.end()) {
    std::cout <<"Yes\n";
  }

  parallel_partial_sum(vec.begin(), vec.end());

  for (int i = 0; i < 11; ++i) {
    std::cout << vec[i] << std::endl;
  }
  std::cout <<"End\n";
  return 0;
}
#endif
/////////////////////////8-5////////////////////////
/////////////////////////9-1////////////////////////
#if 0
template <typename Iterator>
class function_wrapper {
  struct impl_base {
    virtual void call(Iterator b, Iterator e)=0;
    virtual ~impl_base() {}
  };

  std::unique_ptr<impl_base> impl;
  template<typename F>
  struct impl_type: impl_base {
    F f;
    impl_type(F&& f_): f(std::move(f_)) {}
    void call(Iterator b, Iterator e) { f(b, e); }
  };
 public:
  template<typename F>
  function_wrapper(F&& f):
    impl(new impl_type<F>(std::move(f))) {}

  void operator()(Iterator b, Iterator e) { impl->call(b, e); }

  function_wrapper() = default;

  function_wrapper(function_wrapper&& other):
      impl(std::move(other.impl)) {}

  function_wrapper& operator=(function_wrapper&& other) {
    impl=std::move(other.impl);
    return *this;
  }

  function_wrapper(const function_wrapper&)=delete;
  function_wrapper(function_wrapper&)=delete;
  function_wrapper& operator=(const function_wrapper&)=delete;
};

class join_threads {
  std::vector<std::thread>& threads;
 public:
  explicit join_threads(std::vector<std::thread>& threads_):
      threads(threads_) {}
  ~join_threads() {
    for(unsigned long i=0;i<threads.size();++i) {
      if(threads[i].joinable())
        threads[i].join();
    }
  }
};

template<typename T>
class threadsafe_queue {
 private:
  mutable std::mutex mut;
  std::queue<T> data_queue;
  std::condition_variable data_cond;
 public:
  threadsafe_queue() {}
  threadsafe_queue(threadsafe_queue const& other) {
    std::lock_guard<std::mutex> lk(other.mut);
    data_queue=other.data_queue;
  }

  void push(T new_value) {
    std::lock_guard<std::mutex> lk(mut);
    data_queue.push(std::move(new_value));
    data_cond.notify_one();
  }

  void wait_and_pop(T& value) {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk,[this]{return !data_queue.empty();});
    value=std::move(data_queue.front());
    data_queue.pop();
  }

  std::shared_ptr<T> wait_and_pop() {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk,[this]{return !data_queue.empty();});
    std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
    data_queue.pop();
    return res;
  }

  bool try_pop(T& value) {
    std::lock_guard<std::mutex> lk(mut);
    if(data_queue.empty())
      return false;
    value = std::move(data_queue.front());
    data_queue.pop();
    return true;
  }

  std::shared_ptr<T> try_pop() {
    std::lock_guard<std::mutex> lk(mut);
    if(data_queue.empty())
      return std::shared_ptr<T>();
    std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
    data_queue.pop();
    return res;
  }

  bool empty() const {
    std::lock_guard<std::mutex> lk(mut);
    return data_queue.empty();
  }
};

template<typename Iterator, typename T>
class thread_pool {
  std::atomic_bool done;
  std::vector<std::thread> threads;  // 2
  join_threads joiner;  // 3
  Iterator b;
  Iterator e;
  threadsafe_queue<function_wrapper<Iterator> > work_queue;  // 使用function_wrapper，而非使用std::function

  void worker_thread() {
    while(!done) {
      function_wrapper<Iterator> task;
      if(work_queue.try_pop(task)) {
        task(b,e);
      } else {
        std::this_thread::yield();
      }
    }
  }
 public:
  void run_pending_task() {
    function_wrapper<Iterator> task;
    if(work_queue.try_pop(task)) {
      task(b ,e);
    } else {
      std::this_thread::yield();
    }
  }
  thread_pool():
      done(false),joiner(threads) {
    unsigned const thread_count=std::thread::hardware_concurrency();  // 8

    try {
      for(unsigned i=0;i<thread_count;++i) {
        threads.push_back( 
            std::thread(&thread_pool::worker_thread,this));  // 9
      }
    } catch(...) {
      done=true;  // 10
      throw;
    }
  }

  ~thread_pool() {
    done=true;  // 11
  }
  //template<typename T, typename Iterator>
  std::future<T> submit(T (*f)(Iterator, Iterator), Iterator beg, Iterator end) {
    std::packaged_task<T(Iterator, Iterator)> task(std::move(f));  // 3
    std::future<T> res(task.get_future());  // 4
    work_queue.push(std::move(task));  // 5
    b = beg;
    e = end;
    return res;  // 6
  }
};


template<typename Iterator, typename T>
class thread_pool2 {
 private:
  threadsafe_queue<function_wrapper<Iterator> > pool_work_queue;

  Iterator b;
  Iterator e;
  std::atomic_bool done;
  std::vector<std::thread> threads;  // 2
  join_threads joiner;  // 3
 public:
  typedef std::queue<function_wrapper<Iterator> > local_queue_type;  // 1
  static thread_local std::unique_ptr<local_queue_type> local_work_queue;  // 2

  void worker_thread() {
    local_work_queue.reset(new local_queue_type);  // 3
    while(!done) {
      run_pending_task();
    }
  }

void run_pending_task() {
    function_wrapper<Iterator> task;
    if(local_work_queue && !local_work_queue->empty()) { // 6
      task=std::move(local_work_queue->front());
      local_work_queue->pop();
      task(b,e);
    } else if(pool_work_queue.try_pop(task)) { // 7
      task(b,e);
    } else {
      std::this_thread::yield();
    }
  }
 public:
  std::future<T> submit(T (*f)(Iterator, Iterator), Iterator beg, Iterator end) {
    std::packaged_task<T(Iterator, Iterator)> task(std::move(f));  // 3
    std::future<T> res(task.get_future());  // 4
    if(local_work_queue) { // 4
      local_work_queue->push(std::move(task));
    } else {
      pool_work_queue.push(std::move(task));  // 5
    }
    b = beg;
    e = end;
    return res;  // 6
  }

  thread_pool2(): done(false),joiner(threads) {
    unsigned const thread_count=std::thread::hardware_concurrency();  // 8

    try {
      for(unsigned i=0;i<thread_count;++i) {
        threads.push_back( 
            std::thread(&thread_pool2::worker_thread,this));  // 9
      }
    } catch(...) {
      done=true;  // 10
      throw;
    }
  }

  ~thread_pool2() {
    done=true;  // 11
  }
};

template<typename Iterator, typename T>
thread_local std::unique_ptr<typename thread_pool2<Iterator, T>::local_queue_type > thread_pool2<Iterator, T>::local_work_queue;

template<typename Iterator,typename T>
class work_stealing_queue {
 private:
  typedef function_wrapper<Iterator> data_type;
  std::deque<data_type> the_queue;  // 1
  mutable std::mutex the_mutex;

 public:
  work_stealing_queue() {}

  work_stealing_queue(const work_stealing_queue& other)=delete;
  work_stealing_queue& operator=(
      const work_stealing_queue& other)=delete;

  void push(data_type data) {
    std::lock_guard<std::mutex> lock(the_mutex);
    the_queue.push_front(std::move(data));
  }

  bool empty() const {
    std::lock_guard<std::mutex> lock(the_mutex);
    return the_queue.empty();
  }

  bool try_pop(data_type& res) {
    std::lock_guard<std::mutex> lock(the_mutex);
    if(the_queue.empty()) {
      return false;
    }

    res=std::move(the_queue.front());
    the_queue.pop_front();
    return true;
  }

  bool try_steal(data_type& res) {
    std::lock_guard<std::mutex> lock(the_mutex);
    if(the_queue.empty()) {
      return false;
    }

    res=std::move(the_queue.back());
    the_queue.pop_back();
    return true;
  }
};

template<typename Iterator,typename T>
class thread_pool3 {
  typedef function_wrapper<Iterator> task_type;

  Iterator b;
  Iterator e;
  std::atomic_bool done;
  threadsafe_queue<task_type> pool_work_queue;
  std::vector<std::unique_ptr<work_stealing_queue<Iterator, T> > > queues;  // 1
  std::vector<std::thread> threads;
  join_threads joiner;

  static thread_local work_stealing_queue<Iterator, T>* local_work_queue;  // 2
  static thread_local unsigned my_index;

  void worker_thread(unsigned my_index_) {
    my_index=my_index_;
    local_work_queue=queues[my_index].get();  // 3
    while(!done) {
      run_pending_task();
    }
  }

  bool pop_task_from_local_queue(task_type& task) {
    return local_work_queue && local_work_queue->try_pop(task);
  }

  bool pop_task_from_pool_queue(task_type& task) {
    return pool_work_queue.try_pop(task);
  }

  bool pop_task_from_other_thread_queue(task_type& task) {
    for(unsigned i=0;i<queues.size();++i) {
      unsigned const index=(my_index+i+1)%queues.size();  // 5
      if(queues[index]->try_steal(task)) {
        return true;
      }
    }
    return false;
  }

 public:
  thread_pool3():
      done(false),joiner(threads) {
    unsigned const thread_count=std::thread::hardware_concurrency();

    try {
      for(unsigned i=0;i<thread_count;++i) {
        queues.push_back(std::unique_ptr<work_stealing_queue<Iterator, T> >(  // 6
                new work_stealing_queue<Iterator, T>));
        threads.push_back(
            std::thread(&thread_pool3::worker_thread,this,i));
      }
    } catch(...) {
      done=true;
      throw;
    }
  }

  ~thread_pool3() {
    done=true;
  }

  std::future<T> submit(T (*f)(Iterator, Iterator), Iterator beg, Iterator end) {
    std::packaged_task<T(Iterator, Iterator)> task(std::move(f));  // 3
    std::future<T> res(task.get_future());  // 4
    if(local_work_queue) { // 4
      local_work_queue->push(std::move(task));
    } else {
      pool_work_queue.push(std::move(task));  // 5
    }
    b = beg;
    e = end;
    return res;  // 6
  }

  void run_pending_task() {
    task_type task;
    if(pop_task_from_local_queue(task) ||  // 7
       pop_task_from_pool_queue(task) ||  // 8
       pop_task_from_other_thread_queue(task)) {
      task(b,e);
    } else {
      std::this_thread::yield();
    }
  }
};


template<typename Iterator,typename T>
thread_local work_stealing_queue<Iterator, T>* thread_pool3<Iterator, T>::local_work_queue;

template<typename Iterator,typename T>
thread_local unsigned thread_pool3<Iterator, T>::my_index;

template<typename Iterator,typename T>
T accumulate_block(Iterator begin, Iterator end) {
  return std::accumulate(begin, end, T());
}

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first,Iterator last,T init) {
  unsigned long const length=std::distance(first,last);

  if(!length)
    return init;

  unsigned long const block_size=25;
  unsigned long const num_blocks=(length+block_size-1)/block_size;  // 1

  std::vector<std::future<T> > futures(num_blocks-1);
  thread_pool3<Iterator, T> pool;

  Iterator block_start=first;
  for(unsigned long i=0;i<(num_blocks-1);++i) {
    Iterator block_end=block_start;
    std::advance(block_end,block_size);
    futures[i] = pool.submit(accumulate_block<Iterator, T>, block_start, block_end);
    block_start = block_end;
  }
  T last_result =  accumulate_block<Iterator,T>(block_start, last);
  T result=init;
  for(unsigned long i=0;i<(num_blocks-1);++i) {
    result+=futures[i].get();
  }
  result += last_result;
  return result;
}

//,基于线程池的快速排序实现
/*
template<typename T>
struct sorter {
  thread_pool pool;  // 2

  std::list<T> do_sort(std::list<T>& chunk_data) {
    if(chunk_data.empty()) {
      return chunk_data;
    }

    std::list<T> result;
    result.splice(result.begin(),chunk_data,chunk_data.begin());
    T const& partition_val=*result.begin();

    typename std::list<T>::iterator divide_point=
        std::partition(chunk_data.begin(),chunk_data.end(),
                       [&](T const& val){return val<partition_val;});

    std::list<T> new_lower_chunk;
    new_lower_chunk.splice(new_lower_chunk.end(),
                           chunk_data,chunk_data.begin(),
                           divide_point);

    std::future<std::list<T> > new_lower=  // 3
        pool.submit(std::bind(&sorter::do_sort,this,
                              std::move(new_lower_chunk)));

    std::list<T> new_higher(do_sort(chunk_data));

    result.splice(result.end(),new_higher);
    while(!new_lower.wait_for(std::chrono::seconds(0)) ==
          std::future_status::timeout) {
      pool.run_pending_task();  // 4
    }

    result.splice(result.begin(),new_lower.get());
    return result;
  }
};

template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input) {
  if(input.empty()) {
    return input;
  }
  sorter<T> s;

  return s.do_sort(input);
}
*/
int main(int argc, char** argv) {
  std::vector<int> vec;
  for (int i = 0; i < 11; ++i) {
    vec.push_back(i);
  }
  std::cout << parallel_accumulate<std::vector<int>::iterator, int>(vec.begin(), vec.end(), 0) << std::endl;
  std::cout <<"End\n";
  return 0;
}
#endif
/////////////////////////9-1////////////////////////

/////////////////////////9-2////////////////////////
#if 0
#include <functional>
#include <thread>
#include <future>
#include <iostream>
#include <chrono>
#include <random>
#include <mutex>

#include<condition_variable>
using namespace std;
using std::function;

struct thread_interrupted {};
void interruption_point();

class interrupt_flag {
 public:
  std::atomic<bool> flag;
  std::condition_variable* thread_cond;
  std::condition_variable_any* thread_cond_any;
  std::mutex set_clear_mutex;

 public:
  interrupt_flag():
      thread_cond(0),thread_cond_any(0) {}

  void set() {
    flag.store(true,std::memory_order_relaxed);
    std::lock_guard<std::mutex> lk(set_clear_mutex);
    if(thread_cond) {
      thread_cond->notify_all();
    } else if(thread_cond_any) {
      thread_cond_any->notify_all();
    }
  }

  bool is_set() const {
    bool res = flag.load(std::memory_order_relaxed);
    return res;
  }

  void set_condition_variable(std::condition_variable& cv) {
    std::lock_guard<std::mutex> lk(set_clear_mutex);
    thread_cond=&cv;
  }

  void set_condition_variable_any(std::condition_variable_any& cv) {
    std::lock_guard<std::mutex> lk(set_clear_mutex);
    thread_cond_any = &cv;
  }

  void clear_condition_variable() {
    std::lock_guard<std::mutex> lk(set_clear_mutex);
    thread_cond = 0;
  }

  void clear_condition_variable_any() {
    std::lock_guard<std::mutex> lk(set_clear_mutex);
    thread_cond_any = 0;
  }
};

thread_local interrupt_flag this_thread_interrupt_flag;

void interruption_point() {
  if(this_thread_interrupt_flag.is_set()) {
    cout << "Interrupt" << endl;
    throw thread_interrupted();
  }
}


class interruptible_thread {
 public:
  std::thread internal_thread;
  interrupt_flag* flag;
 public:
  template<typename FunctionType>
  interruptible_thread(FunctionType f) {
    std::promise<interrupt_flag*> p;  // 2
    internal_thread=std::thread(
        [f,&p]{
                p.set_value(&this_thread_interrupt_flag);
                try {                 
                  f();
                }
                catch(thread_interrupted const&) {
                } });
    flag = p.get_future().get();  // 5
  }
  void interrupt() {
    if(flag) {
      flag->set();  // 6
    }
  }
  void join() {
    internal_thread.join();
  }

  thread::id get_id() {
    return this_thread::get_id();
  }

  interrupt_flag* getInterruptFlag() {
    return &this_thread_interrupt_flag;
  }
};


void interruptible_wait(std::condition_variable& cv,
  std::unique_lock<std::mutex>& lk) {
  interruption_point();
  this_thread_interrupt_flag.set_condition_variable(cv);
  interruption_point();
  cv.wait_for(lk,std::chrono::milliseconds(1));
  interruption_point();
  this_thread_interrupt_flag.clear_condition_variable();
}

template<typename Predicate>
void interruptible_wait(std::condition_variable& cv,
                        std::unique_lock<std::mutex>& lk,
                        Predicate pred) {
  interruption_point();
  this_thread_interrupt_flag.set_condition_variable(cv);
  while(!this_thread_interrupt_flag.is_set() && !pred())
  {
    cv.wait_for(lk,std::chrono::milliseconds(1));
  }
  interruption_point();
  this_thread_interrupt_flag.clear_condition_variable();
}

void foo() {
  while (true) {
    try {
      interruption_point();
    }
    catch (const thread_interrupted& interrupt) {
      break;
    }
  }
}

std::mutex mut;
condition_variable cv;

void foo2() {
  while (true) {
      try {
        std::unique_lock<std::mutex> lgamma(mut);
        interruptible_wait(cv, lgamma);
      } catch (const thread_interrupted& interrupt) {
        break;
    }
  }
}

int main(int argc, char** argv) {

  ////////////////////////////1//////////////////////////////
  /*
  vector<interruptible_thread> threads;
  cout << thread::hardware_concurrency() - 1 << endl;
  for (auto i = 0; i < thread::hardware_concurrency() - 1; i++) {
    threads.push_back(interruptible_thread(foo));
  }

  for (auto& t : threads) {
    t.interrupt();
    t.join();
  }
  */

  ////////////////////////////2//////////////////////////////
  /*
  vector<interruptible_thread> threads;
  cout << thread::hardware_concurrency() - 1 << endl;
  for (auto i = 0; i < thread::hardware_concurrency() - 1; i++) {
    threads.push_back(interruptible_thread(foo2));
  }

  for (auto& t : threads) {
    t.interrupt();
    t.join();
  }
  */
  return 0;
}
#endif
/////////////////////////9-2////////////////////////
