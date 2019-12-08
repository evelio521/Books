namespace WebbrowserStuff{
	
	class WebBrowser{
	public:
		
		void clearCache(){}
		void clearHistory(){}
		void removeCokies(){}
		
	};
	//宁愿用non-member函数 也不用member函数 详细原因见教材
	void clearBorowser(WebBrowser& wb)
	{
	wb.clearCache();
  wb.clearHistory();
  wb.removeCokies();
	}
	
}