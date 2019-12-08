/*确定你的public继承塑膜出is-a关系*/

//其实不能反映出is-a关系，企鹅不会飞
class Bird{
	public:
		virtual void fly();//鸟可以飞
		...
};

class Penguin:public Bird{//企鹅也是一种鸟
	
	....
};

//也许下面的设计更合理一点
class Bird{
			...
};

class FlyingBird:public Bird{//企鹅也是一种鸟
	
	public:
		virtual void fly();
	....
};

class Penguin:public Bird{//企鹅也是一种鸟
	
	
	....
};

//rectangle 和 square的设计

class Rectangle{
	
	public:
		virtual void setHeight(int newHeight);
		virtual void setWidth(int newHeight);
		virtual int height() const;//返回当前值
		virtual int width() const;
};
void makeBigger(Rectangle& r)
{
	int oldHeight=r.height();
	r.setWidth(r.width()+10);//宽度加10
	assert(r.height()==oldHeight);//判断r的高度是否未曾改变
}

class Square:public Rectangle{ ...};
	
Square s;
...
assert(s.width()==s.height());//正方形一定为真
makeBigger(s);//由于继承 是是一个矩形
assert(s.width()==s.height());//正方形一定为真,遗憾的是并不为真
