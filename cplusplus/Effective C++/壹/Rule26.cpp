/*�����Ӻ����������ֵ�ʱ��*/

void encrypt(std::string& s);//�������ʵ��ĵص�Ϊ�����

//���³������Ķ���encrypted
std::string encryptPasswrod(const std::dtring& password)
	{
		using namespace std;
		string encrypted;
		if(password.length()<MinumumPasswordLength){
			throw logic_error("password is too short");
		}
		encrypt(encrypted);
		return encrypted;
	}
	
	//�ȽϺõĶ��巽ʽ
	std::string encryptPasswrod(const std::dtring& password)
	{
		if(password.length()<MinumumPasswordLength){
			throw logic_error("password is too short");
		}
		string encrypted(password);
		encrypt(encrypted);
		return encrypted;
	}
	
	
	
	//A������ѭ����                     //B������ѭ����  
	 Widget w;
	 for(int i=0;i<n;i++){              for(int i=0;i<n;i++){
	 	 w=ȡ����i��ĳ��ֵ;                   Widget w(ȡ����i��ĳ��ֵ);
	 	 ...                                   ...
	 	}                                    }
	 	
	 	//����A�Ĵ��� 1�����캯��+1����������+n����ֵ����
	 	//����B�Ĵ��� n�����캯��+n����������
	 	//һ�������������ΪB�Ϻ�