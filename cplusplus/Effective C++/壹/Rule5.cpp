/*  �˽�c++ ĬĬ��д��������Щ����*/

 class Empty1{
};
 //�����������ͬ
 class Empty2{
 	
 	public:
 		Empty(){  }
 		Empty(const Empty& rhs){  }
 		~Empty(){  }
 		
 		Empty& operator=(const Empty&rhs){   }
};

template<class T>
  class NameObject1{
  	public:
  		NameObject(string name,const T& value):nameValue(name),objectValue(value)
  			{
  			}
  		
  	private:
  		string nameValue;//�������� reference
  		 T objectValue;//������Ϊconst
  			
  };
//������� һЩ���������ᱻ����
  template<class T>
  class NameObject{
  	public:
  		NameObject(string name,const T& value):nameValue(name),objectValue(value)
  			{
  			}
  		
  	private:
  		string& nameValue;//�������� reference
  		const T objectValue;//������Ϊconst
  			
  };
  
  
 
  string newDog("Persephone");
  string oldDog("Satch");
  NameObject<int> p(newDog,2);
  NameObject<int> s(oldDog,36);
  
  p=s;//������������ c++������referenceָ��ͬ�Ķ���
  
  NameObject1<int> p1(newDog,2);
  NameObject1<int> s1(oldDog,36);
  
  p1=s1;//��������������
 
  
  