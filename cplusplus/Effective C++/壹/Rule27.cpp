/*��������ת�Ͷ���*/

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
	doSomework(staitc_cast<Widget>(15));//����ʹ������д��
	
//����Ĵ���������
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
	//�����������̲� ���Ը�Ϊ
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