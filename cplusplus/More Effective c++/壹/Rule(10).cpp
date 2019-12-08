规则:10
	/*在构造函数中防止资源泄漏*/
	/*
	 *在构造函数中防止资源泄漏如果你正在开发一个具有多媒体功能的通讯录程序。
	 *这个通讯录除了能存储通常的文字信息如姓名、地址、电话号码外，还能存储照片和声音
	 */
	 
	 class Image { // 用于图像数据
			public:
				Image(const string& imageDataFileName);
				...
		};
	 class AudioClip { // 用于声音数据
			public:
				AudioClip(const string& audioDataFileName);
				...
	 };
	 class PhoneNumber { ... }; // 用于存储电话号码
	 class BookEntry { // 通讯录中的条目
				public:
					BookEntry(const string& name,
										const string& address = "",
										const string& imageFileName = "",
										const string& audioClipFileName = "");
					~BookEntry();
					// 通过这个函数加入电话号码
					void addPhoneNumber(const PhoneNumber& number);
					...
				private:
					string theName; // 人的姓名
					string theAddress; // 他们的地址
					list<PhoneNumber> thePhones; // 他的电话号码
					Image *theImage; // 他们的图像
					AudioClip *theAudioClip; // 他们的一段声音片段
		};
		//编写BookEntry 构造函数和析构函数，有一个简单的方法是：
				BookEntry::BookEntry(const string& name,
														 const string& address,
														 const string& imageFileName,
														 const string& audioClipFileName)
				   : theName(name), theAddress(address),
				     theImage(0), theAudioClip(0)
				{
				if (imageFileName != "") {
						//如果BookEntry抛出异常 将不会调用析构函数  
						theImage = new Image(imageFileName);//有可能出发异常
						
					}
				if (audioClipFileName != "") {
					
						theAudioClip = new AudioClip(audioClipFileName);//有可能出发异常
					}
				}
				BookEntry::~BookEntry()
				{
						delete theImage;
						delete theAudioClip;
				}
				
				//解决方法1
				
				class Image { // 用于图像数据
					public:
					Image(const string& imageDataFileName);
					...
			  };
	 			class AudioClip { // 用于声音数据
					public:
						AudioClip(const string& audioDataFileName);
						...
				 };
	 			class PhoneNumber { ... }; // 用于存储电话号码
	 			class BookEntry { // 通讯录中的条目
						public:
								BookEntry(const string& name,
													const string& address = "",
													const string& imageFileName = "",
													const string& audioClipFileName = "");
					~BookEntry();
					// 通过这个函数加入电话号码
					void addPhoneNumber(const PhoneNumber& number);
					...
				private:
					string theName; // 人的姓名
					string theAddress; // 他们的地址
					list<PhoneNumber> thePhones; // 他的电话号码
					Image *theImage; // 他们的图像
					AudioClip *theAudioClip; // 他们的一段声音片段
					void cleanup();//共同清理释放动作
		};
		    void BookEntry::cleanup()
				{
							delete theImage;
							delete theAudioClip;
				}
		
				BookEntry::BookEntry(const string& name,
									const string& address,
									const string& imageFileName,
									const string& audioClipFileName)
											: theName(name), theAddress(address),
												theImage(0), theAudioClip(0)
				{
							try { // 这try block是新加入的
										if (imageFileName != "") {
										theImage = new Image(imageFileName);
										}
									if (audioClipFileName != "") {
												theAudioClip = new AudioClip(audioClipFileName);
										}
									}
							catch (...) { // 捕获所有异常
										cleanup();//释放资源
										throw; // 继续传递异常
										}
					}
					
					
				BookEntry::~BookEntry()
				{
						cleanup();//释放资源
				}
					
					
			//另外一种情况
			//theImage 和theAudioClip是常量（constant）指针类型：
				class BookEntry {
						public:
							... // 同上
						private:
							...
							Image * const theImage; // 指针现在是 const类型
							AudioClip * const theAudioClip; //只能在初始化列表初始化
				};
				// 一个可能在异常抛出时导致资源泄漏的实现方法
				BookEntry::BookEntry(const string& name,
															const string& address,
															const string& imageFileName,
															const string& audioClipFileName)
					: theName(name), theAddress(address),
						theImage(imageFileName != ""? new Image(imageFileName): 0),
						theAudioClip(audioClipFileName != ""? new AudioClip(audioClipFileName): 0)
				{}
				
				//解决方法
				class BookEntry {
					public:
						... // 同上
					private:
						const auto_ptr<Image> theImage; // 它们现在是
						const auto_ptr<AudioClip> theAudioClip; // auto_ptr对象
					};
				//以下代码就不会 出现资源泄漏
				BookEntry::BookEntry(const string& name,
														const string& address,
														const string& imageFileName,
														const string& audioClipFileName)
								: theName(name), theAddress(address),
									theImage(imageFileName != ""? new Image(imageFileName): 0),
									theAudioClip(audioClipFileName != ""? new AudioClip(audioClipFileName): 0)
				{}												