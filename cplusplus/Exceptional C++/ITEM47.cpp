ITEM47:������
	//������Ĵ������ҳ���������ë��
	#include <cassert> 
	#include <iostream>
	#include <typeinfo>
	#include <string>
	using namespace std;
	
	//  The following lines come from other header files.
	//ȫ�ֱ����ĳ���Ӧ�����������ǵľ�����ʹ�����ر�������Щ��ͼ��������ʼ��֮ǰ��ʹ�����Ĵ���
	//�ڸ緭�뵥Ԫ֮�����Щȫ�ֱ���(������ľ�̬����)�ĳ�ʼ��˳��δ����
	char* itoa( int value, char* workArea, int radix );
	extern int fileIdCounter;
	//
	// ʹ��Ĳ���������Զ����ĸ�������
	//
	template<class T>
	inline void AAssert( T& p )
	{
	  static int localFileId = ++fileIdCounter;
	  //����������ˣ����������ǡ���ڳ�ʼ���κ�AAssert<T>::localFileIdCounter�����˳�ʼ������ô����ã�
	  //����Ļ��������ֵ������fileIdCounter�ڳ�ʼ��ǰ����ռ�õ��ڴ����е�����
	  if( !p.Invariant() )
	  {
	    cerr << "Invariant failed: file " << localFileId
	         << ", " << typeid(p).name()
	         << " at " << static_cast<void*>(&p) << endl;
	    assert( false );
	  }
	}
	
	template<class T>
	class AInvariant
	{
	public:
	  AInvariant( T& p ) : p_(p) { AAssert( p_ ); }
	  ~AInvariant()              { AAssert( p_ ); }
	private:
	  T& p_;
	};
	#define AINVARIANT_GUARD AInvariant<AIType> invariantChecker( *this )
	//ʹ����Щ���������Ǹ�������˼�����⣬��������ʹһ�����ں������õ�ǰ���Զ����в������ļ�顣
	//ֻҪ�򵥵Ķ���һ��AItype��typedef,Ȼ���ٰ�AINVARIANT_GUARD��Ϊ��Ա�����ĵ�һ�����Ϳ�����
	//��������������ȫ�ǲ��õġ�
	
	//Ȼ��������Ĵ����У����������ͺܲ��ҵı��һ�㶼����Ȥ����Ҫԭ����Ainvariant�����˶�assert()��
	//���ã�����������non-debugģʽ�±�������ʱ�򣬱��������Զ���ɾ����assert()��
	//-------------------------------------------------------------
	template<class T>
	class Array : private ArrayBase, public Container
	{
	  typedef Array AIType;
	public:
	  Array( size_t startingSize = 10 )
	  : Container( startingSize ),
	    ArrayBase( Container::GetType() ),
	    	//���ﹹ�캯���ĳ�ʼ����������Ǳ�ڴ��󡣵�һ����������س���Ϊ���󣬵�����
	    	//���ڴ��������ֻ��γ�һ��������������۷������Ƿ�����˵�������:
	    	//1.���GetType()��һ����̬��Ա������������һ���Ȳ�ʹ��thisָ���ֲ��ܹ����
	    	//��������(��̬��ʹ�ü���)Ӱ��ĳ�Ա��������ô����ֻ�ǲ����ı�������ѣ���Ȼ����ȷ����
	    	//2.����Ļ������Ǿ����鷳�ˣ���virtual�Ļ���ᱻ�����Ұ������Ǳ�������˳���ʼ��
	    	//������ArrayBase����Container֮ǰ��ʼ�������ҵ��ǣ�����ζ����ͼʹ��һ����δ��ʼ����Container subobjectֵ��Ա
	    	
	    	
	    used_(0),
	    size_(startingSize),
	    buffer_(new T[size_])
	    //���������ඨ���˳���ʼ��
	  {
	    AINVARIANT_GUARD;
	    //Ч�ʷ����С���⣬������Invariant()����û�б�Ҫ������2�Σ����Ǹ�С���ⲻ���������鷳��
	  }
		
	  void Resize( size_t newSize )
	  {//���Ǹ����صĿ�������������⡣
	  	//���벻���쳣��ȫ�ġ������new[]�ĵ��õ����׳�һ���쳣�Ļ�����ô������ǰ�Ķ���ᴦ��һ��
	  	//��Ч��״̬������ԭ����buffer��������ڴ�й©���������Ϊ����ָ������ָ�붼��ʧ�˴Ӷ����²��ܽ���ɾ����
	    AINVARIANT_GUARD;
	    T* oldBuffer = buffer_;
	    buffer_ = new T[newSize];
	    copy( oldBuffer, oldBuffer+min(size_,newSize), buffer_ );
	    delete[] oldBuffer;
	    size_ = newSize;
	  }
	
	  string PrintSizes()
	  {
	  	//����itoa()ԭ�ͺ���ʹ��buf��Ϊ��Ž���ĵط�����δ���Ҳ�п�������������⡣
	  	//�����޷���������Ǹ���������жԱ��ʽ����ְ˳����Ϊ�Ժ��������Ĳ���˳����
	  	//û����ȷ�涨�ģ�����ȫȡ�����ض���ʵ�ַ�����
	  	//����Ǹ�������������ֳ�����������������������������õ����õ�չʾ
	  	//return 
			  //operator+(
			    //operator+(
			     // operator+( string("size = "),
			                // itoa(size_,buf,10) ) ,
			     // ", used = " ) ,
			    //itoa(used_,buf,10) );

				//�������Ǽ���size_��ֵΪ10��used_��ֵΪ5.����������operator+()�ĵ�һ�������ȱ���ֵ�Ļ�
				//��ô�����������ȷ��size=10,used=5,��Ϊ��һ��itoa()���������buf����Ľ�����ڵڶ���itoa()
				//��������buf֮ǰ�Ͷ�����ʹ�á�������������operator+()�ĵڶ��������ȱ���ֵ�Ļ�����ô������´����
				//size=10,used=10,��Ϊ�����Ǹ�itoa()�����ȱ���ֵ�����������ڱ�ʹ��֮ǰ���ֱ��ڲ��Ǹ�itoa()�����ٵ���
	    AINVARIANT_GUARD;
	    char buf[30];
	    return string("size = ") + itoa(size_,buf,10) +
	           ", used = " + itoa(used_,buf,10);
	  }
	
	  bool Invariant()
	  {
	  	//��Resize()�ĵ��ô�����������
	  	//1.�������������ѹ���Ͳ���������������Ϊ��������ж�Ϊ�棬��ôResize()�ᱻ���ã����ֻᵼ������
	  	//�ٴε���Invarient()�����������ж���Ȼ��Ϊ�棬Ȼ���ڵ���Resize()�����ֻᵼ�������ٴα�����Invarient()
	  	//������...�������ˡ����Ǹ��޷���ֹ�ĵݹ����
	  	//2.���AAssert()��д�ߴ���Ч�ʷ��濼�ǵĴ��󱨸�Ĵ���ɾ������ȡ����֮��assert(p->Invarient())
	  	//���ֻ���Σ�����ֻ��������Ĵ����ø��ɱ�����Ϊ��assert()�����м����˻���������õĴ���
	  	//����ζ�ų�����debug mode��release mode���ֲ�ͬ�ı���ģʽ�²����Ŀ�ִ�д�����ִ��ʱ���в�ͬ��Ϊ����ʹ
	  	//û�������һ����˵�������⣬��Ҳ�Ǻܲ��õģ�����ζ����Array��������ݽ���ģʽ�Ĳ�ͬ��Resize()��ͬ�Ĵ�����
	  	//ʹ���������Ա���ϵ����������
	  	
	  	//����Ҫ�ڶ�assert()�ĵ����м����и����õĴ��룬��������ȷ�ϵݹ�϶�����ֹ
	    if( used_ > 0.9*size_ ) Resize( 2*size_ );
	    return used_ <= size_;
	  }
	private:
	  T*     buffer_;
	  size_t used_, size_;
	};
	
	int f( int& x, int y = x ) { return x += y; }
	//�ǵڶ�������ȱʡֵ�Ĳ���������ζ�������һ���Ϸ���C++�÷�����һ������ı�������
	//Ӧ�ñ��벻ͨ����˵����÷����û�����Ϊ���������Բ��������˳��Ժ���������ֵ��y
	//���ܸ���x֮ǰ�ȱ���ʼ����
	int g( int& x )            { return x /= 2; }
	
	int main( int, char*[] )
	{
	  int i = 42;
	  cout << "f(" << i << ") = " << f(i) << ", "
	       << "g(" << i << ") = " << g(i) << endl;
	  //���ﻹ�ǶԲ�����ֵ��˳�����⡣����û��ȷ��f(i)��g(i)����ֵ���Ⱥ�˳��
	  //�����ʾ�����Ľ�������Ǵ���ġ���ͬ�ı��������ܻ��в�ͬ�Ľ����
	  Array<char> a(20);
	  cout << a.PrintSizes() << endl;
	}

