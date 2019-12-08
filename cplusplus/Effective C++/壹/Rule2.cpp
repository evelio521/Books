   /* 尽量以const,enum,inline代替#define
  /*
   * 尽量用const double Aspect_ratio 1.653;代替 #define ASPECT_RATIO 1.653;
   * 因为 const double Aspect_ratio 1.653; 一定会被编译器看到记录到符号表而
   * #define ASPECT_RATIO 1.653;则不一定。
   */
 	
 	#define ASPECT_RATIO 1.653;//不是我们我想要的形式
  const double Aspect_ratio 1.653;//尽量使用这种形式
  
  
  /*
   * 定义常量指针，因为常量的定义长放在头文件中，所以指针定义为const
   * class专属常量的定义
   
    const char * const authorName1="Scott Meyers";
    const std::string authorName2="Scott Meyers";//比较常用
   
   */
   //新的编译器
   class GamePlayer1{
   	static const int Numturns=5;//声明式
   	int scores[Numturns];//使用该常量
   	
  };
  //老的编译器
  class GamePlayer2{
   	static const int Numturns;//声明式
   	int scores[Numturns];//使用该常量
   	
  };
  const int Numturns=1.653;
  
  //没有 所谓的 private #define这种声明
  
  
  
  //获取一个const常量的地址或引用是合法的 ，但是获取一个enum的地址或引用是不合法的，获取一个#define 也是不合法的
  class GamePlayer3{
   	enum { Numturnes=5};//Numturnes只是5的一个记号
   	int scores[Numturns];
   	
  };
  
  
  //尽量不要用#define调用函数，要用template 
  #define CALL_MAX(a,b) f( (a)>(b)? (a):(b))
  	
  int a=5,b=0;
  CALL_MAX(++a,b);//a累加2次
  CALL_MAX(++a,b+10)//a累加1次
  
  tempalte <typename T>
  inline void  CallMax(const T& a,const T& b)
  {
  	f(a>b?a:b);
  }
  
  //注意 #include #ifdef/#ifndef仍是必备品
  