/*Ϊ��̬��������virtual��������*/

//Ϊ������virtual��������ʱ���Լ���"�ֲ�����"����
 class TimeKeeper{
 	
 	public:
 		TimeKeeper(){}
 		virtual ~TimeKeeper(){}
 		
};
clasee Derived:public TimeKeeper{
	
	public:
		Drived(){}
		~Derived(){};
};
//�κ�һ��classֻҪȷ������virtual����������ȷ��Ӧ����һ��virtual��������

/*
 * ���һ��class����virtual������˵�������뱻����һ�����ࡣ�����뱻��������ʱ��
 * ����virtual�������������Ǹ������⡣
 * 
 *
 */
 
 
 //�ĵã�ֻ�е�class�����ٺ���һ��virtual����ʱ��������virtual��������
 
 
 	TimeKeeper *tk=new Derived();
 	
 	delete tk;//���TimeKeeperû��virtual  ��ôDerived�Ĳ��ֿ���û�б�����
 	
