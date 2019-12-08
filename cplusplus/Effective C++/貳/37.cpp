//绝不重新定义继承而来的缺省参数

class Shape{
	
	public:
		enum ShapeColor{Red,Green,Blue};
		//所有的形状都必须提供一个函数 来绘制出自己
		virtual void draw(ShapeColor color=Red) const=0;
		
};

class Rectangle:public Shape{
	public:
		//这里赋予不同的缺省参数 很糟糕！
		virtual void draw(ShapeColor color=Green) const
		
		{
		}
	
};

class Circle:public Shape{
	public:
		//这里赋予不同的缺省参数 很糟糕！
		virtual void draw(ShapeColor color) const
		
		{
		}
	
};
int main(){
	
	//virtual函数是动态绑定 缺省参数是静态绑定
Shape* ps;                //静态类型 Shape*

Shape* pr=new Rectangle; //静态类型 Shape*

Shape* pc=new Circle;    //静态类型 Shape*

ps=pc;       //动态类型 Circle*
ps=pr;       //动态类型 Rectangle*

pr->draw();//Rectangle::draw(Shape::Red);

}