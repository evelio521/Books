/*Ϊ"�쳣��ȫ"��Ŭ����ֵ�õ�*/

//����Ĵ���Գ����쳣��ȫ����
class PreettyMenu{
	public:
		...
		void changeBackground(std::istream& imgSrc);
			...
	private:
		Mutex mutex;//������
		Image* bgImage;//Ŀǰ�ı���ͼƬ
		int imageChange;//����ͼƬ�Ķ��Ĵ���
};
void PreetyMenu::changeBackground(std::istream& imgSrc)
	{
		Lock(&mutex);//����������
		delete bgImage;//���оɵı���ͼ��
		++imageChanges;//�޸�ͼ��������
		bgImage=new Image(imgSrc);//��װ�µı���ͼ��
		unlock(&mutex);//�ͷŻ�����
		
	}
//�޸ĺ�ޚ���
 class PreettyMenu{
	public:
		...
		void changeBackground(std::istream& imgSrc);
		...
	private:
		Mutex mutex;//������
		std::tr1::shared_ptr<Image> bgImage;//Ŀǰ�ı���ͼƬ
		int imageChange;//����ͼƬ�Ķ��Ĵ���
};
void PreetyMenu::changeBackground(std::istream& imgSrc)
	{
		Lock m1(&mutex);//����������
		bgImage.reset(new Image(imgSrc));
		++imageChanges;//�޸�ͼ��������
		
		
	}