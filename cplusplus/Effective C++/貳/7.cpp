class TimeKeeper{
 	
 	public:
 		TimeKeeper(){}
 		virtual ~TimeKeeper(){}
 		
};
class Derived:public TimeKeeper{
	
	public:
		Derived(){}
		~Derived(){};
};
//任何一个class只要确定带有virtual函数，几乎确定应该有一个virtual析构函数

/*
 * 如果一个class不含virtual函数，说明他不想被用作一个基类。当不想被用作基类时候
 * 定义virtual析构函数往往是个坏主意。
 * 
 *
 */
 
 //有时候我们想定义一个抽象类 但是我们有没有virtual函数的时候 可以将析构函数设为纯虚韩式
class AWOV{
public:
	virtual ~AWOV()=0;

};
AWOV::~AWOV()
{
}
 //心得：只有当class内至少含有一个virtual函数时，才声明virtual析构函数
 
 int main(){
 	
 TimeKeeper *tk=new Derived;
 
 delete tk;	
}