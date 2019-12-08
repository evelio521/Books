����28:
	/*����ָ��*/
	
	//�󲿷�����ָ��ģ�������
	class SmartPtr {
		public:
			SmartPtr(T* realPtr = 0); // ����һ������ָ��
				// ָ��dumb pointer��ָ��
				// ����δ��ʼ����ָ��
				// ȱʡֵΪ0(null)
			SmartPtr(const SmartPtr& rhs); // ����һ������ָ��
			~SmartPtr(); // �ͷ�����ָ��
			// make an assignment to a smart ptr
			SmartPtr& operator=(const SmartPtr& rhs);
			T* operator->() const; // dereferenceһ������ָ��
			// �Է�����ָ����ĳ�Ա
			T& operator*() const; // dereference ����ָ��
		private:
			T *pointee; // ����ָ����ָ�Ķ���
};

  
  //����ָ��Ĺ��졢��ֵ������
  
  template<class T>
		class auto_ptr {
			public:
				...
				auto_ptr(auto_ptr<T>& rhs); // �������캯��
				auto_ptr<T>& // ��ֵ
				operator=(auto_ptr<T>& rhs); // ������
				...
		};
	template<class T>
	auto_ptr<T>::auto_ptr(auto_ptr<T>& rhs)
	{
		pointee = rhs.pointee; // ��*pointee������Ȩ
			// ���ݵ� *this
		rhs.pointee = 0; // rhs����ӵ��
	} // �κζ���
	template<class T>
	auto_ptr<T>& auto_ptr<T>::operator=(auto_ptr<T>& rhs)
	{
		if (this == &rhs) // �������������Ҹ�ֵ
			return *this; // ʲôҲ��Ҫ��
		delete pointee; // ɾ������ӵ�еĶ���
		pointee = rhs.pointee; // ��*pointee������Ȩ
		rhs.pointee = 0; // �� rhs ���ݵ� *this
		return *this;
}

	//��Ϊ������auto_ptr�Ŀ������캯��ʱ�����������Ȩ�����ݳ�ȥ��
	//����ͨ����ֵ��ʽ����auto_ptr������һ�������ķ�������Ϊ��
  // �������ͨ���ᵼ�����ѷ���
		void printTreeNode(ostream& s, auto_ptr<TreeNode> p)
			{ s << *p; }
		int main()
		{
			auto_ptr<TreeNode> ptn(new TreeNode);
			...
			printTreeNode(cout, ptn); //ͨ����ֵ��ʽ����auto_ptr
			...
		}
		
		//�ⲻ��˵�㲻�ܰ�auto_ptr��Ϊ�������ݣ���ֻ��ζ�Ų���ʹ�ô�ֵ�ķ�����
		//ͨ��const���ô��ݣ�Pass-by-reference-to-const���ķ����������ģ�
		// �����������Ϊ��ֱ��һЩ
		void printTreeNode(ostream& s,const auto_ptr<TreeNode>& p)
			{ s << *p; }
	//����ָ�����������ͨ���������ģ�
		template<class T>
		SmartPtr<T>::~SmartPtr()
		{
			if (*this owns *pointee) {
			delete pointee;
			}
		}
		
		/*ʵ�ֽ����ò�����*/
		template<class T>
		T& SmartPtr<T>::operator*() const
		{
			perform "smart pointer" processing;
			return *pointee;
		}
		
		template<class T>
		T* SmartPtr<T>::operator->() const
		{
			perform "smart pointer" processing;
			return pointee;
		}
		
		/*��������ָ���Ƿ�ΪNULL*/
		//ĿǰΪֹ�������۵ĺ����������ǽ������ͷš���������ֵ��dereference����ָ�롣
		//������һ�������������������ǡ���������ָ��ΪNULL����
		SmartPtr<TreeNode> ptn;
		...
		if (ptn == 0) ... // error!
		if (ptn) ... // error!
		if (!ptn) ... // error!
		//����һ�����ص����ơ�
		
		//�������д����Ĵ������������ �����һ��������
		template<class T>
		class SmartPtr {
			public:
				...
				operator void*(); // �������ָ��Ϊnull��
				... // ����0�� ���򷵻�
		}; // ��0��
		SmartPtr<TreeNode> ptn;
		...
		if (ptn == 0) ... // ������ȷ
		if (ptn) ... // Ҳ��ȷ
		if (!ptn) ... // ��ȷ
	//����iostream�����ṩ������ת����ͬ�����Կ���������д���룺
		ifstream inputFile("datafile.dat");
		if (inputFile) ... // ����inputFile�Ƿ��Ѿ���
		// �ɹ��ش򿪡�
		//�����е�����ת������һ��������һ��ȱ�㣺��һЩ�������Ȼ��
		//��������Աϣ��������ʧ�ܣ����Ǻ���ȷʵ�ܹ��ɹ��ر����ã��μ�����M5����
		//�ر�������������ָ������ȫ��ͬ������֮����бȽϣ�
			SmartPtr<Apple> pa;
			SmartPtr<Orange> po;
			...
			if (pa == po) ... // ���ܹ����ɹ�����!
				
		//һ�ֲ�ǿ����Ľ���취
		template<class T>
		class SmartPtr {
			public:
				...
				bool operator!() const; // ���ҽ�������ָ����
				... // ��ֵ������true��
		};
		//�û�����������ʾ��
		SmartPtr<TreeNode> ptn;
			...
		if (!ptn) { // ��ȷ
		... // ptn �ǿ�ֵ
		}
		else {
		... // ptn���ǿ�ֵ
		}
		//���������Ͳ���ȷ�ˣ�
		if (ptn == 0) ... // ��Ȼ����
		if (ptn) ... // Ҳ�Ǵ����
		//������������»���ڲ�ͬ����֮����бȽϣ�
		SmartPtr<Apple> pa;
		SmartPtr<Orange> po;
		...
		if (!pa == !po) ... // �ܹ�����
			
		/*������ָ��ת���dumbָ��*/
		class Tuple { ... }; // ͬ��
		void normalize(Tuple *pt); // ��*pt ����
		// ��ʽ��; ע��ʹ�õ�
		// ��dumbָ��
		//����һ�£��������ͼ��ָ��Tuple������ָ������������normalize�������ʲô�����
		DBPtr<Tuple> pt;
		...
		normalize(pt); // ����!
		//������һ�����˷ǳ��ŷ���ԭ��ȥ������ ����Ҫ�ṩ��dumb pointer����ʽ����ת����������
		
		/*smart pointer�� "��̳��йص�"����ת��*/
		
		class MusicProduct { 
			public: 
				MusicProduct(const string& title); 
				virtual void play() const = 0; 
				virtual void displayTitle() const = 0;
				... 
		};
		 class Cassette: public MusicProduct { 
		 	public: 
		 		Cassette(const string& title); 
		 		virtual void play() const; 
		 		virtual void displayTitle() const; 
		 		... 
		 };
		 class CD: public MusicProduct { 
		 	public: 
		 		CD(const string& title); 
		 		virtual void play() const; 
		 		virtual void displayTitle() const;
		 		 ... 
		 };
   //�ٽ��ż��裬������һ������������һ��MusicProduct����
   //������ʾ��Ʒ�������������� 
   void displayAndPlay(const MusicProduct* pmp, int numTimes) 
   { 
   		for (int i = 1; i <= numTimes; ++i) 
   			{ 
   				pmp->displayTitle(); 
   				pmp->play();
   			 } 
   } 
   //��������ܹ�����ʹ�ã� 
   Cassette *funMusic = new Cassette("Alapalooza"); 
   CD *nightmareMusic = new CD("Disco Hits of the 70s"); 
   displayAndPlay(funMusic, 10); 
   displayAndPlay(nightmareMusic, 0); 
   //�Ⲣû��ʲôֵ�þ��ȵĶ��������ǵ�����������ָ�����dumbָ�룬�ᷢ��ʲô�أ�
	void displayAndPlay(const SmartPtr<MusicProduct>& pmp, int numTimes); 
	SmartPtr<Cassette> funMusic(new Cassette("Alapalooza")); 
	SmartPtr<CD> nightmareMusic(new CD("Disco Hits of the 70s")); 
	displayAndPlay(funMusic, 10); // ����! 
	displayAndPlay(nightmareMusic, 0); // ����! 
	
	//��Ȼ����ָ����ô������Ϊʲô���ܱ�����Щ�����أ�
	// ���ܽ��б����ԭ���ǲ��ܰ�SmartPtr<CD>��SmartPtr<Cassette>
	//ת����SmartPtr<MusicProduct>���ӱ������Ĺ۵���������Щ��֮��
	//û���κι�ϵ��Ϊʲô�������Ļ�������Ϊ�أ��Ͼ�SmartPtr<CD> ��
	// SmartPtr<Cassette>���Ǵ�SmartPtr<MusicProduct>�̳й����ģ�
	//��Щ��֮��û�м̳й�ϵ�����ǲ�����Ҫ���������һ�ֶ���ת����
	//����ȫ��ͬ�ģ���һ�����͵Ķ���
	
	//����취��
	template<class T> // ģ���ָ࣬��T�� 
	class SmartPtr { // ����ָ�� 
		public: 
			SmartPtr(T* realPtr = 0); 
			T* operator->() const; T& operator*() const; 
			template<class newType> // ģ���Ա���� 
			operator SmartPtr<newType>() // Ϊ��ʵ����ʽ����ת��. 
			{ 
				return SmartPtr<newType>(pointee); 
				} 
			... 
	};
	void displayAndPlay(const SmartPtr<MusicProduct>& pmp, int numTimes); 
	SmartPtr<Cassette> funMusic(new Cassette("Alapalooza")); 
	SmartPtr<CD> nightmareMusic(new CD("Disco Hits of the 70s")); 
	displayAndPlay(funMusic, 10); // ��ȷ! 
	displayAndPlay(nightmareMusic, 0); // ��ȷ!
	
	//���������õĹ���
	displayAndPlay(funMusic, 10); 
	
	/* 
	 *funMusic�����������SmartPtr<Cassette>������displayAndPlay�����Ĳ�����
	 * SmartPtr<MusicProduct>�ض��󡣱�������⵽���Ͳ�ƥ�䣬����Ѱ�Ұ�funMusic
	 * ת����SmartPtr<MusicProduct>����ķ���������SmartPtr<MusicProduct>����Ѱ
	 * �Ҵ���SmartPtr<Cassette>���Ͳ����ĵ��������캯�����μ�����M5��������û���ҵ���
	 * Ȼ��������Ѱ�ҳ�Ա����ģ�壬��ʵ�������������ĺ�����������SmartPtr<Cassette>
	 * ������ģ�壬��newType�󶨵�MusicProduct�ϣ�����������ĺ���
	 */
	 
	 //���ּ����ܸ����Ǽ���������Ҫ����Ϊ���ԡ�����������һ������CasSingle������
	 //MusicProduct���Σ�������ʾcassette singles��
	 template<class T> // ͬ��, ������Ϊ���� 
	 class SmartPtr { ... }; // ת���������ĳ�Աģ�� 
	 void displayAndPlay(const SmartPtr<MusicProduct>& pmp, int howMany); 
	 void displayAndPlay(const SmartPtr<Cassette>& pc, int howMany); 
	 SmartPtr<CasSingle> dumbMusic(new CasSingle("Achy Breaky Heart")); 
	 displayAndPlay(dumbMusic, 1); // ����! 
	 //����������displayAndPlay�����أ�һ����������SmartPtr<Cassette> ���������
	 //���������Ĳ���ΪSmartPtr<CasSingle>��������������SmartPtr<Cassette>��
	 //��ΪCasSingle��ֱ�Ӵ�Cassette�ϼ̳������ģ�����Ӽ̳���MusicProduct��
	 //��Ȼ����dumbָ��ʱ�Ĺ������������ǵ�����ָ�벻����ô���ɣ����ǵ�ת����
	 //�����ǳ�Ա��������C++���������ԣ���������ת����������ͬ�ȵ�λ�ġ�
	 //���displayAndPlay�ĵ��þ��ж����ԣ���Ϊ��SmartPtr<CasSingle> ��
	 //SmartPtr<Cassette>������ת�������ȵ�SmartPtr<MusicProduct>������ת�����ȡ�
	 
	 /*smart pointer��const*/
	 
	 CD goodCD("Flood");
		const CD *p;// p ��һ��non-const ָ��
							//ָ�� const CD ����
		CD * const p = &goodCD; // p ��һ��const ָ��
									// ָ��non-const CD ����
									// ��Ϊ p ��const, ��
									// ���뱻��ʼ��
		const CD * const p = &goodCD; // p ��һ��const
											// ָ��һ�� const CD ��
		//������Ȼ��Ҫ������ָ�����ͬ��������ԡ����ҵ���ֻ����һ���ط�����const��
		//��ֻ�ܶ�ָ�뱾�������ã��������������ָ����
		 const SmartPtr<CD> p = &goodCD;// p ��һ��const ����ָ��  
		 																// ָ�� non-const CD ����
		 																
		//���������������
		SmartPtr<CD> p; // non-const ���� // non-const ָ�� 
		SmartPtr<const CD> p; // const ����, // non-const ָ�� 
		const SmartPtr<CD> p = &goodCD; // non-const ���� // constָ�� 
		const SmartPtr<const CD> p = &goodCD; // const ���� // const ָ��
		
		CD *pCD = new CD("Famous Movie Themes"); 
		const CD * pConstCD = pCD; // ��ȷ 
		//�������������ͼ�����ַ�����������ָ���ϣ��������ô���أ� 
		SmartPtr<CD> pCD = new CD("Famous Movie Themes"); 
		SmartPtr<const CD> pConstCD = pCD; // ��ȷô?�� SmartPtr<CD> ��SmartPtr<const CD>����ȫ��ͬ������
		
		//��������Ľ������
		template<class T> // ָ��const����� 
		class SmartPtrToConst { // ����ָ�� 
			... // ����ָ��ͨ���� 
					// ��Ա���� 
			protected: 
				union { const T* constPointee; // �� SmartPtrToConst ���� 
					T* pointee; // �� SmartPtr ���� 
					}; 
			};
			 template<class T> // ָ��non-const���� 
			 class SmartPtr: // ������ָ�� 
			 	public SmartPtrToConst<T> {
			 		 ... // û�����ݳ�Ա
			};
			
			//������������ƣ������ܹ������Ҫ����Ϊ���ԣ� 
			SmartPtr<CD> pCD = new CD("Famous Movie Themes"); 
			SmartPtrToConst<CD> pConstCD = pCD; // ��ȷ
	