/*����pass-by-reference-to-const���pass-by-value*/

//���д��뽫��ǳ���ʱ
class Person{
	
	public:
		Person();
		virtual ~Person();
		...
	private:
		std::string name;
		std::string address;
};
class Student:public Person{
	
	public:
		Student();
		~Student();
		...
	private:
		std::string schoolName;
		std::string schoolAddress;
};

bool validStudent(Student s);
Student plato;
bool platoIsOK=validStudent(plato);
//�������һ��Student copy���캯����һ��Person Copy���캯�����Ĵ�string copy���캯��

//�������
bool validStudent(const Student& s);//�÷��������Է�ֹ���͵Ľض�
