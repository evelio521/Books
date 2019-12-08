ITEM6:������ָ��Ͳ�������������
	//1. �������
	vector<char> v;
	// ... populate v ...
	
	char* p = &v[0];
	
	// ... do something with *p ...
	
	//��������˵��ʹ�õ������ĵط�������ʹ��ָ�룬Ȼ����ʱ��ʹ��ָ��ĵط�
	//��һ����ʹ�õ�����
	
	//ʹ�õ�������ȱ��
	1.��ʹ��ָ��ĵط���һ����ʹ�õ�����
	2.�����������һ�����������һ����ָͨ�룬ʹ���������¿ռ�����ܵĿ���
	
	//������һ��map<Name,PhoneNumber> ���ڳ�������ʱ���� �˺�ֻ���ڲ�ѯ��
	//Ҳ���Ǹ���һ���������Ǻ������ѯ���绡���롣����Ҫ���з����ѯ���ǰ죿
	//һ���򵥵ķ��������ڹ���һ�����ݽṹ map<PhoneNumber*,Name*,Deref>
	//����������˫���Ŀ�������Ϊ�ڶ���ӵ�е�ֻ��ָ��
	
	//��������õ��������޷��쵽��
	
	//2.����Ĵ���Ϸ��𣿰�ȫ�� ����
	
	vector<char> c;
	//....���v....
	char* p=&v[0];
	//....ʹ��*p.....
	
	
	//��δ�����ȫ�Ϸ�
	//��δ����ǰ�ȫ�� ֻҪ����֪��ָ��ʲôʱ��ʧЧ
	//��δ�����������ģ�ӵ��һ��ָ�������ڲ���ָ���Ǿ����������
	
	
	//�Դ���ĸĽ�
	vector<char> c;
	//....���v....
	vector<char>::iterator i=c.begin();
	//....ʹ��*i.....
	
	//��������˵��ʹ�õ������ĵط�������ʹ��ָ�룬Ȼ����ʱ��ʹ��ָ��ĵط�
	//��һ����ʹ�õ�����
	
	//ʹ�õ�������ȱ��
	1.��ʹ��ָ��ĵط���һ����ʹ�õ�����
	2.�����������һ�����������һ����ָͨ�룬ʹ���������¿ռ�����ܵĿ���
	
	//������һ��map<Name,PhoneNumber> ���ڳ�������ʱ���� �˺�ֻ���ڲ�ѯ��
	//Ҳ���Ǹ���һ���������Ǻ������ѯ���绡���롣����Ҫ���з����ѯ���ǰ죿
	//һ���򵥵ķ��������ڹ���һ�����ݽṹ map<PhoneNumber*,Name*,Deref>
	//����������˫���Ŀ�������Ϊ�ڶ���ӵ�е�ֻ��ָ��
	
	
	//3. �������Ϸ���
	template<typename T>
	void f( T& t )
	{
	  typename T::value_type* p1 = &t[0];
	  typename T::value_type* p2 = &*t.begin();
	  // ... do something with *p1 and *p2 ...
	}
	
	//�Ϸ�
	
	1.To make the expression &t[0] valid, T::operator[]() 
		must exist and must return something that understands
		operator&(), which in turn must return a valid T::value_type*
	 (or something that can be meaningfully converted to a valid 
	 T::value_type*).

   In particular, this is true of containers that meet the standard��s
    container and sequence requirements and implement the optional 
    operator[](), because that operator must return a reference to 
    the contained object. By definition, you can then take the 
    contained object��s address.

   2.To make the expression &*t.begin() valid, T::begin() must exist, 
   	and it must return something that understands operator*(), 
   	which in turn must return something that understands operator&,
   	which in turn must return a valid T::value_type* (or something 
   	that can be meaningfully converted to a valid T::value_type*).

    //���ڵ�������:�Ա�׼���е�֧��operator[]()���κ�������˵�������
    //���붼�ǺϷ��ģ����ȥ��&t[0]������䣬�Ա�׼���ÿ������Ҳ�ǳ�����
    //Ψ��std::vector<bool>����
    
	 //����Ĵ����bool�ⶼ����
		// Example 6-3: Also works for every T except bool
		//
		template<typename T>
		void g( vector<T>& v )
		{
		  T* p = &v.front();
		  // ... do something with *p ...
		}
		//vector<bool>���ػ�������������һ�������������ϱ�׼�������
		
		
		//����������Ż�
		1.��Ҫ̫���Ż�
		2.����ȷʵ��Ҫ������Ҫʹ���Ż�
		3.��ʹ����������֪����Ҫ�Ż�ʲô��������Ҫ�Ż�������Ҫ�Ż�
		
		//vector<bool>���ػ��汾 �����c++ STL��MSDN