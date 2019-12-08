/*尽量延后变量定义出现的时间*/

void encrypt(std::string& s);//在其中适当的地点为其加密

//以下程序过早的定义encrypted
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
	
	//比较好的定义方式
	std::string encryptPasswrod(const std::dtring& password)
	{
		if(password.length()<MinumumPasswordLength){
			throw logic_error("password is too short");
		}
		string encrypted(password);
		encrypt(encrypted);
		return encrypted;
	}
	
	
	
	//A定义于循环外                     //B定义于循环内  
	 Widget w;
	 for(int i=0;i<n;i++){              for(int i=0;i<n;i++){
	 	 w=取决于i的某个值;                   Widget w(取决于i的某个值);
	 	 ...                                   ...
	 	}                                    }
	 	
	 	//做法A的代价 1个构造函数+1个析构函数+n个赋值函数
	 	//做法B的代价 n个构造函数+n个析构函数
	 	//一般情况下我们认为B较好