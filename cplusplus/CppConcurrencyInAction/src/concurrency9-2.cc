////////////////////////1/////////////////
#ifndef INTERRUPTIBLETHREAD_H
#define INTERRUPTIBLETHREAD_H

#include <thread>
#include <future>
#include <iostream>
#include <chrono>
#include <random>
#include <mutex>

using namespace std;

mutex mut;
condition_variable cv;
int tag;

class InterruptFlag
{
 public:
  inline void set() {
    lock_guard<mutex> guard(_mtx);
    _set = true;
  }

  inline bool is_set() {
    lock_guard<mutex> guard(_mtx);
    return _set;
  }

 private:
  mutex _mtx;
  bool _set;
};

thread_local InterruptFlag this_thread_interrupt_flag;

class InterruptibleThread
{
 public:
  default_random_engine e_;
  uniform_int_distribution<int> u_;
  thread _internal_thread;
  promise<decltype(this_thread::get_id())>* pro_ptr_;
  template<typename FunctionType>
InterruptibleThread(FunctionType f, promise<decltype(this_thread::get_id())>* pro_ptr, uniform_int_distribution<int> u): 
    pro_ptr_(pro_ptr), u_(u){
      promise<InterruptFlag*> p;
      _internal_thread = thread([f, &p, this]()
                                {
                                p.set_value(&this_thread_interrupt_flag);
                                f(*this);
                                });
      _interrupt_flag = p.get_future().get();
    }

  inline void interrupt()
  {
    if (_interrupt_flag != nullptr)
    {
      _interrupt_flag->set();
    }
  }

  void join() {
    _internal_thread.join();
  }

  thread::id get_id() {
    return this_thread::get_id();
  }

  InterruptFlag* getInterruptFlag() {
    return &this_thread_interrupt_flag;
  }

 private:
  InterruptFlag* _interrupt_flag;
};

#endif // INTERRUPTIBLETHREAD_H


#include "foo.h"
#include <functional>

using std::function;

struct thread_interrupted {};

void interruption_point()
{
  if (this_thread_interrupt_flag.is_set()) {
    throw thread_interrupted();
  }
}

void foo(InterruptibleThread& t)
{
  while (true) {
    int res = t.u_(t.e_);
    if (res == 7) {
      if (!tag) {
        unique_lock<mutex> lk(mut);
        t.pro_ptr_->set_value(this_thread::get_id());
        tag = 1;
      }
      break;
    }
    cout << this_thread::get_id() << " **** " << res << endl;
    try {
      interruption_point();
    }
    catch (const thread_interrupted& interrupt) {
      break;
    }
  }
  while (true) {
    try {
      interruption_point();
    }
    catch (const thread_interrupted& interrupt) {
      break;
    }
  }
}

int main()
{
  uniform_int_distribution<int> u(0, 9);
  promise<decltype(this_thread::get_id())> pro;
  future<decltype(this_thread::get_id())> fut = pro.get_future();
  vector<InterruptibleThread> threads;
  cout << thread::hardware_concurrency() - 1 << endl;
  for (auto i = 0; i < thread::hardware_concurrency() - 1; i++) {
    threads.push_back(InterruptibleThread(foo, &pro, u));
  }
  if (fut.wait_for(chrono::milliseconds(1000)) == future_status::ready) {
    cout << "get it!\n";
  }

  for (auto& t : threads) {
    t.interrupt();
    t.join();
  }
  getchar();
  return 0;
}


//////////////////////////////////
// 为std::condition_variable在interruptible_wait中使用超时

class interrupt_flag
{
  std::atomic<bool> flag;
  std::condition_variable* thread_cond;
  std::mutex set_clear_mutex;

public:
  interrupt_flag():
    thread_cond(0)
  {}

  void set()
  {
    flag.store(true,std::memory_order_relaxed);
    std::lock_guard<std::mutex> lk(set_clear_mutex);
    if(thread_cond)
    {
      thread_cond->notify_all();
    }
  }

  bool is_set() const
  {
    return flag.load(std::memory_order_relaxed);
  }

  void set_condition_variable(std::condition_variable& cv)
  {
    std::lock_guard<std::mutex> lk(set_clear_mutex);
    thread_cond=&cv;
  }

  void clear_condition_variable()
  {
    std::lock_guard<std::mutex> lk(set_clear_mutex);
    thread_cond=0;
  }

  struct clear_cv_on_destruct
  {
    ~clear_cv_on_destruct()
    {
      this_thread_interrupt_flag.clear_condition_variable();
    }
  };
};

void interruptible_wait(std::condition_variable& cv,
  std::unique_lock<std::mutex>& lk)
{
  interruption_point();
  this_thread_interrupt_flag.set_condition_variable(cv);
  interrupt_flag::clear_cv_on_destruct guard;
  interruption_point();
  cv.wait_for(lk,std::chrono::milliseconds(1));
  interruption_point();
}

template<typename Predicate>
void interruptible_wait(std::condition_variable& cv,
                        std::unique_lock<std::mutex>& lk,
                        Predicate pred)
{
  interruption_point();
  this_thread_interrupt_flag.set_condition_variable(cv);
  interrupt_flag::clear_cv_on_destruct guard;
  while(!this_thread_interrupt_flag.is_set() && !pred())
  {
    cv.wait_for(lk,std::chrono::milliseconds(1));
  }
  interruption_point();
}


// 为std::condition_variable_any设计的interruptible_wait

class interrupt_flag
{
  std::atomic<bool> flag;
  std::condition_variable* thread_cond;
  std::condition_variable_any* thread_cond_any;
  std::mutex set_clear_mutex;

public:
  interrupt_flag(): 
    thread_cond(0),thread_cond_any(0)
  {}

  void set()
  {
    flag.store(true,std::memory_order_relaxed);
    std::lock_guard<std::mutex> lk(set_clear_mutex);
    if(thread_cond)
    {
      thread_cond->notify_all();
    }
    else if(thread_cond_any)
    {
      thread_cond_any->notify_all();
    }
  }

  template<typename Lockable>
  void wait(std::condition_variable_any& cv,Lockable& lk)
  {
    struct custom_lock
    {
      interrupt_flag* self;
      Lockable& lk;

      custom_lock(interrupt_flag* self_,
                  std::condition_variable_any& cond,
                  Lockable& lk_):
        self(self_),lk(lk_)
      {
        self->set_clear_mutex.lock();  // 1
        self->thread_cond_any=&cond;  // 2
      }

      void unlock()  // 3
      {
        lk.unlock();
        self->set_clear_mutex.unlock();
      }

      void lock()
      {
        std::lock(self->set_clear_mutex,lk);  // 4
      }

      ~custom_lock()
      {
        self->thread_cond_any=0;  // 5
        self->set_clear_mutex.unlock();
      }
    };
    custom_lock cl(this,cv,lk);
    interruption_point();
    cv.wait(cl);
    interruption_point();
  }
  // rest as before
};

template<typename Lockable>
void interruptible_wait(std::condition_variable_any& cv,
                        Lockable& lk)
{
  this_thread_interrupt_flag.wait(cv,lk);
}

template<typename T>
void interruptible_wait(std::future<T>& uf)
{
  while(!this_thread_interrupt_flag.is_set())
  {
    if(uf.wait_for(lk,std::chrono::milliseconds(1)==
       std::future_status::ready)
      break;
  }
  interruption_point();
}
