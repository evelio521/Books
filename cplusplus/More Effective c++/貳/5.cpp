/*
template<class T>
		class Array {
			public:
				
				explicit Array(int size){} // ע��ʹ��"explicit"
				
		};
		
	int main(){
		Array<int> a(10); // ��ȷ, explicit ���캯��
											// �ڽ�������ʱ������ʹ��
		Array<int> b(10); // Ҳ��ȷ
		int i=0;
		int c[10]={1};
		//if (a == b[i]) ... // ����! û�а취
											// ��ʽת��
											// int �� Array<int>
		
		if (a == Array<int>(c[i])){} // ��ȷ,��ʽ��int�� Array<int>ת��
							        // �����Ǵ�����߼�������
		                            //vc2005�����޷�ͨ��
		if (a == static_cast< Array<int> >(c[i])){

		}// ͬ����ȷ�� ������
		//vc2005�����޷�ͨ��
		if (a == (Array<int>)c[i]) {} //C����ת��Ҳ��ȷ��
									// �����߼� ���ɲ�����
		}							//vc2005�����޷�ͨ��				


		*/

template<class T>
		class Array {
			public:
				class ArraySize { // ��������µ�
					public:
						ArraySize(int numElements): theSize(numElements) {}
						int size() const { return theSize; }
					private:
						int theSize;
					};
				Array(int lowBound, int highBound){}
				Array(ArraySize size){} // ע���µ�����
				
		};
    /*
     * ��ı�����Ҫ����int��������Array<int>��Ĺ��캯��������û�������Ĺ��캯����
     * ��������ʶ�����ܴ�int����ת����һ����ʱArraySize����ArraySize����ֻ��
     * Array<int>���캯������Ҫ�ģ�����������������ת�����������ã������Ķ�������Ҳ�ͳɹ��ˡ�
     */
     
		bool operator==( const Array<int>& lhs,const Array<int>& rhs){

				return lhs==rhs;
		}

		int main(){
			Array<int> a(10);
			Array<int> b(10);
			
		for (int i = 0; i < 10; ++i)
		{
		}
			//if (a == b[i]) {} // ����! "a" Ӧ���� "a[i]";
														// ������һ������

		}