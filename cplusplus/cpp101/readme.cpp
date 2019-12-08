// 0 ��Ҫ������С��
    0.1 ��Ҫ�涨�������٣�Ӧ�ù涨ʹ�����������ִ�����
    0.2 ��Ҫǿ���еľ��峤�ȣ����ִ����������Ķ�
    0.3 ��Ҫ����������涨���࣬Ӧ�ù涨ͳһ�������淶
    0.4 ��Ҫ�涨ע������������Ҫ��ȡ�ĵ���
    0.5 ���������򵥶�С���������
1 �ڸ߾��漶��ɾ�����Ľ��б��룬���Ź��κξ���
    1.1 ������������ľ���
    //File: myproj/myjambda.h -- wraps Boost's lambda.hpp
    // Always include this file; don't use lambda.hpp directly.
    // NOTE: Our build now automatically checks "grep lambda.hpp <srcfile>".
    // Boost.Lambda produces noisy compiler warnings that we know are innocuous. 
    // When they fix it we'll remove the pragmas below, but this header will still exist.
     //
    #pragma warning(push) //disable for this header only
    #pragma warning(disable:4512) 
    #pragma warning(disable:4180)
    #include <boost/lambda/lambda.hpp> 
    #pragma warning(pop) //restore original warning level
    1.2 ����δʹ�õĺ�������
    //... inside a user-defined allocator that has no use for the hint... 
    // warning: "unused parameter 'localityHint'"
    pointer allocate( sizejype numObjects, const void *localityHint = 0 )
    { 
        return static_cast<pointer>( mallocShared( numObjects * sizeof(T)));
         }
    pointer allocate( sizejype numObjects, const void * /* localityHint */ = 0 ) 
    {
      return static_cast<pointer>( mallocShared( numObjects * sizeof(T)) );
    }
    1.3 ������δʹ�õı���
    // warning: "variable lock' is defined but never used" 
    void Fun() { 
        Lock lock;
    }
    // new version: probably eliminates warning 
    void Fun() {
        Lock lock;
        lock��
    }
    1.4 ����ʹ��ǰҪ��ʼ��
    1.5 ��ֹ��©return���
    //warning: missing "return"
    int Fun( Color c ) {
      switch( c) {
       case Red: return 2;
       case Green: return 0; 
       case Blue:
       case Black: return 1;
      } 
    }
    // /"string" evaluates to false
    // new version: eliminates warning 
    int Fun( Color c ) {
        switch( c) {
            case Red: return 2;
            case Green: return 0;
            case Blue:
            case Black: return 1;
            default: assert( {"should never get here!" ); return -1;
        } 
    }
    1.6 �з��������޷�������Ҫ�Ƚϣ���ƥ��
2 ʹ���Զ�����ϵͳ
    2.1 ���κξ��棬һ������
3 ʹ�ð汾����ϵͳVCS
4 �ڴ��������Ͷ��
5 һ��ʵ��Ӧ��ֻ��һ�����յ�ְ��һ��ֻ���һ�����⣩
    5.1 ֻ��һ��ʵ�帳��һ���ض�������ְ��
6 ��ȷ���򵥺�������һ
    6.1 KISS ��ȷ�����ٶ� �����ڸ��ӡ� �������ڻ��� ��ȫ���ڲ���ȫ��
    6.2 ��Ҫʹ�ò���Ҫ�Ĳ���������
    6.3 ���캯���Ĳ�����Ҫʹ����ʱ����
7 �����Ӧ֪����ʱ����ο��ǿ������ԣ������㷨���ȣ�
    7.1  ��̬�������ݣ���Ҫʹ�ù̶���С������
    7.2 �˽��㷨�ĸ�����
    7.3 ����ʹ�������㷨�򾡿��ܿ���㷨
    7.4 ��Զ��Ҫʹ��ָ���������㷨
8 ��Ҫ���в�������Ż�
    8.1 ��һ����ȷ�ĳ�������٣�����һ�����ٵĳ�����ȷҪ���׵Ķ�
9 ��Ҫ���в�������ӻ�
    9.1 �ܴ����ò���ֵ
    9.2 ��ʹ��ǰ׺��++ ��ʵ�ú�׺��--
    9.3 ����ʹ�ó�ʼ���б�
10 ��������ȫ�ֺ͹�������

11 ������Ϣ����Ҫ�����ṩ����ʵ����ڲ���Ϣ��

12 ���ú�ʱ����ν��в����Ա�̣����̣߳���������Ҫ�������

13 ȷ����ԴΪ������ӵ�У�ʹ����ʽ��RAII������ָ��
    class Port { 
        public:
          Port( const strings destination ); // call OpenPort 
          ~ Port(); // call ClosePort
    };
    //... ports can't usually be cloned, so disable copying and assignment...
    void DoSomething() { 
        Port portl( "serverl:80" );
    } //can't forget to close portl; it's closed automatically at the end of the scope
    shared_ptr< Port> port2 =/*... */ //port2 is closed automatically when the
                                      //last shared_ptr referring to it goes away
 
    void Fun( shared_ptr<Widget> spl, shared_ptr<Widget> sp2 );
    Fun( shared_ptr<Widget>(new Widget), shared_ptr<Widget>(new Widget) );
14 ��Ҫ����ʱ������ʱ����Ҳ��Ҫ����ʱ����
   14.1 ����ʱbool����ʹ�ö���
   14.2 ����ʱ��̬������ʹ��ģ������麯��
   14.3 ��������ʹ��ö��
   14.4 dynamic_cast ʹ�ý϶ཨ��������ƽӿ�
15 ����ʹ��const
   15.1 ������Ҫǿ��ת��const
    void Fun( int x );
    void Fun( const int x ); //redeclares the same function: top-level const is ignored

    void Fun( const int x ) { //Fun's actual definition
        ++x; // error: cannot modify a const value
    }
16 ����ʹ�ú�
    16.1 ���н׶ξ���ʹ��assert
17 ����ʹ�á�ħ������3.1415�ȣ�
    const sizej PAGE_SIZE = 8192,
                WORDS_PER_PAGE = PAGE_SIZE/sizeof(int), 
                INFO_BITS_PER_PAGE = 32 * CHAR_BIT;

    //File widget.h
    class Widget {
        static const int defaultWidth = 400; // value provided in declaration
        static const double defaultPercent; // value provided in definition 
        static const char* Name() {return "Widget";}
    };

    //File widget.cpp
    const double Widget::defaultPercent = 66.67; // value provided in definition
    const int Widget::defaultWidth; //definition required
18 �����ֲܾ�����������
19 ���ǳ�ʼ������,�����������������
20 ���⺯������������Ƕ�׹���
    ��ʤ�ڳ� ƽ������
    20.1 �������գ�һ������ֻ����һ��ְ��
    20.2 ��Ҫ�����ظ�����Ҫ�����ƵĴ����ظ�����
    20.3 ����ʹ��&& ��&&����Ƕ��ѭ��
    20.4 ����ʹ�ñ�׼�㷨
    20.5 ��Ҫ���ݱ�ǩ���ͽ��з�֧switch
21 �������뵥Ԫ�ĳ�ʼ���� ������ɱ��⣬ʹ��Singleton
22 �������ٶ���������������ѭ������
    22.1 ��ʹ��ǰ����������Ҫʹ��include
23 ͷ�ļ�Ӧ���Ը����㣬��������ÿһ��ͷ�ļ�
24 ���Ǳ�д�ڲ�#include������������Ҫ��д�ⲿ #include���������������룩
    #ifndef FOO_H_INCLUDED_ 
    #define FOO_HJNCLUDED_ 
    //... contents of the file... 
    #endif
25 ��ȷ��ѡ��ͨ��ֵ��(����)ָ��������ô��ݲ���
    ����ֻ�������
    25.1 ʼ��ʹ��const����ָ�����������ָ�������
    25.2 ����ͨ��ֵ��ȡ��ԭʼ���ͺ͸��ƿ����Ƚϵ͵�ֵ���������
    25.3 ����ʹ��const������ȡ�������û��������͵�����
    25.4 ���������Ҫ��������ʹ�ô�ֵ�ķ�ʽ
    �������
    25.1 �����ǿ�ѡ�ģ�����ʹ��ָ�봫��
    25.2 �����Ǳ���ģ�����ʹ�����ô���
26 �������ز���������Ȼ����
27 ����ʹ�������������͸�ֵ�������ı�׼��ʽ
   String& String::operator+=( const String& rhs) 
   { 
      //... implementation...
      return *this; 
   }
   String operator+( const String& Ihs, const String& rhs) 
   { 
       String temp; //initially empty
       temp.Reserve( lhs.size() + rhs.size()); //allocate enough memory
       return (temp += Ihs) += rhs; //append the strings and return
   }
28 ����ʹ��++�͡��ı�׼��ʽ������ʹ��ǰ׺��ʽ
 T& T::operator+ + ()               T& T::operator--() // the prefix form:
 {                                  { 
     //perform increment                //perform decrement // - do the work
     return *this;                      return *this; // - always return *this;
 }                                  }
 
 T T::operator++(int)               T T::operator��(int) //the postfix form: 
 {                                  {  
     T old(*this);                      T old( *this); // - remember old value
     ++*this;                           --*this; // - call the prefix version
     return old;                        return old; // - return the old value
 }                                  }
29 ���������Ա�����������ת��
    bool operator==( const String& Ihs, const String& rhs); 
    bool operator==( const String& Ihs, const char* rhs); 
    bool operator==( const char* Ihs, const String& rhs);
30 ��������&&��||����

31 ��Ҫ��д�����ں���������ֵ˳��Ĵ���
    int count = 5;
    Transmogrify( ++count, ++count); //order of evaluation unknown
32 Ū����Ҫ��д���������ࣨ��ֵ�ࡢ���ࡢ�����ࡢ�쳣�ࣩ
33 ��С��������
34 ����ϴ���̳�
35 ����Ӳ���Ҫ��Ƴɻ�������м̳�
    35.1 ��Ҫ��string�̳�
    35.2 ����ϴ��湫�м̳л�˽�м̳�
    class localized_string {
         public:
           //... provide passthroughs to string member function that we want to 
           //retain unchanged (e.g., define insert that calls impl_.insert) ...
           void clear(); // mask/redefine clear() 
           bool is_in_klingon() const; // add functionality
        private:
        std::string impl_; //... add extra sta
    };

36 �����ṩ����ӿڣ���������ԭ��Dependency Inversion Principle��DIP��
    36.1 �ӿ�һ��Ҫ��ȷ�����ڿ��ܲ������޸�
37 ���ü̳м����滻�ԣ��̳в���Ϊ�����ã�����Ϊ�˱�����
    37.1 �̳в����ƻ���ȷ�� Rectangle �� Square�Ͳ����໥�̳�
38 ʵʩ��ȫ�ĸ�д����Զ��Ҫ�޸�Ĭ�ϲ������̳����д�ĺ���Ӧ�����virtual����Ȼ������ģ���
39 ���ǽ����⺯������Ϊ�ǹ��õģ������ú�������Ϊ������ģ�non-virtaual interface,NVI��
40 Ҫ�����ṩ��ʽת���������ṩ��ʾת����explicit��������ת������, ����ʹ������
41 �����ݳ�Ա��Ϊ˽�еģ�����Ϊ�ľۼ���C����struct�����⣬ ���ǵ�ʹ��Pimpl
42 ��Ҫ�����ڲ����ݣ����ⷵ������������ڲ����ݵľ����
    class String {
         char* buffer_; public:
         char* GetBufferO const { return buffer_;} //bad: should return const char*
         //... 
    };
 
43 ���ǵ�ʹ��Pimpl ��ʹ˽�г�Ա���ɷ����Ҳ��ɼ���
     class Widget {//... 
         public: 
             Widget& operator=( const Widget& );
         private: 
             struct Impl; 
             shared_ptr<lmpl> pimpl_;
     };
     Widget& Widget::operator=( const Widget& )
     { 
         shared_ptr<lmpl> temp( new lmpl(/*...*/) );
         //change temp->t1_ and temp->t2_; if it fails then throw, else commit using:
         pimpl_ = temp; 
         return *this;
     }
44 ���ȱ�д�ǳ�Ա����Ԫ����, ����������ָ��Ϊ�ǳ�Ա����Ԫ����
45 ����һ���ṩnew��delete
46 ����ṩ��ר�ŵ�new��Ӧ���ṩ���б�׼��ʽ����ͨ���͵غͲ��׳���
    void* operator new(std::size_t); //plain new
    void* operator new(std::size_t, std::nothrow_t) throw(); //nothrow new 
    void* operator new(std::size_t, void*); //in-place new
47 ��ͬ����˳����ͳ�ʼ������
48 �ڹ��캯�����ó�ʼ�����渳ֵ
49 �����ڹ��캯�������������е������⺯��
50 ����������������Ϊ����������ģ����߱����ҷ������
    50.1 �������ͨ��ָ�����base��ָ��ִ��ɾ����������base���������������ǹ���������ģ�������Ǳ����ҷ�����ġ�
51 �����������ͷźͽ������Բ���ʧ��
52 һ�µĽ��и��ƺ�����
    52.1 ���ƹ��캯�������Ƹ�ֵ��������������������ͬʱ���֡�
53 ��ʽ�����û��߽��ø���
54 ������Ƭ���ڻ����п����ÿ�¡���渴��
    class B {/*... */};
    class D: public B {/*... */};
    void Transmogrify( B obj); //oops: takes an object by value
    void Transubstantiate( B& obj) { //ok: takes a reference 
        Transmogrify( obj); //oops: slices the object
    }
    D d; 
    Transubstantiate( d );


    class B {//... 
        public:
            B* Clone() const { //nonvirtual
                B* p = DoClone();
                 assert( typeid(*p) == typeid(*this) && "DoClone incorrectly overridden" );
                 return p; //check DoClone's returned type
            }
        protected: 
            B( const B& );
        private: 
            virtual B* DoClone() const = 0;
    };
 
55 ʹ�ø�ֵ�ı�׼��ʽ
    T& operator=( const T& ); //classic
    T& operator( T ); //potentially optimizer-friendly (see Item 27)
56 ֻҪ���У����ṩ����ʧ�ܵ�swap������Ҫ��ȷ���ṩ��
57 �����ͼ���ǳ�Ա�����ӿ�����ͬһ�����ռ䣨argument-dependent lookup ��ADL,�����������ң�
58 Ӧ�ý����ͺͺ����ֱ����ڲ�ͬ�������ռ��У��������������ǹ�����һ��
    #include <vector>
    namespace N { 
        struct X {};
        template<typename T>
        int* operator+( T, unsigned ) {/* do something */} 
    }
    int main() {
        std::vector<N::X> v(5);
        v[0];
    }
59 ��Ҫ��ͷ�ļ�����#include֮ǰ��д�����ռ�using
60 Ҫ�����ڲ�ͬ��ģ���з�����ͷ��ڴ棨����ָ��shared_ptr��
61 ��Ҫ��ͷ�ļ��ж���������ӵ�ʵ��
    //avoid defining entities with external linkage in a header
    int fudge Factor;
    string hello("Hello, world!");
    void foo() {/*... */}

    extern int fudgeFactor;
    extern string hello;
    void foo(); // "extern" is optional with function declarations

    The actual definitions go in a single implementation file:
    int fudgeFactor;
    string hello("Hello, world!"); 
    void foo() {/*... */}
62 ��Ҫ�����쳣��Խģ��߽紫��
63 ��ģ��Ľӿ���ʹ�þ������õĿ���ֲ�Ե�����
    void Translate( const char* src, char* dest, size_t destSize );
64 ���ǵĽ�Ͼ�̬��̬�ԺͶ�̬��̬��(ģ��ͼ̳е�ʹ��)
65 ����Ľ�����ʾ�Զ��壨������Կ�ģ������鼮��
66 ��Ҫ�ػ�����ģ��
67 ��Ҫ����ı�д��ͨ�õĴ���
    67.1 ʹ�� != ���� < �Ե������ıȽϲ���
    67.2 ʹ�õ���������������
    67.3 ʹ��empty ����size() == 0
    67.4 ʹ�ò�νṹ����ߵ����ṩ��Ҫ�Ĺ���
68 �㷺��ʹ�ö��Լ�¼�ڲ�����Ͳ���ʽ ASSERT
69 ��������Ĵ�������ԣ����ϸ�����
    69.1 ģ��Ĵ���Ӧ�þ����ڱ�ģ�鴦��
70 �������ͷǴ���, �����㺯����ǰ�������󣬺���������ȶ��Ǵ���
71 ��ƺͱ�д����ȫ�룬��֤��������ʧ��
72 ����ʹ���쳣�������
73 ͨ��ֵ�׳���ͨ�����ò���
74 ��ȷ�ر��桢�����ת������
75 ����ʹ���쳣�淶
76 Ĭ��ʹ��vector������ѡ���������ʵ�����
77 ��vector��string��������
78 ʹ��vector��string::c_str���C++ API��������
79 ��������ֻ�洢ֵ������ָ��
80 ��push_back����������չ���еķ�ʽ
81 ���÷�Χ���������õ�Ԫ�ز��� vector��insert
82 ʹ�ù��ϵĹ��÷�������ѹ��������swap����������ɾ��Ԫ�أ�erase_remove��
83 ʹ�ô�����STLʵ�֣� STL���㷨һ��Ҫ��鴫��������ĵ������Ƿ�����ʹ�ã������Ƿ�Խ��
84 ���㷨���ô����ֹ���д��(lambda���ʽ)
85 ʹ����ȷ��STL�����㷨������find/find_if��count/count_if������lower_bound/upper_bound/equal_range/binary_search��
86 ʹ����ȷ����STL�����㷨��������С���� partition()->stable_partition()->nth_element()->partial_sort()->sort()->stable_sort() ��
87 ʹν�ʳ�Ϊ��������ν�ʾ��Ƿ���ֵΪ bool ���͵ĺ������� ���治Ӧ�����κθı��Ա�����Ĳ�����
88 �㷨�ͱȽ����Ĳ������ú����������ú���
    struct IsHeavy : unary_function<Thing, bool> { 
        bool operator()( const Thing& ) const {/*...*/}
    };
    find_if( v.begin(), v.end(), notl( IsHeavy() )); //ok: adaptable

    struct CompareThings : public binary_function<Thing,Thing,bool> { 
        bool operator()( const Thing&, const Thing& ) const;
    }
    set<Thing, CompareThings> s; 

    struct FunctionObject: unary_function<Thing, bool> {
        bool operator()( const Thing& ) const {/*...*/} 
    };
    find_if(v.begin(), v.end(), FunctionObject());
89 ��ȷ��д�������� �������̳���unary_function��binary_function��
90 ����ʹ�����ͷ�֧����ʹ�ö�̬
    class Shape {//...
       virtaual void Draw() const = 0;
    };

    template<Class S>
    void Draw(const S& shape) {
        shape.Draw();
    };


91 �������ͣ��������﷽ʽ
    91.1 ������֤��2Ϊ����
    91.2 ������֤��2�Ĳ�����ʽ��ʾ
    91.3 ��Ա��ɸ�������˳��洢

    91.4 int����ǡ����32λ
    91.5 ָ���int��С��������ȣ��������⻥��
    91.6 ���Ա֮�������Ϊ������ڼ�϶
    91.7 ����ܻ��������ֶ�
    91.8 ���ڶ������⣬�޷��������ڴ�д��������󣬼�ʹ���㹻���ڴ�
    91.9 ����ָ���������ת�����������ͣ�ֵ�޷�Ԥ��
92 ����ʹ��reinterpret_cast
     If you lie to the compiler, it will get its revenge.

     Tl* p1 = ...;
     T2* p2 = reinterpret_cast<T2*>( p1 );
     
     should write: 
     Tl* p1 = ... ;
     void* pV = p1;
     T2* p2 = static_cast<T2*>( pV )
93 �����ָ��ʹ��static_cast������dynamic_cast�滻��

94 ����ǿ��ת��const_cast
     // ����
     const Object& f( const Object);
     Object& f( Objects obj) {
         const Objects ref = obj;
         return const_cast<Object&>( foo(ref)); //have to const_cast the return typ
     }
95 ��Ҫʹ��c����ǿ��ת��
96 ��Ҫ�Է�POD����memcpy��������memcmp����
97 ��Ҫʹ���������½��ͱ�ʾ��ʽ
     long intValue_;
     char* pointerValuej };
     pointerValue_ = somePointer; 
     long int gotcha = intValue_;
98 ��Ҫʹ�ÿɱ䳤������...��
99 ��Ҫʹ��ʧЧ���󣬲�Ҫʹ�ò���ȫ����
    Don't use the unsafe C legacy: strcpy, strncpy, sprintf, or any other functions that do write to range-unchecked buffers, 
    and/or do not check and correctly handle out-of-bounds errors. 
100 ��Ҫ��̬�Ĵ�������