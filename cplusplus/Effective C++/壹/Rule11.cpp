/*��operator= �������Ҹ�ֵ*/

//���ܳ������Ҹ�ֵ�����
 a[i]=a[j];//i==j
 *px=*py;//px,pyָ��ͬһ������
 
 class Base {};
 class Derived:public Base {};
 void function(const Base& rb,Derived * pd);//rb��*pd��ͬһ����
 
 //�������Ҹ�ֵ������ͽ���취
 
 class Bitmap{
 };
 class Widget{
 
 ...
 private:
 	Bitmap	*pb;
}
  
//һ�ݲ���ȫ��operator=ʵ�ְ汾
 Widget& Widget::operator=(const Widget& rhs)
 	{
 		delete pb;
 		pb=new Bitmap(*rhs.pb);
 		return *this;
 	}
//�������
	Widget& Widget::operator=(const Widget& rhs)
 	{
 		if(this==&rhs)
 			return *this;
 		delete pb;
 		pb=new Bitmap(*rhs.pb);
 		return *this;
 	}
 //�������
	Widget& Widget::operator=(const Widget& rhs)
 	{
 		Bitmap *pOrig=pb;
 		pb=new Bitmap(*rhs.pb);
 		delete pOrig;
 		return *this;
 	}
 