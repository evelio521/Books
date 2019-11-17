#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <list>
#include <mutex>
#include <condition_variable>
#include <future>

///////////////2/////////////////
std::chrono::milliseconds ms(54802);
std::chrono::senonds s =
    std::chrono::duration_cast<std::chrono::seconds>(ms);

std::future<int> f = std::async(some_task);
if (f.wait_for(std::chrono::milliseconds(35)) == 
    std::future_status::ready) {
  do_something_with(f.get());
}


///////////////3/////////////////
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
  std::cout <<"End\n";
}
