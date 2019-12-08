����:10
	/*�ڹ��캯���з�ֹ��Դй©*/
	/*
	 *�ڹ��캯���з�ֹ��Դй©��������ڿ���һ�����ж�ý�幦�ܵ�ͨѶ¼����
	 *���ͨѶ¼�����ܴ洢ͨ����������Ϣ����������ַ���绰�����⣬���ܴ洢��Ƭ������
	 */
	 
	 class Image { // ����ͼ������
			public:
				Image(const string& imageDataFileName);
				...
		};
	 class AudioClip { // ������������
			public:
				AudioClip(const string& audioDataFileName);
				...
	 };
	 class PhoneNumber { ... }; // ���ڴ洢�绰����
	 class BookEntry { // ͨѶ¼�е���Ŀ
				public:
					BookEntry(const string& name,
										const string& address = "",
										const string& imageFileName = "",
										const string& audioClipFileName = "");
					~BookEntry();
					// ͨ�������������绰����
					void addPhoneNumber(const PhoneNumber& number);
					...
				private:
					string theName; // �˵�����
					string theAddress; // ���ǵĵ�ַ
					list<PhoneNumber> thePhones; // ���ĵ绰����
					Image *theImage; // ���ǵ�ͼ��
					AudioClip *theAudioClip; // ���ǵ�һ������Ƭ��
		};
		//��дBookEntry ���캯����������������һ���򵥵ķ����ǣ�
				BookEntry::BookEntry(const string& name,
														 const string& address,
														 const string& imageFileName,
														 const string& audioClipFileName)
				   : theName(name), theAddress(address),
				     theImage(0), theAudioClip(0)
				{
				if (imageFileName != "") {
						//���BookEntry�׳��쳣 �����������������  
						theImage = new Image(imageFileName);//�п��ܳ����쳣
						
					}
				if (audioClipFileName != "") {
					
						theAudioClip = new AudioClip(audioClipFileName);//�п��ܳ����쳣
					}
				}
				BookEntry::~BookEntry()
				{
						delete theImage;
						delete theAudioClip;
				}
				
				//�������1
				
				class Image { // ����ͼ������
					public:
					Image(const string& imageDataFileName);
					...
			  };
	 			class AudioClip { // ������������
					public:
						AudioClip(const string& audioDataFileName);
						...
				 };
	 			class PhoneNumber { ... }; // ���ڴ洢�绰����
	 			class BookEntry { // ͨѶ¼�е���Ŀ
						public:
								BookEntry(const string& name,
													const string& address = "",
													const string& imageFileName = "",
													const string& audioClipFileName = "");
					~BookEntry();
					// ͨ�������������绰����
					void addPhoneNumber(const PhoneNumber& number);
					...
				private:
					string theName; // �˵�����
					string theAddress; // ���ǵĵ�ַ
					list<PhoneNumber> thePhones; // ���ĵ绰����
					Image *theImage; // ���ǵ�ͼ��
					AudioClip *theAudioClip; // ���ǵ�һ������Ƭ��
					void cleanup();//��ͬ�����ͷŶ���
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
							try { // ��try block���¼����
										if (imageFileName != "") {
										theImage = new Image(imageFileName);
										}
									if (audioClipFileName != "") {
												theAudioClip = new AudioClip(audioClipFileName);
										}
									}
							catch (...) { // ���������쳣
										cleanup();//�ͷ���Դ
										throw; // ���������쳣
										}
					}
					
					
				BookEntry::~BookEntry()
				{
						cleanup();//�ͷ���Դ
				}
					
					
			//����һ�����
			//theImage ��theAudioClip�ǳ�����constant��ָ�����ͣ�
				class BookEntry {
						public:
							... // ͬ��
						private:
							...
							Image * const theImage; // ָ�������� const����
							AudioClip * const theAudioClip; //ֻ���ڳ�ʼ���б��ʼ��
				};
				// һ���������쳣�׳�ʱ������Դй©��ʵ�ַ���
				BookEntry::BookEntry(const string& name,
															const string& address,
															const string& imageFileName,
															const string& audioClipFileName)
					: theName(name), theAddress(address),
						theImage(imageFileName != ""? new Image(imageFileName): 0),
						theAudioClip(audioClipFileName != ""? new AudioClip(audioClipFileName): 0)
				{}
				
				//�������
				class BookEntry {
					public:
						... // ͬ��
					private:
						const auto_ptr<Image> theImage; // ����������
						const auto_ptr<AudioClip> theAudioClip; // auto_ptr����
					};
				//���´���Ͳ��� ������Դй©
				BookEntry::BookEntry(const string& name,
														const string& address,
														const string& imageFileName,
														const string& audioClipFileName)
								: theName(name), theAddress(address),
									theImage(imageFileName != ""? new Image(imageFileName): 0),
									theAudioClip(audioClipFileName != ""? new AudioClip(audioClipFileName): 0)
				{}												