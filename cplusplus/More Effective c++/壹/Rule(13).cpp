����:13
	/*��by reference ��ʽ�����쳣*/
	//��by pointer��ʽ�����쳣���Ǻ�ע�� ��Ӧ�ñܿ�������by value ��by reference����
	
	/*
	 *ͨ��ֵ�����쳣��catch-by-value�����Խ�����������⣬�����쳣����ɾ���������ʹ�ñ�
	 *׼�쳣���͵����⡣���ǵ����Ǳ��׳�ʱϵͳ�����쳣���󿽱����Σ��μ�����M12����������
	 *�����slicing problem������������쳣������Ϊ�����쳣���󲶻�ʱ����������������Ϊ
	 *�ͱ��е��ˣ�sliced off����������sliced����ʵ������һ�������������û�����������
	 *�ݳ�Ա�����ҵ���׼���������ǵ����⺯��ʱ��ϵͳ��������õ�ȴ�ǻ������ĺ�����
	 */
	 
	 /*
	  *���ʣ�·�������ͨ�����ò����쳣��catch-by-reference����ͨ�����ò����쳣��ʹ��ܿ�
	  *�����������⡣����ͨ��ָ�벶���쳣�����ַ��������ж���ɾ�����������Ҳ�ܲ����׼�쳣
	  *���͡�Ҳ����ͨ��ֵ�����쳣�����ַ���û��slicing problem�������쳣����ֻ������һ�Ρ�
	  */
	  
	  class exception { // ���ϣ�����
public: // һ����׼�쳣��
virtual const char * what() throw();
// �����쳣�ļ������.
... // ���ں��������Ľ�β��
// ��"throw()"��
}; //�й�������Ϣ
// �μ�����14)
			class runtime_error:public exception { ... }; //Ҳ���Ա�׼C++�쳣��
			class Validation_error: // �ͻ��Լ��������
				public runtime_error {
									public:
										virtual const char * what() throw();
									// ���¶������쳣����
											... //���⺯��
				}; //
			void someFunction() // �׳�һ�� validation
			{ // �쳣
					...
					if (a validation ����ʧ��) {
					throw Validation_error();
					}
			...
			}
			void doSomething()
			{
					try {
					someFunction(); // �׳� validation
					} //�쳣
					catch (exception &ex) { //�������б�׼�쳣��
					// ������������
					cerr << ex.what(); // ���� Validation_error::what()
					... 
					}
			}