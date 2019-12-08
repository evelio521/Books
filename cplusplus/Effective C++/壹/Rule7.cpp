/*为多态基类声明virtual析构函数*/

//为类声明virtual析构函数时可以减少"局部销毁"问题
 class TimeKeeper{
 	
 	public:
 		TimeKeeper(){}
 		virtual ~TimeKeeper(){}
 		
};
clasee Derived:public TimeKeeper{
	
	public:
		Drived(){}
		~Derived(){};
};
//任何一个class只要确定带有virtual函数，几乎确定应该有一个virtual析构函数

/*
 * 如果一个class不含virtual函数，说明他不想被用作一个基类。当不想被用作基类时候
 * 定义virtual析构函数往往是个坏主意。
 * 
 *
 */
 
 
 //心得：只有当class内至少含有一个virtual函数时，才声明virtual析构函数
 
 
 	TimeKeeper *tk=new Derived();
 	
 	delete tk;//如果TimeKeeper没有virtual  那么Derived的部分可能没有被销毁
 	
