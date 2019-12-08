	#include<iostream>

	class Widget {
	public:
		Widget()
		{
		}
		virtual int temp()
		{
			return 1;
		}
		
	 };
	class SpecialWidget: public Widget { 
	public:
		 int temp()
		{
			return 2;
		}
		
	 };
	void update(SpecialWidget *psw)
	{
	}
	void updateViaRef(SpecialWidget& rsw)
	{
	}
	 int doSomething()
	 {
		 return 3;
	 }
int main()
{
	//static_cast����ӵ�о�ʽcת���Ļ������ܺ�����
	//���ǲ���ȥ�����ʽ�ĳ�����
	int firstNumber=5, secondNumber=6;
	double result = static_cast<double>(firstNumber)/secondNumber;
	
	//const_cast��������ת�������ʽ��const��volatileness����
	
		 SpecialWidget sw; // sw ��һ����const ����
		 const SpecialWidget& csw = sw; // csw ��sw��һ������
		 																// ����һ��const ����
		 
		 	//update(&csw);
		  // ����!���ܴ���һ��const SpecialWidget* ����
		 	// ��һ������SpecialWidget*���ͱ����ĺ���
		 	
		 	
		 	
		 update(const_cast<SpecialWidget*>(&csw));
					// ��ȷ��csw��const����ʾ��ת������
					// csw��sw��������ֵ��update
				//�������ܱ����£�
		 update((SpecialWidget*)&csw);
				// ͬ�ϣ�������һ������ʶ��
				//��C��������ת��
		/*
		 Widget *pw = new SpecialWidget;
		 update(pw); // ����pw��������Widget*������
		 							// update�����������SpecialWidget*����
		 update(const_cast<SpecialWidget*>(pw));
									// ����const_cast���ܱ�����Ӱ��
                  // constness or volatileness�ĵط��ϡ�,
                  // ����������̳������������ת����
    */
    
    

	    Widget *pw;
		
		update(dynamic_cast<SpecialWidget*>(pw));
					// ��ȷ�����ݸ�update����һ��ָ��
					// ��ָ���������ΪSpecialWidget��pw��ָ��
					// ���pwȷʵָ��һ������,
					// ���򴫵ݹ�ȥ�Ľ�ʹ��ָ�롣
		updateViaRef(dynamic_cast<SpecialWidget&>(*pw));
					//��ȷ�� ���ݸ�updateViaRef����
					// SpecialWidget pw ָ�룬���pw
					// ȷʵָ����ĳ������
					// �����׳��쳣
			//dynamic_casts�ڰ���������̳в�����������Ƶġ������ܱ�����ȱ���麯���������ϣ��μ�����M24����
			//Ҳ����������ת����constness��
	
	
	//reinterpret_casts������ͨ����;�����ں���ָ������֮�����ת��
	typedef void (*FuncPtr)(); // FuncPtr is һ��ָ����
																	// ��ָ�룬�ú���û�в���
																	// ����ֵ����Ϊvoid
	FuncPtr funcPtrArray[10]; // funcPtrArray ��һ��������
																// 10��FuncPtrsָ�������
		 
		  
		  //funcPtrArray[0] = &doSomething; // �������Ͳ�ƥ��

      //reinterpret_cast����������ʹ����������ķ���ȥ�������ǣ�
      funcPtrArray[0] = reinterpret_cast<FuncPtr>(&doSomething);// this compiles
	return 0;
}