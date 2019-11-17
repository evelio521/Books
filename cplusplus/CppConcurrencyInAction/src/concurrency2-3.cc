#include <thread>
#include <iostream>
#include <vector>
void some_function() {

}

void some_other_function () {

}


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
