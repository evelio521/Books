/////////////////////1//////////////////
#include <vector>
#include <atomic>
#include <iostream>
#include <chrono>
#include <thread>

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

////////////////////////2/////////////////////
void foo(int a,int b) {
  std::cout<<a<<”,”<<b<<std::endl;
}

int get_num() {
  static int i=0;
  return ++i;
}

int main() {
  // 输出1，2  或者 2, 1
  foo(get_num(),get_num());
}

////////////////////////3/////////////////////
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
  if(y.load(std::memory_order_seq_cst))
    ++z;
}

void read_y_then_x() {
  while(!y.load(std::memory_order_seq_cst));
  if(x.load(std::memory_order_seq_cst))
    ++z;
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
  assert(z.load()!=0); // 可能发生
}

// 复杂的例子
#include <thread>
#include <atomic>
#include <iostream>

std::atomic<int> x(0),y(0),z(0);
std::atomic<bool> go(false);
unsigned const loop_count=10;

struct read_values {
  int x,y,z;
};

read_values values1[loop_count];
read_values values2[loop_count];
read_values values3[loop_count];
read_values values4[loop_count];
read_values values5[loop_count];

void increment(std::atomic<int>* var_to_inc,
               read_values* values) {
  while(!go)
    std::this_thread::yield();
  for(unsigned i=0;i<loop_count;++i) {
    values[i].x=x.load(std::memory_order_relaxed);
    values[i].y=y.load(std::memory_order_relaxed);
    values[i].z=z.load(std::memory_order_relaxed);
    var_to_inc->store(i+1,std::memory_order_relaxed);
    std::this_thread::yield();
  }
}

void read_vals(read_values* values) {
  while(!go)
    std::this_thread::yield();
  for(unsigned i=0;i<loop_count;++i) {
    values[i].x=x.load(std::memory_order_relaxed);
    values[i].y=y.load(std::memory_order_relaxed);
    values[i].z=z.load(std::memory_order_relaxed);
    std::this_thread::yield();
  }
}

void print(read_values* v) {
  for(unsigned i=0;i<loop_count;++i) {
    if(i)
      std::cout<<",";
    std::cout<<"("<<v[i].x<<","<<v[i].y<<","<<v[i].z<<")";
  }
  std::cout<<std::endl;
}

int main() {
  std::thread t1(increment,&x,values1);
  std::thread t2(increment,&y,values2);
  std::thread t3(increment,&z,values3);
  std::thread t4(read_vals,values4);
  std::thread t5(read_vals,values5);
  go=true;
  t5.join();
  t4.join();
  t3.join();
  t2.join();
  t1.join();
  print(values1);
  print(values2);
  print(values3);
  print(values4);
  print(values5);
}

//
// memory_order_consume
// memory_order_acquire
// memory_order_release
// memory_order_acq_rel
//

#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x,y;
std::atomic<int> z;

void write_x() {
  x.store(true,std::memory_order_release);
}

void write_y() {
  y.store(true,std::memory_order_release);
}

void read_x_then_y() {
  while(!x.load(std::memory_order_acquire));
  if(y.load(std::memory_order_acquire))
    ++z;
}

void read_y_then_x() {
  while(!y.load(std::memory_order_acquire));
  if(x.load(std::memory_order_acquire))
    ++z;
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
  assert(z.load()!=0); // 有可能触发
}

// 对上面程序的修正
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
  assert(z.load()!=0); // 不会触发
}

// 使用获取和释放顺序进行同步船体
#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<int> data[5];
std::atomic<bool> sync1(false),sync2(false);

void thread_1() {
  data[0].store(42,std::memory_order_relaxed);
  data[1].store(97,std::memory_order_relaxed);
  data[2].store(17,std::memory_order_relaxed);
  data[3].store(-141,std::memory_order_relaxed);
  data[4].store(2003,std::memory_order_relaxed);
  sync1.store(true,std::memory_order_release);
}

void thread_2() {
  while(!sync1.load(std::memory_order_acquire));
  sync2.store(std::memory_order_release);
}

void thread_3() {
  while(!sync2.load(std::memory_order_acquire));
  assert(data[0].load(std::memory_order_relaxed)==42);
  assert(data[1].load(std::memory_order_relaxed)==97);
  assert(data[2].load(std::memory_order_relaxed)==17);
  assert(data[3].load(std::memory_order_relaxed)==-141);
  assert(data[4].load(std::memory_order_relaxed)==2003);
}

int main() {
  std::thread t1(thread_1);
  std::thread t2(thread_2);
  std::thread t3(thread_3);
  t1.join();
  t2.join();
  t3.join();
}

// memory_order_consume
#include <string>
#include <thread>
#include <atomic>
#include <assert.h>
struct X {
  int i;
  std::string s;
};

std::atomic<X*> p;
std::atomic<int> a;

void create_x() {
  X* x=new X;
  x->i=42;
  x->s="hello";
  a.store(99,std::memory_order_relaxed);
  p.store(x,std::memory_order_release);
}

void use_x() {
  X* x;
  while(!(x=p.load(std::memory_order_consume)))
    std::this_thread::sleep_for(
        std::chrono::microseconds(1));
  assert(x->i==42);
  assert(x->s=="hello");
  assert(a.load(std::memory_order_relaxed)==99);
}
int main() {
  std::thread t1(create_x);
  std::thread t2(use_x);
  t1.join();
  t2.join();
}

// kill_dependency
// 有时，你不想为携带依赖增加其他的开销。
// 你想让编译器在寄存器中缓存这些值，
// 以及优化重排序操作代码，而不是对这些
// 依赖大惊小怪。这种情况下，你可以使用
// std::kill_dependecy()来显式打破依赖链。
// std::kill_dependency()是一个简单的函数模板，
// 其会复制提供的参数给返回值，但是依旧会打破依赖链。
int global_data[] = {....};
std::atomic<int> index;

void f() {
  int i = index.load(std::memory_order_consume);
  do_sth(global_data[std::kill_dependency(i)]);
}

/////////////////////////4////////////////////////
#include <atomic>
#include <thread>

std::vector<int> queue_data;
std::atomic<int> count;

void populate_queue() {
  unsigned const number_of_items=20;
  queue_data.clear();
  for(unsigned i=0;i<number_of_items;++i) {
    queue_data.push_back(i);
  }

  count.store(number_of_items, std::memory_order_release);
}

void consume_queue_items() {
  while(true) {
    int item_index;
    if((item_index = count.fetch_sub(
                1,std::memory_order_acquire))<=0) {
      wait_for_more_items();
      continue;
    }
    process(queue_data[item_index-1]);
  }
}

int main() {
  std::thread a(populate_queue);
  std::thread b(consume_queue_items);
  std::thread c(consume_queue_items);
  a.join();
  b.join();
  c.join();
}


////////////////////5/////////////////////
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

/////////////////////////6////////////////////
//使用非原子操作执行序列
#include <atomic>
#include <thread>
#include <assert.h>

bool x=false;  // x现在是一个非原子变量
std::atomic<bool> y;
std::atomic<int> z;

void write_x_then_y() {
  x=true;  // 1 在栅栏前存储x
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
