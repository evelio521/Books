//is-a has-a
//��ʵ���ܷ�ӳ��is-a��ϵ����첻���

#include<iostream>
#include<assert.h>
class Bird{
	
};



//Ҳ���������Ƹ�����һ��,�������ֻ�ɺͲ���ɵ���


class FlyingBird:public Bird{//���Ҳ��һ����
	
	public:
		virtual void fly()
		{
			std::cout<<"����Է���"<<std::endl;
		};//����Է�
	
};

class Penguin:public Bird{//���Ҳ��һ���񣬵��ǲ����
	
	
	
};

//rectangle �� square�����

class Rectangle{
	
	public:
		virtual void setHeight(int newHeight){
			Height=newHeight;
		}
		virtual void setWidth(int newWidth){
			Width=newWidth;
		}
		virtual int height() const//���ص�ǰֵ
		{
			return Height;
		}
		virtual int width() const
		{
			return Width;
		}
	private:
		int Height;
		int Width;
		
};
void makeBigger(Rectangle& r)
{
	int oldHeight=r.height();
	r.setWidth(r.width()+10);//��ȼ�10
	assert(r.height()==oldHeight);//�ж�r�ĸ߶��Ƿ�δ���ı�
}

class Square:public Rectangle{ 
	
};
int main(){
	
Square s;

assert(s.width()==s.height());//������һ��Ϊ��
makeBigger(s);//���ڼ̳� ����һ������
assert(s.width()==s.height());//������һ��Ϊ��,�ź����ǲ���Ϊ��
}