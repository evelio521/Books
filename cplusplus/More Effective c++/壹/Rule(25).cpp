����25:
	/*�����캯���ͷǳ�Ա�������⻯*/
	//virtual constructor
	class NLComponent { //���� newsletter components
		public: // �ĳ������
			... //��������һ�����麯��
		private:
			
		};
	class TextBlock: public NLComponent {
		public:
			... // ���������麯��
		};
	class Graphic: public NLComponent {
		public:
			... // ���������麯��
		};
	class NewsLetter { // һ�� newsletter ����
		public: // ��NLComponent ����
			... // ���������
		private:
			// Ϊ������һ��NLComponent�����str��ȡ����,
			// ����component ������һ��ָ�롣
			static NLComponent * readComponent(istream& str);
			list<NLComponent*> components;
		};
		
	NewsLetter::NewsLetter(istream& str)
		{
			while (str) {
			// ��readComponent���ص�ָ����ӵ�components��������
			// "push_back" һ������ĳ�Ա���������������������в��������
			components.push_back(readComponent(str));
				}
		}
		
		//virtual copy constructor
		class NLComponent {
			public:
				// declaration of virtual copy constructor
				virtual NLComponent * clone() const = 0;
				...
		};
		class TextBlock: public NLComponent {
			public:
				virtual TextBlock * clone() const // virtual copy
						{ return new TextBlock(*this); } // constructor
				...
		};
		class Graphic: public NLComponent {
			public:
				virtual Graphic * clone() const // virtual copy
					{ return new Graphic(*this); } // constructor
				...
		};
		class NewsLetter {
			public:
				NewsLetter(const NewsLetter& rhs);
				...
			private:
				list<NLComponent*> components;
		};
		NewsLetter::NewsLetter(const NewsLetter& rhs)
		{
			// ��������rhs����ʹ��ÿ��Ԫ�ص����⿽�����캯��
			// ��Ԫ�ؿ�������������component����
			// �й��������������е���ϸ�������μ�����M35.
			for (list<NLComponent*>::const_iterator it =
				rhs.components.begin();
				it != rhs.components.end();
				++it) {
						// "it" ָ��rhs.components�ĵ�ǰԪ�أ�����Ԫ�ص�clone������
						// �õ���Ԫ�ص�һ�����������Ѹÿ����ŵ�
						// ��������component�����β�ˡ�
				components.push_back((*it)->clone());
				}
		}
		
		/*
		 * ע�����������ʵ������������ű����ɵĽϿ��ɵ����⺯������ֵ���͹���
		 * ���������ض�������⺯�����ñ������������⺯������һ���ķ������͡�
		 * ��������ķ���������һ��ָ������ָ�루��һ�����ã�����ô������ĺ�
		 * �����Է���һ��ָ�������������ָ�루�����ã����ⲻ��C++�����ͼ���ϵ�
		 * ©������ʹ���п������������⹹�캯�������ĺ���.
		 */
		 
		 //��non-member functions����Ϊ�黯
		 //����Ĵ��������鷳
		 class NLComponent {
			public:
				// ������������Ĳ�Ѱ��������
				virtual ostream& operator<<(ostream& str) const = 0;
			...
		};
		class TextBlock: public NLComponent {
			public:
				// �������������(ͬ����Ѱ��)
				virtual ostream& operator<<(ostream& str) const;
		};
		class Graphic: public NLComponent {
			public:
				// ������������� (�þͲ�Ѱ��)
				virtual ostream& operator<<(ostream& str) const;
		};
		TextBlock t;
		Graphic g;
		...
		t << cout; 	// ͨ��virtual operator<<
								//��t��ӡ��cout�С�
								// ��Ѱ�����﷨
		g << cout; 	//ͨ��virtual operator<<
								//��g��ӡ��cout�С�
								//��Ѱ�����﷨
		//����취
		class NLComponent {
			public:
				virtual ostream& print(ostream& s) const = 0;
				...
		};
		class TextBlock: public NLComponent {
			public:
				virtual ostream& print(ostream& s) const;
				...
		};
		class Graphic: public NLComponent {
			public:
				virtual ostream& print(ostream& s) const;
				...
		};
		inline ostream& operator<<(ostream& s, const NLComponent& c)
		{
				return c.print(s);
		}