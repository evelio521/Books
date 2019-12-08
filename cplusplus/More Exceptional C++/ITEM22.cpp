ITEM22:�쳣��ȫ��������֮һ:������ֵ
	//�쳣��ȫ����������
	//1.������֤
	//2.ǿ�ұ�֤
	//3.���׳��쳣�ı�֤
	
	//��׼��auto_ptr��һ����Ҫ�����ǣ�auto_ptr�����������׳��쳣
	
	//����ǿ�ҳ���ȫ�ԵĿ�����ֵ�Ĺ淶��ʲô��
	
	//һ�����ṩһ�����׳��쳣��swap()
	void T::Swap( T& other ) /* throw() */
	{
	  // ...swap the guts of *this and other...
	}
  //���ţ����ô���һ����ʱ����Ȼ�󽻻����ַ�ʵ��operator=()
  T& T::operator=( const T& other )
	{
	  T temp( other ); // do all the work off to the side
	  Swap( temp );    // then "commit" the work using
	  return *this;    //  nonthrowing operations only
	}
	
	//3.�뿴���������
	// Example 22-1: The Cargill Widget Example
	//
	class Widget
	{
	public:
	  Widget& operator=( const Widget& ); // ???
	  // ...
	private:
	  T1 t1_;
	  T2 t2_;
	};
	//����T1��T2��ĳһ���������׳��쳣��������ı���Ľṹ���п���д�������쳣��ȫ��
	//Widget::operator=(const Widget&)��
	
	//������ı�ṹ�������޷�д�������쳣��ȫ��Widget::operator=(const Widget&)
	
	//4.˵��ʾ��һ�ּ򵥵�ת������:����ת������Ӧ�����κ�һ����
	//�����Ժ����׵�ʹ���Ǹ���Ŀ�����ֵ����ǿ�ҵ��쳣��ȫѵ�����������ϣ����ǿ��������ּ�����
	
	//���ܲ��ı�Widget�Ľṹ�����޷�д�������쳣��ȫ��Widget::operator=()
	//����ͨ������ת�����������ǿ���ʵ��һ�����н���ǿ���쳣��ȫ�ĸ�ֵ����
	//ͨ��ָ�������ֵ��ӵ�г�Ա�������ʹ��Pimplת���ַ�����һ��������
	//����ָ��֮��
	
	// Example 22-2: The general solution to
	// Cargill's Widget Example
	//
	class Widget
	{
	public:
	  Widget();  //���µ�WidgetImpl��ʼ�� pimpl_ 
	
	  ~Widget(); //����������������ṩ����Ϊ��ʽ���ɵİ汾��
	       //  ����ʹ���ϵ�����
	       //  (see Items 30 and 31)
	  Widget& operator=( const Widget& );
	
	  // ...
	
	  private:
	    class WidgetImpl;     
	    auto_ptr<WidgetImpl> pimpl_;
	    // ... �ṩ�������������Ŀ���
	    //     ���캯�����߽����� ...
	};
	
	// Ȼ��һ����һ��������ʵ���ļ���
	// implementation file:
	//
	class Widget::WidgetImpl
	{
	public:
	  // ...
	  T1 t1_;
	  T2 t2_;
	};
  //ע�⣬���ʹ��auto_ptr��Ա�����
   //(1) Ҫô������뽫WidgetImpl�Ķ����ṩ��Widget��Ҫô��������������
       //WidgetImpl����ͱ���ΪWidgetд���Լ���������������ʹ������������ܼ�  
   //(2)����Widget����Ӧ���ṩ�Լ��Ŀ������캯���͸�ֵ������һ����˵�㲻ϣ����ĳ�Ա
       //����"ӵ��Ȩת��"���塣�����������һ������ָ�룬����Կ��Ǵ���auto_ptr,��������ԭ����Ȼ��Ҫ
   
   //�������ǿ��Ժ�����ʵ��һ�����׳��쳣��swap����ô���Ǻ�����д��һ����������ǿ���쳣��ȫ��֤�Ŀ�����ֵ����
   void Widget::Swap( Widget& other ) /* throw() */
	{
	  auto_ptr<WidgetImpl> temp( pimpl_ );
	  pimpl_ = other.pimpl_;
	  other.pimpl_ = temp;
	}
	Widget& Widget::operator=( const Widget& other )
	{
	  Widget temp( other ); // do all the work off to the side
	  Swap( temp );    // then "commit" the work using
	  return *this;    //  nonthrowing operations only
	}









  //�õ��Ľ���
  //����1: �쳣��ȫӰ��������
  //����2: ��������Ĵ������ǿ�ҵ��쳣��ȫ��
  //����3: ���ǵ�ʹ��ָ��(1)ָ������ĵ��ˣ�������������������auto_ptrҪ����������
                      //(2)ָ����������ѣ�ʹ��ָ�벻������쳣
  
