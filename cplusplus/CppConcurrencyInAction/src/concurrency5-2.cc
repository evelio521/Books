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

///////////////1/////////////////
//表5.1 标准原子类型的备选名和
//与其相关的std::atomic<>特化类

atomic_bool       std::atomic<bool>
atomic_char       std::atomic<char>
atomic_schar      std::atomic<signed char>
atomic_uchar      std::atomic<unsigned char>
atomic_int        std::atomic<int>
atomic_uint       std::atomic<unsigned>
atomic_short      std::atomic<short>
atomic_ushort     std::atomic<unsigned short>
atomic_long       std::atomic<long>
atomic_ulong      std::atomic<unsigned long>
atomic_llong      std::atomic<long long>
atomic_ullong     std::atomic<unsigned long long>
atomic_char16_t   std::atomic<char16_t>
atomic_char32_t   std::atomic<char32_t>
atomic_wchar_t    std::atomic<wchar_t>

//表5.2 标准原子类型定义(typedefs)和
//对应的内置类型定义(typedefs)

atomic_int_least8_t      int_least8_t
atomic_uint_least8_t     uint_least8_t
atomic_int_least16_t     int_least16_t
atomic_uint_least16_t    uint_least16_t
atomic_int_least32_t     int_least32_t
atomic_uint_least32_t    uint_least32_t
atomic_int_least64_t     int_least64_t
atomic_uint_least64_t    uint_least64_t
atomic_int_fast8_t       int_fast8_t
atomic_uint_fast8_t      uint_fast8_t
atomic_int_fast16_t      int_fast16_t
atomic_uint_fast16_t     uint_fast16_t
atomic_int_fast32_t      int_fast32_t
atomic_uint_fast32_t     uint_fast32_t
atomic_int_fast64_t      int_fast64_t
atomic_uint_fast64_t     uint_fast64_t
atomic_intptr_t          intptr_t
atomic_uintptr_t         uintptr_t
atomic_size_t            size_t
atomic_ptrdiff_t         ptrdiff_t
atomic_intmax_t          intmax_t
atomic_uintmax_t         uintmax_t

///////////////2/////////////////
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
void count1m(int id) {
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
    threads.push_back(std::thread(count1m, i));
  ready = true;

  for (auto & th:threads)
    th.join();
  return 0;
}

////////////////////3///////////////////
std::atomic<bool> b;
bool x = b.load(std::memory_order_acquire);
b.store(true);
x = b.exchange(false, std::memory_order_acq_rel);

bool expected = false;
extern atomic<bool> b;
while (!b.compare_exchange_weak(expected, true));

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

/////////////////////4/////////////////////////
class Foo {};
Foo some_array[5];

std::atomic<Foo*> p(some_array);

Foo* x = p.fetch_add(2); // 返回第零个值
assert(x == some_array);
assert(p.load() == &some_array[2]);

x = (p-=1);
assert(x == &some_array[1]);
assert(p.load() == &some_array[1]);

////////////////////////7////////////////////////
std::shared_ptr<my_data> p;
void process_global_data() {
  std::shared_ptr<my_data> local = std::atomic_load(&p);
  process_data(local);
}

void update_global_data() {
  std::shared_ptr<my_data> local(new my_data);
  std::atomic_store(&p, local);
}

