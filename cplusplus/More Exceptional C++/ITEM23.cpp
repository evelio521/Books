ITEM23:�쳣��ȫ��������֮��:�̳�
	//1.Is-Implemented-In-Terms-Of �ĺ�����ʲô��
	
	//���T������ʵ������ĳ����ʽʹ������һ������U���ͳ�T Is-Implemented-In-Terms-Of U��
	//��ĳ����ʽʹ����һ��ǵ�Ȼ���кܴ���أ�����ʾ�ķ�Χ�ܹ㣬��T������U��һ��������
	//�������װ��;����T����ֻ��������ʵ��ϸ����ż���õ�U��
	
	//T IIITO Uͨ����ζ��:Ҫô��T��һ��U ����:
	// Example 23-1(a): "T IIITO U" using Has-A
	//
	class T
	{
	  // ...
	private:
	  U* u_;  // or by value or by reference
	};
	//ҪôT�ǹ���������U������
	// Example 23-1(b): "T IIITO U" using derivation
	//
	class T : private U
	{
	  // ...
	};
		
		
	//2.��C++�У�Is-Implemented-In-Terms-Of����ͨ���ǹ��м̳л����/ί�������
	//������˵����дһ����T��ʱ�������Ҫ����U��ʵ�֡�������Ҫ��ѡ����:
	//��T��U˽�м̳У�������T����һ��U��Ա����

	//�̳����ױ�ʹ�ù��ȣ�һ����Ч�Ĺ������ԭ��:������Խ�����ͣ����һ�ֹ�ϵ�����ö�����Ч��ʽ����
	//��ʹ�ù�ϵ��������һ������Ȼ�̳н��ƺ���C++�п��Ա������ǿ�ҵĹ�ϵ����������Ԫ����ô��ֻ����
	//û�и����Ĺ�ϵʱ�����ǲ�ʹ������
	
	//��С�����ԭ�����ɻ�ֱ��Ӱ�쵽����Ľ�׳�ԣ�����ʱ�䣬�Լ������ɼ������
	//��Ȥ���ǣ�Ϊ��ʵ��IIITO ,�ڼ̳к�ί��֮��������ѡ�񻹻����쳣��ȫ���ϵ�ǣ��
	//
	
	
	//���쳣��ȫ��Ӱ��
	//�������Has-A������IIITO��ϵ������Ӧ������дT::operator=()��
	// Example 23-2(a): "T IIITO U" using Has-A
	//
	class T
	{
	  // ...
	  private:
	    U* u_;
	};
	//����ǿ�Ұ�ȫ�Ե�T::operator=()
	T& T::operator=( const T& other )
	{
	   U* temp = new U( *other.u_ );   // do all the work
	                                   //  off to the side
	
	   delete u_;      // then "commit" the work using
	   u_ = temp;      //  nonthrowing operations only
	   return *this;
	}


  //һ��U��T֮��Ĺ�ϵ�漰���κη�ʽ�ļ̳У��������ʲô�仯
  // Example 23-2(b): "T IIITO U" using derivation
	//
	class T : private U
	{
	  // ...
	};
	T& T::operator=( const T& other )
	{
	  U::operator=( other );  // ???��������ڿ�ʼ�޸�Ŀ���������׳��쳣
	  //�����޷�д������ǿ�Ұ�ȫ�Ե�T::operator=(),����Uͨ������ĳ�������ṩ�˺��ʹ���
	  return *this;
	}
	//���仰˵�����TΪ���ĳ�Ա����T::operator=()�ṩ�쳣��ȫ��֤��������Ȼ
	//������U�İ�ȫ�ͱ�֤���ڱ���T��U֮��Ĺ�ϵʱ����������ʹ���������ܵĽ��ܹ�ϵ
	//�Ӷ����������ܵĸ�����ԡ�


  //�ܽ�:
  //��ɢ������Դٽ��������ȷ��(�����쳣��ȫ��)�����ܵ�����Խ��ͳ����������
  //��ȷ��(�����쳣��ȫ)