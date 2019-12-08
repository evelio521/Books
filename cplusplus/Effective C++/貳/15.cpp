std::tr1::shared_ptr<Investment> pInv(createInvestment());
int daysHeld(const Investment *pi);//返回投资天数
int days=daysHeld(pInv);//error

//auto_ptr和tr1::shared_ptr提供get函数来返回智能指针内部的原始指针
int days=daysHeld(pInv.get());

//auto_ptr和tr1::shared_ptr重载了 operator->和operatot*

class Investment{
	
	public:
		bool isFree()const;
		...
}

Investment *createInvestment();

std::tr1::shared_ptr<Investment> pInv1(createInvestment());
	bool tab1=!(pInv1->isFree());
std::auto_ptr<Investment> pInv1(createInvestment());
	bool tab2=!( (*pInv2).isFree());


//对原始资源的访问可能有显示转转和隐式转换：
//显示转换比较安全 但是对用户不方便
//隐式转换对用户方便但是不安全

FontHandle getFont();//这是C API
void releaseFont();//同一组C API
class Font{
	
	public:
		explicit Font(FontHandle fh):f(fh)
		{
			
		}
		FontHandle get() const//显示转换函数
		{
			return f;
		}
		operator FontHandle()const//隐式转换函数
		{
			return f;
		}
		~Font(){}
	private:
		FontHandle f;
};

void changeFontSize(FontHandle f,int newSize);
Font f(getFont);
int newFontSize;
...

changeFontSize(f.get(),newFontSize);//明白的将Font转换为FontHandle
changeFontSize(f,newFontSize);//隐式的将Font转换为FontHandle

隐式转换的错的发生率高
Font f1(getFont());
...
FontHandle f2=f1;//原来是要拷贝一哥 Font对象
          //却反而将f1转换为FontHandle在复制他