/*明智而审慎的使用private*/

//如果类之间是private继承关系，那么编译器不会将derived class转换为base class 
//尽可能的使用复合，必要时才使用private继承（当protected成员或virtual函数牵扯进来的时候）

class Timer{
	public:
		explicit Timer(int tickFrequency);
		virtual void onTick();//定时器每滴答一次 此函数调用一次
		...
};

class Widget : private Timer{
	private:
		virtual void onTick();
};

//面对上面的代码 下面的代码可能更好一些

class Widget{
	
	private:
		class WidgetTimer: public Timer{
			public:
			  virtual void onTick()const;//定时器每滴答一次 此函数调用一次
			
		};
		WidgetTimer timer;
		...
};

//下面这种设计到空间最优化的时候 你要选择private继承

class Empty{//没有数据 不应该占内存

};
class HoldAnInt{//应该值需要一个int空间
	private:
		int x;
		Empty e;//应该是不需要内存
};//其实sizeof( HoldAnInt)>sizeof(int)  大多数编译器sizeof(Empty)==1

//选择private继承
class HoldAnInt:private Empty {//应该值需要一个int空间
	private:
		int x;
};//sizeof( HoldAnInt)==sizeof(int) 
