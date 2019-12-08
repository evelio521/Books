/*在operator= 处理自我赋值*/

//可能出现自我赋值的情况
 a[i]=a[j];//i==j
 *px=*py;//px,py指向同一个对象
 
 class Base {};
 class Derived:public Base {};
 void function(const Base& rb,Derived * pd);//rb和*pd是同一对象
 
 //给出自我赋值的情况和解决办法
 
 class Bitmap{
 };
 class Widget{
 
 ...
 private:
 	Bitmap	*pb;
}
  
//一份不安全的operator=实现版本
 Widget& Widget::operator=(const Widget& rhs)
 	{
 		delete pb;
 		pb=new Bitmap(*rhs.pb);
 		return *this;
 	}
//解决方法
	Widget& Widget::operator=(const Widget& rhs)
 	{
 		if(this==&rhs)
 			return *this;
 		delete pb;
 		pb=new Bitmap(*rhs.pb);
 		return *this;
 	}
 //解决方法
	Widget& Widget::operator=(const Widget& rhs)
 	{
 		Bitmap *pOrig=pb;
 		pb=new Bitmap(*rhs.pb);
 		delete pOrig;
 		return *this;
 	}
 