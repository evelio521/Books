规则29:
	/*Reference counting 引用计数*/
	
	
	//实现引用计数
	template<class T> // template class for smart
	class RCPtr { // pointers-to-T objects; T
		public: // must inherit from RCObject
			RCPtr(T* realPtr = 0);
			RCPtr(const RCPtr& rhs);
			~RCPtr();
			RCPtr& operator=(const RCPtr& rhs);
			T* operator->() const;
			T& operator*() const;
		private:
			T *pointee;
		void init();
	};
	class RCObject { // base class for reference-
		public: // counted objects
			void addReference();
			void removeReference();
			void markUnshareable();
			bool isShareable() const;
			bool isShared() const;
			protected:
			RCObject();
			RCObject(const RCObject& rhs);
			RCObject& operator=(const RCObject& rhs);
			virtual ~RCObject() = 0;
	private:
			int refCount;
			bool shareable;
	};
	class String { // class to be used by
		public: // application developers
			String(const char *value = "");
			const char& operator[](int index) const;
			char& operator[](int index);
		private:
			// class representing string values
			struct StringValue: public RCObject {
			char *data;
			StringValue(const char *initValue);
			StringValue(const StringValue& rhs);
			void init(const char *initValue);
			~StringValue();
		};
			RCPtr<StringValue> value;
	};

//实现
//这是RCObject的实现：
	RCObject::RCObject(): refCount(0), shareable(true) {}
	RCObject::RCObject(const RCObject&): refCount(0), shareable(true) {}
	RCObject& RCObject::operator=(const RCObject&){ return *this; }
	RCObject::~RCObject() {}
	void RCObject::addReference() { ++refCount; }
	void RCObject::removeReference()
     	{ if (--refCount == 0) delete this; }
	void RCObject::markUnshareable()
			{ shareable = false; }
	bool RCObject::isShareable() const
			{ return shareable; }
	bool RCObject::isShared() const
			{ return refCount > 1; }
	//这是RCPtr的实现：
	template<class T>
	void RCPtr<T>::init()
		{
			if (pointee == 0) return;
			if (pointee->isShareable() == false) {
			pointee = new T(*pointee);
			}
			pointee->addReference();
		}
	template<class T>
	RCPtr<T>::RCPtr(T* realPtr): pointee(realPtr)
		{ init(); }
	template<class T>
	RCPtr<T>::RCPtr(const RCPtr& rhs): pointee(rhs.pointee)
		{ init(); }
	template<class T>
	RCPtr<T>::~RCPtr()
		{ if (pointee)pointee->removeReference(); }
	template<class T>
	RCPtr<T>& RCPtr<T>::operator=(const RCPtr& rhs)
		{
			if (pointee != rhs.pointee) {
			if (pointee) pointee->removeReference();
			pointee = rhs.pointee;
			init();
			}
			return *this;
		}
	template<class T>
	T* RCPtr<T>::operator->() const { return pointee; }
	template<class T>
	T& RCPtr<T>::operator*() const { return *pointee; }
//这是String::StringValue的实现：
	void String::StringValue::init(const char *initValue)
	{
			data = new char[strlen(initValue) + 1];
			strcpy(data, initValue);
	}
	String::StringValue::StringValue(const char *initValue)
			{ init(initValue); }
	String::StringValue::StringValue(const StringValue& rhs)
			{ init(rhs.data); }
	String::StringValue::~StringValue()
			{ delete [] data; }
//最后，归结到String，它的实现是：
	String::String(const char *initValue): value(new StringValue(initValue)) {}
	const char& String::operator[](int index) const
			{ return value->data[index]; }
	char& String::operator[](int index)
		{
			if (value->isShared()) {
			value = new StringValue(value->data);
			}
			value->markUnshareable();
			return value->data[index];
		}
		
		
		//将reference counting 加到既有的classes身上
		template<class T>
		class RCIPtr {
			public:
				RCIPtr(T* realPtr = 0);
				RCIPtr(const RCIPtr& rhs);
				~RCIPtr();
				RCIPtr& operator=(const RCIPtr& rhs);
				const T* operator->() const; // see below for an
				T* operator->(); // explanation of why
				const T& operator*() const; // these functions are
				T& operator*(); // declared this way
			private:
				struct CountHolder: public RCObject {
					~CountHolder() { delete pointee; }
					T *pointee;
				};
				CountHolder *counter;
				void init();
				void makeCopy(); // see below
	};
	template<class T>
	void RCIPtr<T>::init()
	{
		if (counter->isShareable() == false) {
		T *oldValue = counter->pointee;
		counter = new CountHolder;
		counter->pointee = new T(*oldValue);
		}
		counter->addReference();
	}
	template<class T>
	RCIPtr<T>::RCIPtr(T* realPtr): counter(new CountHolder)
	{
		counter->pointee = realPtr;
		init();
	}
	template<class T>
	RCIPtr<T>::RCIPtr(const RCIPtr& rhs): counter(rhs.counter)
		{ init(); }
	template<class T>
	RCIPtr<T>::~RCIPtr()
		{ counter->removeReference(); }
	template<class T>
	RCIPtr<T>& RCIPtr<T>::operator=(const RCIPtr& rhs)
		{
			if (counter != rhs.counter) {
			counter->removeReference();
			counter = rhs.counter;
			init();
			}
			return *this;
		}
	template<class T> // implement the copy
	void RCIPtr<T>::makeCopy() // part of copy-on-
	{ // write (COW)
		if (counter->isShared()) {
			T *oldValue = counter->pointee;
			counter->removeReference();
			counter = new CountHolder;
			counter->pointee = new T(*oldValue);
			counter->addReference();
			}
	}
	template<class T> // const access;
	const T* RCIPtr<T>::operator->() const // no COW needed
		{ return counter->pointee; }
	template<class T> // non-const
	T* RCIPtr<T>::operator->() // access; COW
		{ makeCopy(); return counter->pointee; } // needed
	template<class T> // const access;
	const T& RCIPtr<T>::operator*() const // no COW needed
		{ return *(counter->pointee); }
	template<class T> // non-const
	T& RCIPtr<T>::operator*() // access; do the
		{ makeCopy(); return *(counter->pointee); } // COW thing
	class Widget {
		public:
			Widget(int size);
			Widget(const Widget& rhs);
			~Widget();
			Widget& operator=(const Widget& rhs);
			void doThis();
			int showThat() const;
	};
	//那么RCWidget将被定义为这样：
	class RCWidget {
		public:
			RCWidget(int size): value(new Widget(size)) {}
			void doThis() { value->doThis(); }
			int showThat() const { return value->showThat(); }
		private:
			RCIPtr<Widget> value;
	};