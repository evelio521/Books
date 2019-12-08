/*运用成员函数模版接受所有兼容类型*/

class Top{
};
class Middle:public Top{
	
};
class Bottom:public Middle{
	
};
Top* pt1=new Middle;//Middle*转到Top*
Top* pt2=new Bottom;//Bottom*转到Top*

const Top* pct2=pt1;//Top*转到const Top*

//
 template <typename T>
 class SmartPtr{//智能指针
 	public:
 		explicit SmartPtr(T* realPtr);
};

 SmartPtr<Top> pt1=SmartPtr<Middle>(new Middle);//SmartPtr<Top> 转换为SmartPtr<Middle>
 SmartPtr<Top> pt2=SmartPtr<Bottom>(new Bottom);//SmartPtr<Top> 转换为SmartPtr<Bottom>
 SmartPtr<const Top> pt2=pt1;//SmartPtr<Top> 转换为 SmartPtr<const Top>
 
 //由于上面的函数 构造函数无法完成 我们采用一下做法
  template<typename T>
  class SmartPtr{
  	public:
  		template<typename U>
  		SamrtPtr(const SmartPtr<U>& other);
  		...
  };//无法阻止SmartPtr<double>到SmartPtr<int>的转换
  
  //针对上面的问题做出改变
  template<typename T>
  class SmartPtr{
  	public:
  		template<typename U>
  		SamrtPtr(const SmartPtr<U>& other):heldPtr(other.get())
  			{
  			}
  		T* get() const {return heldPtr;}
  	private:
  		T* heldPtr;
  		...
  };
 