   /* ������const,enum,inline����#define
  /*
   * ������const double Aspect_ratio 1.653;���� #define ASPECT_RATIO 1.653;
   * ��Ϊ const double Aspect_ratio 1.653; һ���ᱻ������������¼�����ű��
   * #define ASPECT_RATIO 1.653;��һ����
   */
 	
 	#define ASPECT_RATIO 1.653;//������������Ҫ����ʽ
  const double Aspect_ratio 1.653;//����ʹ��������ʽ
  
  
  /*
   * ���峣��ָ�룬��Ϊ�����Ķ��峤����ͷ�ļ��У�����ָ�붨��Ϊconst
   * classר�������Ķ���
   
    const char * const authorName1="Scott Meyers";
    const std::string authorName2="Scott Meyers";//�Ƚϳ���
   
   */
   //�µı�����
   class GamePlayer1{
   	static const int Numturns=5;//����ʽ
   	int scores[Numturns];//ʹ�øó���
   	
  };
  //�ϵı�����
  class GamePlayer2{
   	static const int Numturns;//����ʽ
   	int scores[Numturns];//ʹ�øó���
   	
  };
  const int Numturns=1.653;
  
  //û�� ��ν�� private #define��������
  
  
  
  //��ȡһ��const�����ĵ�ַ�������ǺϷ��� �����ǻ�ȡһ��enum�ĵ�ַ�������ǲ��Ϸ��ģ���ȡһ��#define Ҳ�ǲ��Ϸ���
  class GamePlayer3{
   	enum { Numturnes=5};//Numturnesֻ��5��һ���Ǻ�
   	int scores[Numturns];
   	
  };
  
  
  //������Ҫ��#define���ú�����Ҫ��template 
  #define CALL_MAX(a,b) f( (a)>(b)? (a):(b))
  	
  int a=5,b=0;
  CALL_MAX(++a,b);//a�ۼ�2��
  CALL_MAX(++a,b+10)//a�ۼ�1��
  
  tempalte <typename T>
  inline void  CallMax(const T& a,const T& b)
  {
  	f(a>b?a:b);
  }
  
  //ע�� #include #ifdef/#ifndef���Ǳر�Ʒ
  