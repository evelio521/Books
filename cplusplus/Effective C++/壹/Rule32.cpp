/*ȷ�����public�̳���Ĥ��is-a��ϵ*/

//��ʵ���ܷ�ӳ��is-a��ϵ����첻���
class Bird{
	public:
		virtual void fly();//����Է�
		...
};

class Penguin:public Bird{//���Ҳ��һ����
	
	....
};

//Ҳ���������Ƹ�����һ��
class Bird{
			...
};

class FlyingBird:public Bird{//���Ҳ��һ����
	
	public:
		virtual void fly();
	....
};

class Penguin:public Bird{//���Ҳ��һ����
	
	
	....
};

//rectangle �� square�����

class Rectangle{
	
	public:
		virtual void setHeight(int newHeight);
		virtual void setWidth(int newHeight);
		virtual int height() const;//���ص�ǰֵ
		virtual int width() const;
};
void makeBigger(Rectangle& r)
{
	int oldHeight=r.height();
	r.setWidth(r.width()+10);//��ȼ�10
	assert(r.height()==oldHeight);//�ж�r�ĸ߶��Ƿ�δ���ı�
}

class Square:public Rectangle{ ...};
	
Square s;
...
assert(s.width()==s.height());//������һ��Ϊ��
makeBigger(s);//���ڼ̳� ����һ������
assert(s.width()==s.height());//������һ��Ϊ��,�ź����ǲ���Ϊ��
