/*绝不重新定义继承而来的缺省参数*/

class Shape{
	
	public:
		enum ShapeColor{Red,Green,Blue};
		//所有的形状都必须提供一个函数 来绘制出自己
		virtual void draw(ShapeColor color=Red) const=0;
		...
};

class Rectangle:public Shape{
	public:
		//这里赋予不同的缺省参数 很糟糕！
		virtual void draw(ShapeColor color=Green) const;
	
};

class Circle:public Shape{
	public:
		//这里赋予不同的缺省参数 很糟糕！
		virtual void draw(ShapeColor color) const;
	
};

Shape* ps;                //静态类型 Shape*

Shape* pr=new Rectangle; //静态类型 Shape*

Shape* pc=new Circle;    //静态类型 Shape*

ps=pc;       //动态类型 Circle*
ps=pr;       //动态类型 Rectangle*

pr->draw();//Rectangle::draw(Shape::Red);

//也许我们可以改变这种设计结构

class Shape{
	
	public:
		enum ShapeColor{ Red,Blue,Green};
		void draw(ShapeColor color=Red) const
		{
			doDraw(color);
		}
	private:
		virtual void doDraw(ShapeColor color=Red) const=0;//真正的工作再次完成
		
};
class Rectangle:public Shape{
	public:
		//这里赋予不同的缺省参数 很糟糕！
		virtual void doDraw(ShapeColor color) const;//不需要指定缺省参数
	
};
