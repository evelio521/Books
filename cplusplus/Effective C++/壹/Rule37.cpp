/*�������¶���̳ж�����ȱʡ����*/

class Shape{
	
	public:
		enum ShapeColor{Red,Green,Blue};
		//���е���״�������ṩһ������ �����Ƴ��Լ�
		virtual void draw(ShapeColor color=Red) const=0;
		...
};

class Rectangle:public Shape{
	public:
		//���︳�費ͬ��ȱʡ���� ����⣡
		virtual void draw(ShapeColor color=Green) const;
	
};

class Circle:public Shape{
	public:
		//���︳�費ͬ��ȱʡ���� ����⣡
		virtual void draw(ShapeColor color) const;
	
};

Shape* ps;                //��̬���� Shape*

Shape* pr=new Rectangle; //��̬���� Shape*

Shape* pc=new Circle;    //��̬���� Shape*

ps=pc;       //��̬���� Circle*
ps=pr;       //��̬���� Rectangle*

pr->draw();//Rectangle::draw(Shape::Red);

//Ҳ�����ǿ��Ըı�������ƽṹ

class Shape{
	
	public:
		enum ShapeColor{ Red,Blue,Green};
		void draw(ShapeColor color=Red) const
		{
			doDraw(color);
		}
	private:
		virtual void doDraw(ShapeColor color=Red) const=0;//�����Ĺ����ٴ����
		
};
class Rectangle:public Shape{
	public:
		//���︳�費ͬ��ȱʡ���� ����⣡
		virtual void doDraw(ShapeColor color) const;//����Ҫָ��ȱʡ����
	
};
