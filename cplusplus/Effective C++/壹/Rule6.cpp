/*  �粻���ñ������Զ����ɺ��� ���͸���ȷ�ܾ�*/

 //��һ������
 class HomeForSale{
 	
 	public:
 	  
 	private:
 		HomeForSale(const HomeForSale&);//����Ϊprivate ���Ҳ�����
 		HomeForSale& operator=(const HomeforSale&);//����Ϊprivate ���Ҳ�����
};

 //�ڶ�������
 class Uncopyable{
 	protected:
 		Uncopyable(){}
 		~Uncopyable(){}
 	private:
 		Uncopyable(const Uncopyable&);
 		Uncopyable& operator=(const Uncopyable&);
 	
};
//Ϊ����ֹHomeForSale  ����ֻ��Ҫ��Uncopyable�̳�

 class HomeForSale1:private Uncopyable{
                                    //����д�κδ���	
};

