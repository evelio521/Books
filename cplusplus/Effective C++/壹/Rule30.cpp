/*͸���˽�inlining����������*/

class Person{
	
	public:
		...
		int age() const//һ��������inline����
		{
			return theAge;
		}
		...
	private:
		int theAge;
};
//ͨ������inline�������������ں���ǰ����inline
 template <typename T>
 inline const T& std::max(const T&a,const T&b)
 	{
 		return a<b?b:a;
 		}
 		
 	//	Inline����һ�����ͷ�ļ���
 	//	templateһ�����ͷ�ļ���
 	//
 	//inline�����ĵ��ÿ��ܱ�inlinedҲ���ܲ���inlined
 	
 	//������ͨ������"ͨ������ָ������еĵ���"ʵʩinlining
 	
 	inline void f() {...}
 	void (*pf)()=f;
 	
 	...
 	f();//������ñ�inlined
 	pf();//������û�����inlined
 	
 	//���캯��������������Ӧ����inline