//is-a has-a
//其实不能反映出is-a关系，企鹅不会飞

#include<iostream>
#include<assert.h>
class Bird{
	
};



//也许下面的设计更合理一点,可以区分会飞和不会飞的鸟


class FlyingBird:public Bird{//企鹅也是一种鸟
	
	public:
		virtual void fly()
		{
			std::cout<<"鸟可以飞翔"<<std::endl;
		};//鸟可以飞
	
};

class Penguin:public Bird{//企鹅也是一种鸟，但是不会飞
	
	
	
};

//rectangle 和 square的设计

class Rectangle{
	
	public:
		virtual void setHeight(int newHeight){
			Height=newHeight;
		}
		virtual void setWidth(int newWidth){
			Width=newWidth;
		}
		virtual int height() const//返回当前值
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
	r.setWidth(r.width()+10);//宽度加10
	assert(r.height()==oldHeight);//判断r的高度是否未曾改变
}

class Square:public Rectangle{ 
	
};
int main(){
	
Square s;

assert(s.width()==s.height());//正方形一定为真
makeBigger(s);//由于继承 是是一个矩形
assert(s.width()==s.height());//正方形一定为真,遗憾的是并不为真
}