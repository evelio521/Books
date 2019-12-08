/*尽量少做转型动作*/

const_cast<T>(expression);
dynamic_cast<T>(expression);
reinterpret_cast<T>(expression);
static_cast<T>(expression);

class Widget{
	public:
		explicit Widget(int size);
		...
};

	void doSomework(const Widget& w);
	doSomework(Widget(15));
	doSomework(staitc_cast<Widget>(15));//尽量使用这种写法
	
//下面的代码有问题
 class Window{
 	public:
 		virtual void onReszie()
 		{
 			...
 		}
};
 class SpecialWindow:public Window{
 	public:
 		virtual void onResize()
 		{
 			static_cast<Window>(*this).onResize();
 			
 			...
 			...
 			...
 		}
};
	//具体的问题见教材 可以改为
	class Window{
 	public:
 		virtual void onReszie()
 		{
 			...
 		}
};
 class SpecialWindow:public Window{
 	public:
 		virtual void onResize()
 		{
 			Window::onResize();
 			
 			...
 			...
 			...
 		}
};