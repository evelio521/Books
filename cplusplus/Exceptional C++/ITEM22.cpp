ITEM22:classes之间的关系
	//下面的代码有需要改进的吗？
	class BasicProtocol /* : possible base classes */ 
	{
	public:
	  BasicProtocol();
	  virtual ~BasicProtocol();//不是设计用来被其他类继承的，virtual没有必要
	  bool BasicMsgA( /*...*/ );
	  bool BasicMsgB( /*...*/ );
	  bool BasicMsgC( /*...*/ );
	};
	
	class Protocol1 : public BasicProtocol
	{
	public:
	  Protocol1();
	  ~Protocol1();
	  bool DoMsg1( /*...*/ );
	  bool DoMsg2( /*...*/ );
	  bool DoMsg3( /*...*/ );
	  bool DoMsg4( /*...*/ );
	};
	
	class Protocol2 : public BasicProtocol
	{
	public:
	  Protocol2();
	  ~Protocol2();
	  bool DoMsg1( /*...*/ );
	  bool DoMsg2( /*...*/ );
	  bool DoMsg3( /*...*/ );
	  bool DoMsg4( /*...*/ );
	  bool DoMsg5( /*...*/ );
	};
	//每一个DoMsg...() member function 都呼叫
	//BasicProtocol::Basic...()执行共同的工作，
	//然后DoMsg...()另外执行传送工作。
	
	//常见错误: 绝不要使用public inheritance ，除非你要塑膜出真正的IS-A 和LIKE-A 关系。
	        // 所有被改写的member functions不能要求更多也不能承诺更少。
	        
  //设计准则:绝不要以public inheritance复用base class内的程式码，public
          // inheritance是为了被复用--被那些以多型方式运用object的程式码复用
       1.BasicProtocol没有提供任何虚拟函数函数式，这就意味着他不希望以多型的凡是被使用
         这对public inheritance是一个强烈的反对暗示
       2.BasicProtocol没有任何protected members，这就意味着没有继承接口，这对任何继承型式
         (public ,private)都是一个强烈的反对暗示
       3.BasicProtocol封装了共同的工作，但是他不像derived class那样执行自己的传送工作。
         这就意味着BasicProtocol物件运行起来并非像一个LIKE-A衍生的protocol物件
         也非可视为IS-A 衍生的protocol物件。public inheritance 应该只能用来塑膜唯一一件事情
         一个真正的IS-A接口关系。
       4.所有衍生的classes只是用BasicProtocol的public接口。这就意味着他并没有因为自己是
         derived class而受益，他们的工作可以轻易以一个型别为BasicProtocol的辅助物件完成
    
    
    //设计准则: 当我们希望塑膜出 is implemented in terms of 的关系时，请选择membership/aggregation
               //而不要使用inheritance。只有在绝对必要下才使用private inheritance
               //也就是说当你需要存取protected members或是需要改写虚拟函数式时，绝对不要为了复用程序式的代码
               //使用public inheritance
               
               
    