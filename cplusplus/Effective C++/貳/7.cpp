class TimeKeeper{
 	
 	public:
 		TimeKeeper(){}
 		virtual ~TimeKeeper(){}
 		
};
class Derived:public TimeKeeper{
	
	public:
		Derived(){}
		~Derived(){};
};
//�κ�һ��classֻҪȷ������virtual����������ȷ��Ӧ����һ��virtual��������

/*
 * ���һ��class����virtual������˵�������뱻����һ�����ࡣ�����뱻��������ʱ��
 * ����virtual�������������Ǹ������⡣
 * 
 *
 */
 
 //��ʱ�������붨��һ�������� ����������û��virtual������ʱ�� ���Խ�����������Ϊ���麫ʽ
class AWOV{
public:
	virtual ~AWOV()=0;

};
AWOV::~AWOV()
{
}
 //�ĵã�ֻ�е�class�����ٺ���һ��virtual����ʱ��������virtual��������
 
 int main(){
 	
 TimeKeeper *tk=new Derived;
 
 delete tk;	
}