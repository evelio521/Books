/*���ⷵ��handlesָ������ڲ�*/

//����ĳ���������
class Point{
	public:
		Point(int x,int y);
		..
		void setX(int newX);
		void setY(int newY);
		...
};
struct RectData{
	Point ulhc;
	Point lrhc;
};

class Rectangle{
	public:
		Point& upperLeft()const{ return pData->ulhc;}//���Ǳ����ǲ������޸�upperLeft��ֵ ��������ȴ���޸���ֵ
		Point& lowRight()const{ return pData->lrhc;}
		
	...
	private:
		std::tr1::shared_ptr<RectData> pData;
};

Point coord1(0,0);
Point coord2(100,100);
const Ractangle rec(coord1,coord2);//rec�� (0,0)--->(100,100)

rec.upperLeft().setX(50);//rec�� (50,0)--->(100,100)


//���ǿ��������޸ĳ���
class Point{
	public:
		Point(int x,int y);
		..
		void setX(int newX);
		void setY(int newY);
		...
};
struct RectData{
	Point ulhc;
	Point lrhc;
};

class Rectangle{
	public:
		const Point& upperLeft()const{ return pData->ulhc;}//���Ǳ����ǲ������޸�upperLeft��ֵ ��������ȴ���޸���ֵ
		const Point& lowRight()const{ return pData->lrhc;}
		
	...
	private:
		std::tr1::shared_ptr<RectData> pData;
};

Point coord1(0,0);
Point coord2(100,100);
const Ractangle rec(coord1,coord2);//rec�� (0,0)--->(100,100)

rec.upperLeft().setX(50);//wrong ���ǻ���ֿ���handles�����
                         //��֮ ���ⷵ��handlesָ������ڲ�