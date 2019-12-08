/*接口继承和实现继续*/
class Shape{
	
	public:
		virtual void draw()const=0;
		virtual void error(const std::string& msg);
		int objectID()const;
		...
};

class Rectangle:public Shape{ ...};
class Ellipse:public Shape{ ...};
	
Shape* ps=new Shape;//Shape是抽象类
Shape* ps1=new Rectangle;
ps1->draw();//Rectangle::draw()
Shape* ps1=new Ellipse;
ps2->draw();//Ellipse::draw()

ps1->Shape::draw();
ps2->Shape::draw();
	
//考虑以下代码的缺陷和改进方法
class Airport { ...};
class Airplane{
	
	public:
		virtual void fly(const Airport& destination);
		...
};

void fly(const Airport& destination)
{
	缺省代码 将飞机飞至指定的目的地
}
class ModelA:public Airplane{ ...};
class ModelB:public Airplane{ ...};
	
class ModelC:public Airplane{
	 ...
};//C的飞行方式可能不一样，这是灾难的原因

Airport PDX(...);
Airplane* pa=new ModelC;
...
pa->fly(PDX);//Airplane::fly()

//下面是一种解决方法

class Airport { ...};
class Airplane{
	
	public:
		virtual void fly(const Airport& destination);
		...
	protected:
		void defaultFly(const Airport& destination);
};

void Airplane::defaultFly(const Airport& destination)
{
	缺省代码 将飞机飞至指定的目的地
}

class ModelA:public Airplane{ 
	public:
		virtual void fly(const Airport& destination)
		{
			defaultFly(destination);
		}
	...
};
class ModelB:public Airplane{
	
	public:
		virtual void fly(const Airport& destination)
		{
			defaultFly(destination);
		}
	 ...
};
	
class ModelC:public Airplane{
	virtual void fly(const Airport& destination);
	 ...
};
void ModelC::fly(const Airport& destination)
{
	缺省代码 将飞机C飞至指定的目的地
}

//另一种解决方法
class Airport { ...};
class Airplane{
	
	public:
		virtual void fly(const Airport& destination)=0;
		...
};

void Airplane::fly(const Airport& destination)
{
	缺省代码 将飞机飞至指定的目的地
}

class ModelA:public Airplane{ 
	public:
		virtual void fly(const Airport& destination)
		{
			Airplane::fly(destination);
		}
	...
};
class ModelB:public Airplane{
	
	public:
		virtual void fly(const Airport& destination)
		{
			Airplane::fly(destination);
		}
	 ...
};
	
class ModelC:public Airplane{
	virtual void fly(const Airport& destination);
	 ...
};
void ModelC::fly(const Airport& destination)
{
	缺省代码 将飞机C飞至指定的目的地
}