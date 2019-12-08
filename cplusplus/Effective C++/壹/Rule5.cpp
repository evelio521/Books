/*  了解c++ 默默编写并调用那些函数*/

 class Empty1{
};
 //与上面的类相同
 class Empty2{
 	
 	public:
 		Empty(){  }
 		Empty(const Empty& rhs){  }
 		~Empty(){  }
 		
 		Empty& operator=(const Empty&rhs){   }
};

template<class T>
  class NameObject1{
  	public:
  		NameObject(string name,const T& value):nameValue(name),objectValue(value)
  			{
  			}
  		
  	private:
  		string nameValue;//如今这个是 reference
  		 T objectValue;//如今这个为const
  			
  };
//下面的类 一些函数将不会被构造
  template<class T>
  class NameObject{
  	public:
  		NameObject(string name,const T& value):nameValue(name),objectValue(value)
  			{
  			}
  		
  	private:
  		string& nameValue;//如今这个是 reference
  		const T objectValue;//如今这个为const
  			
  };
  
  
 
  string newDog("Persephone");
  string oldDog("Satch");
  NameObject<int> p(newDog,2);
  NameObject<int> s(oldDog,36);
  
  p=s;//会产生编译错误 c++不容许reference指向不同的对象
  
  NameObject1<int> p1(newDog,2);
  NameObject1<int> s1(oldDog,36);
  
  p1=s1;//不会产生编译错误
 
  
  