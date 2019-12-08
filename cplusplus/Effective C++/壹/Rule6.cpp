/*  如不想让编译器自动生成函数 ，就该明确拒绝*/

 //第一种做法
 class HomeForSale{
 	
 	public:
 	  
 	private:
 		HomeForSale(const HomeForSale&);//声明为private 而且不定义
 		HomeForSale& operator=(const HomeforSale&);//声明为private 而且不定义
};

 //第二种做法
 class Uncopyable{
 	protected:
 		Uncopyable(){}
 		~Uncopyable(){}
 	private:
 		Uncopyable(const Uncopyable&);
 		Uncopyable& operator=(const Uncopyable&);
 	
};
//为了阻止HomeForSale  我们只需要从Uncopyable继承

 class HomeForSale1:private Uncopyable{
                                    //不用写任何代码	
};

