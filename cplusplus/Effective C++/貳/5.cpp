#include<iostream>

class Empty{
	public:
		//����ĺ���������Լ����� ��ô����������������
		//Ĭ�Ϲ��캯��
		Empty(){
		}
		//copy ���캯��
		Empty(const Empty& rhs){
		
		}
		//��������
		~Empty(){
		}
		//copy assignment������
		Empty& operator=(const Empty& rhs){
			return *this;
		}
};
template<typename T>
class NameObject{
	
	public:
	NameObject(std::string name,const T& value):nameValue(name),ObjectValue(value){}
  private:
  	//�������಻��Ĭ�ϵ��ṩoperator= ��ΪC++������referenceָ��ͬ�Ķ��� ����const Ҳ���Ϸ�
  	//����������� ������Լ�����copy assignment 
  	std::string& nameValue;
  	const T ObjectValue;
};
int main()
{
	Empty e1;//Ĭ�Ϲ��캯��
					//��������
	Empty e2(e1);//copy���캯��
	e2=e1;//copy assignment ������
	
	std::string newDog("perry");
	std::string oldDog("stach");
	
	NameObject<int> p(newDog,2);
	NameObject<int> s(oldDog,36);
	
	//p=s; ������÷�
	
	
	
	return 0;
}