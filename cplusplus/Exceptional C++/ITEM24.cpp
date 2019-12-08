ITEM24:����/ʹ�ü̳�
	//Ԫ�ؼ̳е����������ǿ�ģ����ʱ�����Խ��Խ��
	
	//һ��template�ṩ��list�Ĺ����ܣ������ض���listλ���ϵĴ���Ԫ�ص�����
	// Example 1 
	//
	template <class T>
	class MyList
	{
	public:
	  bool   Insert( const T&, size_t index );
	  T      Access( size_t index ) const;
	  size_t Size() const;
	private:
	  T*     buf_;
	  size_t bufsize_;
	};
	//�������´��룬���г�����MyListΪ������ʵ����һ��MySet class �Ĳ�ͬ���������е���ҪԪ���Գ���
	// Example 1(a) 
		//
		template <class T>
		class MySet1 : private MyList<T>
		{
		public:
		  bool   Add( const T& ); // calls Insert()
		  T      Get( size_t index ) const;
		                          // calls Access()
		  using MyList<T>::Size;
		  //...
		};
		
		// Example 1(b)
		//
		template <class T>
		class MySet2
		{
		public:
		  bool   Add( const T& ); // calls impl_.Insert()
		  T      Get( size_t index ) const;
		                          // calls impl_.Access()
		  size_t Size() const;    // calls impl_.Size();
		  //...
		private:
		  MyList<T> impl_;
		};
		
    //MySet1 MySet2������ʵ������Ĳ�� ��ɵĹ���һ��
    
    //���׼�򣺾����Ĳ���aggregation(����composition��layering,HAS-A,delegation)��ȡ��
              //inheritance��������׼����Ĥ��IS-IMPLEMENTED-IN-TERMS-OF��ϵʱ������aggregation
              //����Ҫ����inheritance
    //��Щ����inheritance������containment������
      1.��������Ҫ��д����ʽʱ��
      2.��������Ҫ����protected members--һ�������protected member functions����ʱ����protected ctors
      3.��������Ҫ��һ��base subobject֮ǰ�ȹ���used object��������һ��base subobject֮��ݻ�used object�����Ǳ�����inheritance
      4.��������Ҫ(1)����ĳ����ͬ��virtual base classes��(2)��дĳ��virtual base classes�Ľ�������ʱ�����Ǳ�����inheritance
      5.�����Ǵ�empty base class����ѻ����ʵ������ʱ����ʱ�򣬾���ʵ�����Ǹ�class��û���κε����ݳ�Ա��Ҳ����˵��ֻ�Ǻ�������ϡ�
        �����������inheritanceȡ��containment������empty base class��ѻ�֮�ʶ���ÿռ����ơ�
        �򵥵�˵������������һ��empty base subobjectռ����ռ䣺��Ȼһ��empty member object����ռ�÷���ռ䣬��ʹ�������κ�����
        
        class B { /* ... functions only, no data ... */ }; 
				// Containment: incurs some space overhead
				//
				class D
				{
				  B b_; // b_ must occupy at least one byte,
				};      // even though B is an empty class
				// Inheritance: can incur zero space overhead
				//
				class D : private B
				{      // the B base subobject need not
				};      // occupy any space at all
		6.public inheritance����Ӧ����ĤIS-A�Ĺ�ϵ��nonpublic inheritance���Ա���һ���յ�������IS-A��ϵ��
		
		
		//�����и�class DErived��public Base�����ⲿ����DErived���������һ��Base���������޷��Զ��͵ķ�ʽ����һ��Base����Ϊprivate inheritance������ȡ�ϵ�������
		//Ȼ����Derived�Լ� ��members functions��friends�ڣ�һ��DErived object�����Զ��͵ķ�ʽ����������һ��base������ӦΪmembers functions ��friend���㹻�Ĵ�ȡȨ��
		//���������private inheritance������protected inheritance����ôIS-A�Ĺ�ϵ�Ը����DErived classes�������Щ����ζ��subclasses����ʹ�ö��͡�
		
		
		//����һ��MyList MySet1 MySet2
		
		1.MyListû��protected members ���������ǲ���Ҫ�̳�����ȡ����
		2.MyListû�����⺯��ʽ������Ҫ�̳�����д����
		3.MySetû������Ǳ�ڵ�base classes������MyList�������Ҫ������һ��basesubject֮ǰ������֮��ݻ�����
		4.MyListû���κε�virtual base classes ��hiMySet������Ҫ����ģ�����construction������Ҫ��д�ġ�
		5.MyList���ǿյģ�����empty base classes ����������
		6.MySet����һ��MyList��������MySet��members functions��friend��Ҳ���ǡ�
		
		//����:MySet��Ӧ�ü̳�MyList����containmentͬ����Ч�ʵ������ʹ��inheritance��ֻ�ᵼ���޳���ż�����Ǳ�Ҫ��������
		
		
		//��ʱ��inheritance���Ǳ�Ҫ��
		// Example 2: Sometimes you need to inherit 
		//
		class Base
		{
		public:
		  virtual int Func1();
		protected:
		  bool Func2();
		private:
		  bool Func3(); // uses Func1
		};
		//������Ҫ��дFunc2,���ȡһ��protected member Func2,�ͱ������inheritance
		
		//����Ĵ��벻�Ǻܺã����������
		// Example 2(a) 
		//
		class Derived : private Base // necessary?
		{
		public:
		  int Func1();
		  // ... more functions, some of which use
		  //     Base::Func2(), some of which don't ...
		};
		//������Ĵ���ĸĽ�
		// Example 2(b) 
		//
		class DerivedImpl : private Base
		{
		public:
		  int Func1();
		  // ... functions that use Func2 ...
		};
		class Derived
		{
		  // ... functions that don't use Func2 ...
		private:
		  DerivedImpl impl_;
		};
		
		//���ϴ������õĸ��벢��װ��Base�������ԡ�DErivedֻ����Base��public�����Լ�DerivedImpl��public���档
		
		
		//Containment���ŵ�
		   1.����������ӵ�ж��used classesʵ�壬���inheritance���Բ���ʵ�ã�����������
		   2.����used classes��Ϊһ��data members����������൯�ԡ�
		
		//��MySet2 ����д
		// Example 1(c): Generic containment 
		//
		template <class T, class Impl = MyList<T> >
		class MySet3
		{
		public:
		  bool   Add( const T& ); // calls impl_.Insert()
		  T      Get( size_t index ) const;
		                          // calls impl_.Access()
		  size_t Size() const;    // calls impl_.Size();
		  // ...
		private:
		  Impl impl_;
		};
		
		//��ѭһ�¹��򣬱��ⳣ���Ĵ���
		1.���nonpublic inheritance���ã����Բ�Ҫ����public inheritance
		2.���class֮����໥��ϵ�����ö��ַ�ʽ���֣���ʹ������������һ�ֹ�ϵ
		
		//���׼��:����ȷ��public inheritance������ģ��IS-A��WORKS-LIKE-A�Ĺ�ϵ��������Liskov Substitution Principle.
		          //���б���д��member functions�����벻Ҫ����࣬Ҳ����ŵ����
		
		//���׼��:���Բ�ҪΪ���ظ�ʹ��base class �Ĵ���ʹ��public inheritance��public inheritance��Ŀ����Ϊ�˱����еĴ����Զ��ͷ�ʽ�ظ�����base objects.
		
		//���ۣ�
		   1.���ǵ�����inheritance
		   2.������ܹ���������containment/delegation����ĳ��class�ļ��ϵ����Ӧ����ô����
		   3.�������Ҫinheritance��������ģ��IS-A�Ĺ�ϵ����ʹ��nonpublic inheritance
		
				
						
				        
				      
				
						
				        
				      