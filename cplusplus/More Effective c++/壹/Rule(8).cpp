����8:
	/*�����ֲ�ͬ�����new��delete*/
	string *ps = new string("Memory Management");
	//��ʹ�õ�new��new���������������������sizeofһ��
	//���������õģ��㲻�ܸı����ĺ��壬���Ĺ�������һ���ġ�
	//��Ҫ��ɵĹ��ֳܷ������֡���һ�����Ƿ����㹻���ڴ���
	//�������������͵Ķ��󡣵ڶ������������ù��캯����ʼ����
	//���еĶ���new���������������������飬�㲻�����κη�ʽ�ı�������Ϊ��
	
	//����operator new ͨ������������
	void * operator new(size_t size);
	//����ֵ������void*����Ϊ�����������һ��δ������raw����ָ�룬δ��ʼ�����ڴ档
	
	void *rawMemory = operator new(sizeof(string));
	//������operator new������һ��ָ�룬ָ��һ���㹻����һ��string���Ͷ�����ڴ档
	//����mallocһ����operator new��ְ��ֻ�Ƿ����ڴ档���Թ��캯��һ����֪��
	//operator new���˽�����ڴ���䡣��operator new ���ص�δ�������ָ�봫
	//�ݸ�һ��������new�������Ĺ�����
	
	string *ps = new string("Memory Management");
	//һ�´���Ĺ��ܺ������һ��
	void *memory = // �õ�δ��������ڴ�
	operator new(sizeof(string)); // ΪString����
	call string::string("Memory Management") //��ʼ��
	on *memory; // �ڴ��еĶ���
	string *ps =static_cast<string*>(memory); // ��psָ��ָ���µĶ���
	
	//placement new ���÷� #include<new>
	class Widget {
		public:
			Widget(int widgetSize);
			...
	};
	Widget * constructWidgetInBuffer(void *buffer,int widgetSize)
	{
		return new (buffer) Widget(widgetSize);
	}
	
	
	//ɾ�����ڴ��ͷ�
	string *ps;
	...
	delete ps; // ʹ��delete ������
	
	void *buffer = operator new(50*sizeof(char));// �����㹻�� �ڴ�������50��char
                                             //û�е��ù��캯��
  ...
  operator delete(buffer); // �ͷ��ڴ� û�е�����������
  
  //placement new�ͷ��ڴ�ķ���
  /*
   * �������placement new���ڴ��н���������Ӧ�ñ����ڸ��ڴ�
   * ����delete����������Ϊdelete����������operator delete����
   * ���ڴ棬���ǰ���������ڴ�������Ǳ�operator new����ģ�
   * placement newֻ�Ƿ���ת�ݸ�����ָ�롣˭֪�����ָ�����Ժη���
   * ����Ӧ����ʽ���ö��������������������캯����Ӱ�죺
   */
    // �ڹ����ڴ��з�����ͷ��ڴ�ĺ���
    void * mallocShared(size_t size);
		void freeShared(void *memory);
		void *sharedMemory = mallocShared(sizeof(Widget));
		Widget *pw = // ������ʾ,
		constructWidgetInBuffer(sharedMemory, 10); // ʹ��// placement new
		...
		delete pw; // �����ȷ��! �����ڴ����� mallocShared, ������operator new
		pw->~Widget(); // ��ȷ�� ���� pwָ���Widget������û���ͷŰ���Widget���ڴ�
		freeShared(pw); // ��ȷ�� �ͷ�pwָ��Ĺ����ڴ浫��û�е�����������
		
		//������Դ���ͷ�
		string *ps=new string[10];
		
		delete [] ps;