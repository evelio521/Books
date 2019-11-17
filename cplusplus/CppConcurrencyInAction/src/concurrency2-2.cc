#include <thread>
#include <iostream>
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
  std::cout <<"End\n";
}
