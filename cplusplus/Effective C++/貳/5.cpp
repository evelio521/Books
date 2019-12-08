#include<iostream>

class Empty{
	public:
		//下面的函数如果不自己构造 那么编译器会主动构造
		//默认构造函数
		Empty(){
		}
		//copy 构造函数
		Empty(const Empty& rhs){
		
		}
		//析构函数
		~Empty(){
		}
		//copy assignment操作符
		Empty& operator=(const Empty& rhs){
			return *this;
		}
};
template<typename T>
class NameObject{
	
	public:
	NameObject(std::string name,const T& value):nameValue(name),ObjectValue(value){}
  private:
  	//如今这个类不在默认的提供operator= 因为C++不允许reference指向不同的对象 更改const 也不合法
  	//面对这样的类 你必须自己定义copy assignment 
  	std::string& nameValue;
  	const T ObjectValue;
};
int main()
{
	Empty e1;//默认构造函数
					//析构函数
	Empty e2(e1);//copy构造函数
	e2=e1;//copy assignment 操作符
	
	std::string newDog("perry");
	std::string oldDog("stach");
	
	NameObject<int> p(newDog,2);
	NameObject<int> s(oldDog,36);
	
	//p=s; 错误的用法
	
	
	
	return 0;
}