//�������¶���̳ж�����ȱʡ����

class Shape{
	
	public:
		enum ShapeColor{Red,Green,Blue};
		//���е���״�������ṩһ������ �����Ƴ��Լ�
		virtual void draw(ShapeColor color=Red) const=0;
		
};

class Rectangle:public Shape{
	public:
		//���︳�費ͬ��ȱʡ���� ����⣡
		virtual void draw(ShapeColor color=Green) const
		
		{
		}
	
};

class Circle:public Shape{
	public:
		//���︳�費ͬ��ȱʡ���� ����⣡
		virtual void draw(ShapeColor color) const
		
		{
		}
	
};
int main(){
	
	//virtual�����Ƕ�̬�� ȱʡ�����Ǿ�̬��
Shape* ps;                //��̬���� Shape*

Shape* pr=new Rectangle; //��̬���� Shape*

Shape* pc=new Circle;    //��̬���� Shape*

ps=pc;       //��̬���� Circle*
ps=pr;       //��̬���� Rectangle*

pr->draw();//Rectangle::draw(Shape::Red);

}