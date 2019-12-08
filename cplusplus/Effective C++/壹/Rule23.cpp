/*宁以non-member non-friend替代member函数*/

class WebBrowser{
	public:
		...
		void clearCache();
		void clearHistory();
		void removeCokies();
		...
};

//让客户一个一个执行
  WebBrowser wb;
  wb.clearCache();
  wb.clearHistory();
  wb.removeCokies();
  
 //提供一个整套动作的函数
 class WebBrowser{
	public:
		...
		void clearEverything();//调用上面的3个函数
		...
};

//提供一个non-member函数(这种写法是最好的)
void clearBorowser(WebBrowser& wb)
{
	wb.clearCache();
  wb.clearHistory();
  wb.removeCokies();
}