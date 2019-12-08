class Base{
	public:
		Base(){};
		virtual ~Base(){}
		virtual size_t size()const {
			return 100;
		}
		bool operator>(int val){
			
			return true;
		}
		bool operator!=(Base val){
			
			return true;
		}
	
	
};

class Derived:public Base{
	
	public:
		
	
};
class Temp{
	public:
		operator Derived()const{
			
		return base;
		}
private:
	Derived base;
	
	
	};
template<typename T>
void doProcessing(T& w)
{
	T someNastyWidget;
	if(w.size()>0 && w!=someNastyWidget){//if遵守的规则见教材
		
			
	}
}

int main()
{
	Base b;
	Derived d;
	Temp t;
	doProcessing(d);
	
	
	
}