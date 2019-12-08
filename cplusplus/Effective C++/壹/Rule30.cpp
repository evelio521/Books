/*透彻了解inlining的里里外外*/

class Person{
	
	public:
		...
		int age() const//一个隐喻的inline申请
		{
			return theAge;
		}
		...
	private:
		int theAge;
};
//通常声明inline函数的做法是在函数前加上inline
 template <typename T>
 inline const T& std::max(const T&a,const T&b)
 	{
 		return a<b?b:a;
 		}
 		
 	//	Inline函数一般放在头文件内
 	//	template一般放在头文件内
 	//
 	//inline函数的调用可能被inlined也可能不被inlined
 	
 	//编译器通常不对"通过函数指针而进行的调用"实施inlining
 	
 	inline void f() {...}
 	void (*pf)()=f;
 	
 	...
 	f();//这个调用被inlined
 	pf();//这个调用或许不被inlined
 	
 	//构造函数和析构函数不应该是inline