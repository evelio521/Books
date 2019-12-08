/*�ӿڼ̳к�ʵ�ּ���*/
class Shape{
	
	public:
		virtual void draw()const=0;
		virtual void error(const std::string& msg);
		int objectID()const;
		...
};

class Rectangle:public Shape{ ...};
class Ellipse:public Shape{ ...};
	
Shape* ps=new Shape;//Shape�ǳ�����
Shape* ps1=new Rectangle;
ps1->draw();//Rectangle::draw()
Shape* ps1=new Ellipse;
ps2->draw();//Ellipse::draw()

ps1->Shape::draw();
ps2->Shape::draw();
	
//�������´����ȱ�ݺ͸Ľ�����
class Airport { ...};
class Airplane{
	
	public:
		virtual void fly(const Airport& destination);
		...
};

void fly(const Airport& destination)
{
	ȱʡ���� ���ɻ�����ָ����Ŀ�ĵ�
}
class ModelA:public Airplane{ ...};
class ModelB:public Airplane{ ...};
	
class ModelC:public Airplane{
	 ...
};//C�ķ��з�ʽ���ܲ�һ�����������ѵ�ԭ��

Airport PDX(...);
Airplane* pa=new ModelC;
...
pa->fly(PDX);//Airplane::fly()

//������һ�ֽ������

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
	ȱʡ���� ���ɻ�����ָ����Ŀ�ĵ�
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
	ȱʡ���� ���ɻ�C����ָ����Ŀ�ĵ�
}

//��һ�ֽ������
class Airport { ...};
class Airplane{
	
	public:
		virtual void fly(const Airport& destination)=0;
		...
};

void Airplane::fly(const Airport& destination)
{
	ȱʡ���� ���ɻ�����ָ����Ŀ�ĵ�
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
	ȱʡ���� ���ɻ�C����ָ����Ŀ�ĵ�
}