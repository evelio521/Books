std::tr1::shared_ptr<Investment> pInv(createInvestment());
int daysHeld(const Investment *pi);//����Ͷ������
int days=daysHeld(pInv);//error

//auto_ptr��tr1::shared_ptr�ṩget��������������ָ���ڲ���ԭʼָ��
int days=daysHeld(pInv.get());

//auto_ptr��tr1::shared_ptr������ operator->��operatot*

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


//��ԭʼ��Դ�ķ��ʿ�������ʾתת����ʽת����
//��ʾת���Ƚϰ�ȫ ���Ƕ��û�������
//��ʽת�����û����㵫�ǲ���ȫ

FontHandle getFont();//����C API
void releaseFont();//ͬһ��C API
class Font{
	
	public:
		explicit Font(FontHandle fh):f(fh)
		{
			
		}
		FontHandle get() const//��ʾת������
		{
			return f;
		}
		operator FontHandle()const//��ʽת������
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

changeFontSize(f.get(),newFontSize);//���׵Ľ�Fontת��ΪFontHandle
changeFontSize(f,newFontSize);//��ʽ�Ľ�Fontת��ΪFontHandle

��ʽת���Ĵ�ķ����ʸ�
Font f1(getFont());
...
FontHandle f2=f1;//ԭ����Ҫ����һ�� Font����
          //ȴ������f1ת��ΪFontHandle�ڸ�����