/*����non-member non-friend���member����*/

class WebBrowser{
	public:
		...
		void clearCache();
		void clearHistory();
		void removeCokies();
		...
};

//�ÿͻ�һ��һ��ִ��
  WebBrowser wb;
  wb.clearCache();
  wb.clearHistory();
  wb.removeCokies();
  
 //�ṩһ�����׶����ĺ���
 class WebBrowser{
	public:
		...
		void clearEverything();//���������3������
		...
};

//�ṩһ��non-member����(����д������õ�)
void clearBorowser(WebBrowser& wb)
{
	wb.clearCache();
  wb.clearHistory();
  wb.removeCokies();
}