/*���ó�Ա����ģ��������м�������*/

class Top{
};
class Middle:public Top{
	
};
class Bottom:public Middle{
	
};
Top* pt1=new Middle;//Middle*ת��Top*
Top* pt2=new Bottom;//Bottom*ת��Top*

const Top* pct2=pt1;//Top*ת��const Top*

//
 template <typename T>
 class SmartPtr{//����ָ��
 	public:
 		explicit SmartPtr(T* realPtr);
};

 SmartPtr<Top> pt1=SmartPtr<Middle>(new Middle);//SmartPtr<Top> ת��ΪSmartPtr<Middle>
 SmartPtr<Top> pt2=SmartPtr<Bottom>(new Bottom);//SmartPtr<Top> ת��ΪSmartPtr<Bottom>
 SmartPtr<const Top> pt2=pt1;//SmartPtr<Top> ת��Ϊ SmartPtr<const Top>
 
 //��������ĺ��� ���캯���޷���� ���ǲ���һ������
  template<typename T>
  class SmartPtr{
  	public:
  		template<typename U>
  		SamrtPtr(const SmartPtr<U>& other);
  		...
  };//�޷���ֹSmartPtr<double>��SmartPtr<int>��ת��
  
  //�����������������ı�
  template<typename T>
  class SmartPtr{
  	public:
  		template<typename U>
  		SamrtPtr(const SmartPtr<U>& other):heldPtr(other.get())
  			{
  			}
  		T* get() const {return heldPtr;}
  	private:
  		T* heldPtr;
  		...
  };
 