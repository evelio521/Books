#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

void f() {
  std::cout << std::this_thread::get_id() << std::endl;
}
int main() {
  std::thread t(f);
  t.join();
  std::cout << std::this_thread::get_id() << std::endl;
  std::cout <<"End\n";
}
