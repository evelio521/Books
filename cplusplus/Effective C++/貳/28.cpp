/*避免返回handles指向对象内部*/

//下面的程序有问题
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
		Point& upperLeft()const{ return pData->ulhc;}//我们本意是不容许修改upperLeft的值 但是我们却能修改其值
		Point& lowRight()const{ return pData->lrhc;}
		
	...
	private:
		std::tr1::shared_ptr<RectData> pData;
};

Point coord1(0,0);
Point coord2(100,100);
const Ractangle rec(coord1,coord2);//rec是 (0,0)--->(100,100)

rec.upperLeft().setX(50);//rec是 (50,0)--->(100,100)


//我们可以这样修改程序
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
		const Point& upperLeft()const{ return pData->ulhc;}//我们本意是不容许修改upperLeft的值 但是我们却能修改其值
		const Point& lowRight()const{ return pData->lrhc;}
		
	...
	private:
		std::tr1::shared_ptr<RectData> pData;
};

Point coord1(0,0);
Point coord2(100,100);
const Ractangle rec(coord1,coord2);//rec是 (0,0)--->(100,100)

rec.upperLeft().setX(50);//wrong 但是会出现空悬handles的情况
                         //总之 避免返回handles指向对象内部