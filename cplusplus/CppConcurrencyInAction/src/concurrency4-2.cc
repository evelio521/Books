#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <list>
#include <mutex>
#include <condition_variable>
#include <future>

///////////////1/////////////////
int find_the_answer_to_ltuaw () { return 0;}
void do_other_stuff() {}
void test_future() {
  std::future<int> the_answer = 
      std::async(find_the_answer_to_ltuaw);
  do_other_stuff();
  std::cout<<the_answer.get() << std::endl;
}

struct X {
  void foo(int,std::string const&);
  std::string bar(std::string const&);
};


X x;
auto f1=std::async(&X::foo,&x,42,"hello");
auto f2=std::async(&X::bar,x,"goodbye");

struct Y {
  double operator()(double);
};
Y y;
auto f3=std::async(Y(),3.141);
auto f4=std::async(std::ref(y),2.718);
X baz(X&);
auto f6=std::async(baz,std::ref(x));
class move_only {
 public:
  move_only();
  move_only(move_only&&);
  move_only(move_only const&) = delete;
  move_only& operator=(move_only&&);
  move_only& operator=(move_only const&) = delete;
  void operator()();
};
auto f5=std::async(move_only());

// defered 表明在wait或get函数调用才执行
// async   函数调用立即执行
auto f7 = std::async(std::launch::defered, bax, std::ref(x));
auto f8 = std::async(std::launch::defered | std::launch::async , bax, std::ref(x));

f7.wait();

///////////////2/////////////////
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

std::thread gui_bg_thread(gui_thread);

template<typename Func>
std::future<void> post_task_for_gui_thread(Func f) {
  std::packaged_task<void()> task(f);
  std::future<void> res=task.get_future();
  std::lock_guard<std::mutex> lk(m);
  tasks.push_back(std::move(task));
  return res;
}

///////////////3/////////////////
/*
 *在线程1中创建一个std::promise对象

 std::promise<int> promiseObj;

 目前为止，该promise对象没有任何管理的值，但它承诺肯定会有人对其进行赋值，一旦被赋值，就可以通过其管理的std::future对象来获取该值。

 但是，假设线程1创建了该promise对象并将其传给线程2，那么线程1怎样知道线程2什么时候会对promise对象进行赋值呢？

 答案是使用std::future对象

 每个std::promise对象都有个对应的std::future对象，其他人可以通过它来获取promise设置的值。

 所以，线程1将会创建std::promise对象，然后在将其传递给线程2之前从它那里获取std::future对象。

 std::future<int> futureObj = promiseObj.get_future();

 现在，线程1将promiseObj传递给线程2.

 那么线程1将会获取到线程2通过std::future的get函数设置在std::promise中的值，

 int val = futureObj.get();

 但是如果线程2还没有对该值进行设置，那么这个调用将会阻塞，直到线程2在promise对象中对该值进行设置。

 promiseObj.set_value(45);
 */
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

///////////////4/////////////////
extern std::promise<double> some_promise;
try {
  some_promise.set_value(value);
}
catch (...) {
  some_promise.set_exception(std::copy_exception(std::logic_error("foo")));
}

///////////////5/////////////////
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
  std::cout <<"End\n";
}
