#include <thread>
#include <iostream>

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
    std::cout <<"func\n";
    i++;
  }
};

void f() {
  int some_local_state = 0;
  func my_fun(some_local_state);
  std::thread t(my_fun);
  thread_guard g(t);
}

int main() {
  background_task bf;
  std::thread my_thread(bf);
  my_thread.join();
  std::thread my_thread2((background_task()));
  my_thread2.join();
  std::thread my_thread3{background_task()};
  my_thread3.detach();
  assert(!my_thread3.joinable());

  f();
  std::cout <<"End\n";
}
