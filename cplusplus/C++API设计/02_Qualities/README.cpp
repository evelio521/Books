永远不要返回私有数据成员非const的指针或引用，会破坏封装性
实用pimpl隐藏细节
若无必要 勿增实体
避免将函数声明成可以重写的函数，除非有特别的需求
可以枚举的有限个类型，尽量用enum，枚举代替布尔类型
避免写拥有多个相同类型的参数
函数尽量正交
与成员函数相比 使用非成员、非友元方法可以降低耦合度
冗余的数据降低耦合是合理的
尽量避免include一个完整类的定义
管理器类可以通过封装降低耦合

// myobject.h 
class MyObject {
	public:
		std::string GetName() const;
	protected:
	private:
		std::string mName; 
};
void PrintName(const MyObject &obj);

namespace MyObjectHelper {void PrintName(const MyObject &obj); };


class MyObject; // only need to know the name of MyObject
class MyObjectHolder {
	public:
		MyObjectHolder();
		void SetObject(MyObject *obj); 
		MyObject *GetObject() const;
	private:
		MyObject *mObj;
};

class IdealShower {
	public:
		float GetTemperature() const; // units 1⁄4 Fahrenheit 
		float GetPower() const; // units 1⁄4 percent, 0..100 
		void SetTemperature(float t);
		void SetPower(float p);
	private:
		float mTemperature; 
		float mPower;
};

float IdealShower::GetTemperature() const 
{
	return mTemperature; 
}
float IdealShower::GetPower() const 
{
	return mPower; 
}
void IdealShower::SetTemperature(float t) 
{
	if (t < 42) t = 42; 
	if (t > 85) t = 85; 
	mTemperature = t;
}
void IdealShower::SetPower(float p) 
{
	if (p < 0) p = 0;
	if (p > 100) p = 100; 
	mPower = p;
}


class Year {
	public:
		explicit Year(int y) : mYear(y) {} 
		int GetYear() const { return mYear; }
	private:
		int mYear;
};
class Month {
	public:
		explicit Month(int m) : mMonth(m) {}
		int GetMonth() const { return mMonth; } 
		static Month Jan() { return Month(1); } 
		static Month Feb() { return Month(2); } 
		static Month Mar() { return Month(3); } 
		static Month Apr() { return Month(4); } 
		static Month May() { return Month(5); } 
		static Month Jun() { return Month(6); } 
		static Month Jul() { return Month(7); } 
		static Month Aug() { return Month(8); } 
		static Month Sep() { return Month(9); } 
		static Month Oct() { return Month(10); } 
		static Month Nov() { return Month(11); } 
		static Month Dec() { return Month(12); }
	private:
		int mMonth;
};
class Day {
	public:
		explicit Day(int d) : mDay(d) {} 
		int GetDay() const { return mDay; }
	private:
		int mDay;
};
class Date {
	public:
		Date(const Year &y, const Month &m, const Day &d);
};


enum SearchDirection { 
	FORWARD,
	BACKWARD 
};
enum CaseSensitivity { 
	CASE_SENSITIVE,
	CASE_INSENSITIVE
};
std::string FindString(const std::string &text, SearchDirection direction, 
		CaseSensitivity case_sensitivity);
