namespace WebbrowserStuff{
	
	class WebBrowser{
	public:
		
		void clearCache(){}
		void clearHistory(){}
		void removeCokies(){}
		
	};
	//��Ը��non-member���� Ҳ����member���� ��ϸԭ����̲�
	void clearBorowser(WebBrowser& wb)
	{
	wb.clearCache();
  wb.clearHistory();
  wb.removeCokies();
	}
	
}