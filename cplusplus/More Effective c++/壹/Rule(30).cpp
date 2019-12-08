����30:
	/*proxy classes*/
	//ʵ�ֶ�ά����
	class Array2D {
		public:
			class Array1D {
				public:
					T& operator[](int index);
					const T& operator[](int index) const;
					...
		};
			Array1D operator[](int index);
			const Array1D operator[](int index) const;
			...
	};
	//���ڣ����Ϸ��ˣ�
		Array2D<float> data(10, 20);
		...
		cout << data[3][6]; // fine
		
	//����operator  []�Ķ�д����
	class String { // reference-counted strings;
		public: // see Item 29 for details
			class CharProxy { // proxies for string chars
				public:
					CharProxy(String& str, int index); // creation
					CharProxy& operator=(const CharProxy& rhs); // lvalue
					CharProxy& operator=(char c); // uses
					operator char() const; // rvalue
					// use
				private:
					String& theString; // string this proxy pertains to
					int charIndex; // char within that string
					// this proxy stands for
			};
				// continuation of String class
			const CharProxy
			operator[](int index) const; // for const Strings
			CharProxy operator[](int index); // for non-const Strings
			...
			friend class CharProxy;
		private:
			RCPtr<StringValue> value;
	};

	String s1, s2; // reference-counted strings
	// using proxies
	...
	cout << s1[5]; // still legal, still works
 	//���ʽs1[5]���ص���һCharProxy����û��Ϊ�����Ķ����������������
 	//���Ա�����Ŭ����Ѱ��һ����ʽ������ת����ʹ��operator<<���óɹ�����Item M5����
 	//�����ҵ�һ������CahrProxy���ڲ�������һ����ʽת����char�Ĳ���������
 	//�Զ��������ת���������������CharProxy����ݵ��ַ�����ӡ����ˡ����
 	//CharProxy��char��ת�������д����������ֵʹ��ʱ�����ĵ�����Ϊ��
	s2[5] = 'x'; // also legal, also works
	//��ǰ��һ�������ʽs2[5]���ص���һ��CharProxy���󣬵�������Ǹ�ֵ������Ŀ�ꡣ
	//���ڸ�ֵ��Ŀ����CharProxy�࣬���Ե��õ���CharProxy���еĸ�ֵ������
	//��������Ҫ����Ϊ��CharProxy�ĸ�ֵ�����У�����֪������ֵ��CharProxy��������
	//��ֵʹ�õġ���ˣ�����֪��proxy����ݵ��ַ�������ֵʹ�õģ�����ִ��һЩ��Ҫ
	//�Ĳ�����ʵ���ַ�����ֵ������
	s1[3] = s2[8]; // of course it's legal,of course it works
 	//��������������CharProxy�����ĸ�ֵ�������ڴ˲����ڲ���
 	//����֪�����һ��������ֵ���ұ�һ������ֵ��
 
 	const String::CharProxy String::operator[](int index) const
		{
			return CharProxy(const_cast<String&>(*this), index);
		}
	String::CharProxy String::operator[](int index)
		{
			return CharProxy(*this, index);
		}
	String::CharProxy::CharProxy(String& str, int index)
		: theString(str), charIndex(index) {}
	String::CharProxy::operator char() const
		{
			return theString.value->data[charIndex];
		}

	String::CharProxy&
	String::CharProxy::operator=(const CharProxy& rhs)
	{
		// if the string is sharing a value with other String objects,
		// break off a separate copy of the value for this string only
		if (theString.value->isShared()) {
			theString.value = new StringValue(theString.value->data);
		}
		// now make the assignment: assign the value of the char
		// represented by rhs to the char represented by *this
		theString.value->data[charIndex] =
		rhs.theString.value->data[rhs.charIndex];
		return *this;
		}
	String::CharProxy& String::CharProxy::operator=(char c)
		{
			if (theString.value->isShared()) {
				theString.value = new StringValue(theString.value->data);
				}
			theString.value->data[charIndex] = c;
			return *this;
		}
		
		
		
		//ʹ�ô����������
		//���String::operator[]����һ��CharProxy������char &������Ĵ��뽫���ܱ��룺
		String s1 = "Hello";
		char *p = &s1[1]; // error!
		//���ʽs1[1]����һ��CharProxy�����ǡ�=�����ұ���һ��CharProxy *��
		//û�д�CharProxy *��char *��ת������������p�ĳ�ʼ�����̱���ʧ���ˡ�
		//ͨ����ȡproxy�����ַ�Ĳ�����ȡʵ�ʶ����ַ�Ĳ����õ���ָ�룬�������ǲ�ͬ�ġ�
		
		//Ҫ���������ͬ������Ҫ����CharProxy���ȡ��ַ���㣺
		class String {
			public:
				class CharProxy {
					public:
						...
						char * operator&();
						const char * operator&() const;
					...
				};
				...
	 };
	 
	 const char * String::CharProxy::operator&() const
	{
			return &(theString.value->data[charIndex]);
	}
	char * String::CharProxy::operator&()
	{
		// make sure the character to which this function returns
		// a pointer isn't shared by any other String objects
		if (theString.value->isShared()) {
			theString.value = new StringValue(theString.value->data);
		}
		// we don't know how long the pointer this function
		// returns will be kept by clients, so the StringValue
		// object can never be shared
		theString.value->markUnshareable();
		return &(theString.value->data[charIndex]);
	}
	
	
	template<class T> // reference-counted array
	class Array { // using proxies
		public:
			class Proxy {
				public:
					Proxy(Array<T>& array, int index);
					Proxy& operator=(const T& rhs);
					operator T() const;
					...
			};
		const Proxy operator[](int index) const;
		Proxy operator[](int index);
	...
	};
	//��һ�����������ܱ�����ʹ�ã�
		Array<int> intArray;
		...
		intArray[5] = 22; // fine
		intArray[5] += 5; // error!
		++intArray[5]; // error!
		//����취����Ϊ�������ֶ����+= ++������

	
		//���ǽ�����һ��Rational�࣬Ȼ��ʹ��ǰ�濴����Arrayģ�壺
	class Rational {
		public:
			Rational(int numerator = 0, int denominator = 1);
			int numerator() const;
			int denominator() const;
			...
	};
	Array<Rational> array;
	//����������������ʹ�÷�ʽ�������Ǻ�ʧ����
	cout << array[4].numerator(); // error!
	int denom = array[22].denominator(); // error!

	//��һ��proxy�������ʵ�ʶ���ʧ�ܵ��������Ϊ��const�����ô���������
	void swap(char& a, char& b); // swaps the value of a and b
	String s = "+C+"; // oops, should be "C++"
	swap(s[0], s[1]); // this should fix the
	// problem, but it won't
	// compile
	
	
	class TVStation {
		public:
			TVStation(int channel);
			...
	};
		void watchTV(const TVStation& station, float hoursToWatch);
		//������int��TVStation����ʽ����ת������Item M5�������ǿ�����ô����
		watchTV(10, 2.5); // watch channel 10 for
		// 2.5 hours
		//Ȼ������ʹ���Ǹ���proxy������operator[]������ֵ�Ĵ����ü���������ģ��ʱ�����ǾͲ�����ô���ˣ�
		Array<int> intArray;
		intArray[4] = 10;
		watchTV(intArray[4], 2.5); // error! no conversion
		// from Proxy<int> to
		// TVStation