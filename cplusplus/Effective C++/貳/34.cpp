#include<iostream>
using namespace std;
//pure virtual函数的用法。
/*
class Shape{
	
	public:
		virtual void draw() const=0;
		
		virtual void error(const std::string& msg)
			{
			}
		int objectID()const{
			
		}
		
};
void Shape::draw() const
{
			cout<<"pure virtual "<<endl;
}

class Rectangle:public Shape{ 
	public:
		virtual void draw()const
		{
			cout<<"virtual "<<endl;
		}
	
	};
class Ellipse:public Shape{ 
	public:
		virtual void draw()const
		{
			cout<<"virtual "<<endl;
		}
	
	};
	int main(){
		
//Shape* ps=new Shape;//Shape是抽象类
Shape* ps1=new Rectangle;
ps1->draw();//Rectangle::draw()
Shape* ps2=new Ellipse;
ps2->draw();//Ellipse::draw()

ps1->Shape::draw();
ps2->Shape::draw();
}*/

//
class Airport { 
	
	};
class Airplane{
	
	public:
		virtual void fly(const Airport& destination)=0;
		
};

void Airplane::fly(const Airport& destination)
{
	  cout<<"default fly"<<endl;
}

class ModelA:public Airplane{ 
	public:
		virtual void fly(const Airport& destination)
		{
			Airplane::fly(destination);
		}
	
};
class ModelB:public Airplane{
	
	public:
		virtual void fly(const Airport& destination)
		{
			Airplane::fly(destination);
		}
	 
};
	
class ModelC:public Airplane{
	virtual void fly(const Airport& destination);
	 
};
void ModelC::fly(const Airport& destination)
{
	cout<<"C fly"<<endl;
}

int main(){
	
	Airport airp;
	Airplane *ap1=new ModelA();
	ap1->fly(airp);
	Airplane *ap2=new ModelB();
	ap2->fly(airp);
	Airplane *ap3=new ModelC();
	ap3->fly(airp);



	
	
}