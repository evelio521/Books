// myapi.h 
class MyAPI {
    public:
    static const int MAX_NAME_LENGTH; 
	static const int MAX_RECORDS;
    static const std::string LOG_FILENAME; 
};
    
//You can then define the value for these constants in the associated .cpp file.
// myapi.cpp
const int MyAPI::MAX_NAME_LENGTH = 128;
const int MyAPI::MAX_RECORDS = 65525;
const std::string MyAPI::LOG_FILENAME = "filename.log";

Finally, you can step back and consider the actual size requirements for each variable rather than simply using 
ints for all integer values. Doing so, you might decide that the RGB variance values only need to be 1 byte, 
and the screen space coordinates can be 2 bytes each. I’ll use the types of char and short, respectively, 
to simplify what follows, but in reality you could use size-specific types such as int8_t and uint16_t.

class Fireworks_D {
	bool mIsActive:1;
	bool mVaryColor:1; 
	bool mRepeatCycle:1; 
	bool mFadeParticles:1; 
	char mRed;
	char mGreen;
	char mBlue;
	char mRedVariance; 
	char mGreenVariance; 
	char mBlueVariance; 
	int mTotalParticles; 
	short mOriginX; 
	short mOriginY;
};

class Vector {
	public:
		double GetX() const; 
		double GetY() const; 
		double GetZ() const; 
		void SetX(double x); 
		void SetY(double y); 
		void SetZ(double z);
	private:
		double mX, mY, mZ;

};
#include "detail/Vector.h"// 实现具体细节


// myobject.h 
class MyObject {
	public:
		MyObject();
		std::string GetValue() const;
		void SetValue(const std::string &value);
	private:
		CowPtr<std::string> mData;
};

// myobject.cpp MyObject::MyObject() : mData(0) {}
std::string MyObject::GetValue() const {
    return (mData) ? *mData : "";
}

void MyObject::SetValue(const std::string &value) {
    mData = new std::string(value);
}

MyObject obj1;
obj1.SetValue("Hello");
MyObject obj2 = obj1;
std::string val = obj2.GetValue();
MyObject obj3 = obj1;
obj3.SetValue("There");

