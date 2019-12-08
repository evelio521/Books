// 0 不要拘泥于小节
    0.1 不要规定缩进多少，应该规定使用缩进来体现代码风格
    0.2 不要强制行的具体长度，保持代码有利于阅读
    0.3 不要在命名方面规定过多，应该规定统一的命名规范
    0.4 不要规定注释体例（除非要提取文档）
    0.5 函数尽量简单短小和易于理解
1 在高警告级别干净利落的进行编译，不放过任何警告
    1.1 消除第三方库的警告
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
    1.2 处理未使用的函数参数
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
    1.3 处理定义未使用的变量
    // warning: "variable lock' is defined but never used" 
    void Fun() { 
        Lock lock;
    }
    // new version: probably eliminates warning 
    void Fun() {
        Lock lock;
        lock；
    }
    1.4 变量使用前要初始化
    1.5 防止遗漏return语句
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
    1.6 有符号数和无符号数不要比较，不匹配
2 使用自动构建系统
    2.1 无任何警告，一键构建
3 使用版本控制系统VCS
4 在代码审查上投入
5 一个实体应该只有一个紧凑的职责（一次只解决一个问题）
    5.1 只给一个实体赋予一个特定的良好职责。
6 正确、简单和清晰第一
    6.1 KISS 正确优于速度 简单优于复杂。 清晰优于机巧 安全优于不安全。
    6.2 不要使用不必要的操作符重载
    6.3 构造函数的参数不要使用临时变量
7 编程中应知道何时和如何考虑可伸缩性（线性算法优先）
    7.1  动态分配数据，不要使用固定大小的数组
    7.2 了解算法的复杂性
    7.3 优先使用线性算法或尽可能快的算法
    7.4 永远不要使用指数复杂性算法
8 不要进行不成熟的优化
    8.1 让一个正确的程序更快速，比让一个快速的程序正确要容易的多
9 不要进行不成熟的劣化
    9.1 能传引用不传值
    9.2 能使用前缀的++ 不实用后缀的--
    9.3 尽量使用初始化列表
10 尽量减少全局和共享数据

11 隐藏信息（不要公开提供抽象实体的内部信息）

12 懂得何时和如何进行并发性编程（多线程），尽量不要用锁设计

13 确保资源为对象所拥有，使用显式的RAII和智能指针
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
14 宁要编译时和连接时错误，也不要运行时错误
   14.1 编译时bool条件使用断言
   14.2 编译时多态，考虑使用模版代替虚函数
   14.3 常量尽量使用枚举
   14.4 dynamic_cast 使用较多建议重新设计接口
15 积极使用const
   15.1 尽量不要强制转换const
    void Fun( int x );
    void Fun( const int x ); //redeclares the same function: top-level const is ignored

    void Fun( const int x ) { //Fun's actual definition
        ++x; // error: cannot modify a const value
    }
16 避免使用宏
    16.1 运行阶段尽量使用assert
17 避免使用“魔数”（3.1415等）
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
18 尽可能局部的声明变量
19 总是初始化变量,否则会带来无穷的灾难
20 避免函数过长，避免嵌套过深
    短胜于长 平优于深
    20.1 尽量紧凑，一个函数只赋予一个职能
    20.2 不要自我重复，不要让相似的代码重复出现
    20.3 优先使用&& 用&&代替嵌套循环
    20.4 优先使用标准算法
    20.5 不要根据标签类型进行分支switch
21 避免跨编译单元的初始化， 如果不可避免，使用Singleton
22 尽量减少定义性依赖，避免循环依赖
    22.1 能使用前向声明，不要使用include
23 头文件应该自给自足，独立编译每一个头文件
24 总是编写内部#include保护符，绝不要编写外部 #include保护符（条件编译）
    #ifndef FOO_H_INCLUDED_ 
    #define FOO_HJNCLUDED_ 
    //... contents of the file... 
    #endif
25 正确地选择通过值、(智能)指针或者引用传递参数
    对于只输入参数
    25.1 始终使用const限制指向输入参数的指针或引用
    25.2 优先通过值来取得原始类型和复制开销比较低的值对象的输入
    25.3 优先使用const的引用取得其他用户定义类型的输入
    25.4 如果参数需要副本，则使用传值的方式
    输出参数
    25.1 参数是可选的，优先使用指针传递
    25.2 参数是必须的，优先使用引用传递
26 保持重载操作符的自然语义
27 优先使用算术操作符和赋值操作符的标准形式
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
28 优先使用++和—的标准形式，优先使用前缀形式
 T& T::operator+ + ()               T& T::operator--() // the prefix form:
 {                                  { 
     //perform increment                //perform decrement // - do the work
     return *this;                      return *this; // - always return *this;
 }                                  }
 
 T T::operator++(int)               T T::operator—(int) //the postfix form: 
 {                                  {  
     T old(*this);                      T old( *this); // - remember old value
     ++*this;                           --*this; // - call the prefix version
     return old;                        return old; // - return the old value
 }                                  }
29 考虑重载以避免隐含类型转换
    bool operator==( const String& Ihs, const String& rhs); 
    bool operator==( const String& Ihs, const char* rhs); 
    bool operator==( const char* Ihs, const String& rhs);
30 避免重载&&、||、或，

31 不要编写依赖于函数参数求值顺序的代码
    int count = 5;
    Transmogrify( ++count, ++count); //order of evaluation unknown
32 弄清所要编写的是哪种类（如值类、基类、策略类、异常类）
33 用小类代替巨类
34 用组合代替继承
35 避免从并非要设计成基类的类中继承
    35.1 不要从string继承
    35.2 用组合代替公有继承或私有继承
    class localized_string {
         public:
           //... provide passthroughs to string member function that we want to 
           //retain unchanged (e.g., define insert that calls impl_.insert) ...
           void clear(); // mask/redefine clear() 
           bool is_in_klingon() const; // add functionality
        private:
        std::string impl_; //... add extra sta
    };

36 优先提供抽象接口（依赖倒置原理Dependency Inversion Principle，DIP）
    36.1 接口一定要正确，后期可能不允许修改
37 共用继承即可替换性，继承不是为了重用，而是为了被重用
    37.1 继承不能破坏正确性 Rectangle 和 Square就不能相互继承
38 实施安全的改写（永远不要修改默认参数，继承类改写的函数应该添加virtual（虽然是冗余的））
39 考虑将虚拟函数声明为非共用的，将共用函数声明为非虚拟的（non-virtaual interface,NVI）
40 要避免提供隐式转换，可以提供显示转换（explicit）或命名转换函数, 谨慎使用重载
41 将数据成员设为私有的，无行为的聚集（C语言struct）除外， 明智地使用Pimpl
42 不要公开内部数据（避免返回类所管理的内部数据的句柄）
    class String {
         char* buffer_; public:
         char* GetBufferO const { return buffer_;} //bad: should return const char*
         //... 
    };
 
43 明智地使用Pimpl （使私有成员不可访问且不可见）
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
44 优先编写非成员非友元函数, 尽量将函数指定为非成员非友元函数
45 总是一起提供new与delete
46 如果提供类专门的new，应该提供所有标准形式（普通、就地和不抛出）
    void* operator new(std::size_t); //plain new
    void* operator new(std::size_t, std::nothrow_t) throw(); //nothrow new 
    void* operator new(std::size_t, void*); //in-place new
47 以同样的顺序定义和初始化变量
48 在构造函数中用初始化代替赋值
49 避免在构造函数和析构函数中调用虚拟函数
50 将基类析构函数设为共用且虚拟的，或者保护且非虚拟的
    50.1 如果允许通过指向基类base的指针执行删除操作，则base的析构函数必须是公用切虚拟的，否则就是保护且非虚拟的。
51 析构函数、释放和交换绝对不能失败
52 一致的进行复制和销毁
    52.1 复制构造函数、复制赋值操作符和析构函数必须同时出现。
53 显式地启用或者禁用复制
54 避免切片，在基类中考虑用克隆代替复制
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
 
55 使用赋值的标准形式
    T& operator=( const T& ); //classic
    T& operator( T ); //potentially optimizer-friendly (see Item 27)
56 只要可行，就提供不会失败的swap（而且要正确的提供）
57 将类型及其非成员函数接口置于同一命名空间（argument-dependent lookup ，ADL,参数依赖查找）
58 应该将类型和函数分别置于不同的命名空间中，除非有意让他们工作在一起
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
59 不要在头文件或者#include之前编写命名空间using
60 要避免在不同的模块中分配和释放内存（智能指针shared_ptr）
61 不要在头文件中定义具有链接的实体
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
62 不要允许异常跨越模块边界传播
63 在模块的接口中使用具有良好的可移植性的类型
    void Translate( const char* src, char* dest, size_t destSize );
64 理智的结合静态多态性和动态多态性(模版和继承的使用)
65 有意的进行显示自定义（具体可以看模版相关书籍）
66 不要特化函数模板
67 不要无意的编写不通用的代码
    67.1 使用 != 代替 < 对迭代器的比较操作
    67.2 使用迭代代替索引访问
    67.3 使用empty 代替size() == 0
    67.4 使用层次结构中最高的类提供需要的功能
68 广泛地使用断言记录内部假设和不变式 ASSERT
69 建立合理的错误处理策略，并严格遵守
    69.1 模块的错误应该尽量在本模块处理
70 区别错误和非错误, 不满足函数的前条件错误，后条件错误等都是错误
71 设计和编写错误安全码，保证函数不会失败
72 优先使用异常报告错误
73 通过值抛出，通过引用捕获
74 正确地报告、处理和转换错误
75 避免使用异常规范
76 默认使用vector，否则，选择其他合适的容器
77 用vector和string代替数组
78 使用vector和string::c_str与非C++ API交换数据
79 在容器中只存储值和智能指针
80 用push_back代替其他扩展序列的方式
81 多用范围操作，少用单元素操作 vector的insert
82 使用公认的惯用法真正的压缩容量（swap），真正的删除元素（erase_remove）
83 使用带检查的STL实现， STL的算法一定要检查传入的容器的迭代器是否正常使用，索引是否越界
84 用算法调用代替手工编写的(lambda表达式)
85 使用正确的STL查找算法（无序find/find_if或count/count_if，有序lower_bound/upper_bound/equal_range/binary_search）
86 使用正确定的STL排序算法（开销从小到大 partition()->stable_partition()->nth_element()->partial_sort()->sort()->stable_sort() ）
87 使谓词成为纯函数（谓词就是返回值为 bool 类型的函数对象， 里面不应该有任何改变成员变量的操作）
88 算法和比较器的参数多用函数对象少用函数
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
89 正确编写函数对象 （尽量继承至unary_function或binary_function）
90 避免使用类型分支，多使用多态
    class Shape {//...
       virtaual void Draw() const = 0;
    };

    template<Class S>
    void Draw(const S& shape) {
        shape.Draw();
    };


91 依赖类型，而非其表达方式
    91.1 整数保证以2为基数
    91.2 负数保证以2的补码形式表示
    91.3 成员变成根据声明顺序存储

    91.4 int不会恰好是32位
    91.5 指针和int大小不总是相等，不能随意互换
    91.6 类成员之间可能因为对齐存在间隙
    91.7 类可能会有隐藏字段
    91.8 由于对齐问题，无法在任意内存写入任意对象，即使有足够的内存
    91.9 两个指针有序，如果转换成整数类型，值无法预测
92 避免使用reinterpret_cast
     If you lie to the compiler, it will get its revenge.

     Tl* p1 = ...;
     T2* p2 = reinterpret_cast<T2*>( p1 );
     
     should write: 
     Tl* p1 = ... ;
     void* pV = p1;
     T2* p2 = static_cast<T2*>( pV )
93 避免对指针使用static_cast（可用dynamic_cast替换）

94 避免强制转换const_cast
     // 例外
     const Object& f( const Object);
     Object& f( Objects obj) {
         const Objects ref = obj;
         return const_cast<Object&>( foo(ref)); //have to const_cast the return typ
     }
95 不要使用c风格的强制转换
96 不要对非POD进行memcpy操作或者memcmp操作
97 不要使用联合重新解释表示方式
     long intValue_;
     char* pointerValuej };
     pointerValue_ = somePointer; 
     long int gotcha = intValue_;
98 不要使用可变长参数（...）
99 不要使用失效对象，不要使用不安全函数
    Don't use the unsafe C legacy: strcpy, strncpy, sprintf, or any other functions that do write to range-unchecked buffers, 
    and/or do not check and correctly handle out-of-bounds errors. 
100 不要多态的处理数组