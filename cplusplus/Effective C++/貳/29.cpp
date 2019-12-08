/*为"异常安全"而努力是值得的*/

//下面的代码对出现异常安全问题
class PreettyMenu{
	public:
		...
		void changeBackground(std::istream& imgSrc);
			...
	private:
		Mutex mutex;//互斥器
		Image* bgImage;//目前的背景图片
		int imageChange;//背景图片改动的次数
};
void PreetyMenu::changeBackground(std::istream& imgSrc)
	{
		Lock(&mutex);//锁定互斥器
		delete bgImage;//拜托旧的北京图象
		++imageChanges;//修改图像变更次数
		bgImage=new Image(imgSrc);//安装新的背景图像
		unlock(&mutex);//释放互斥器
		
	}
//修改后逇结果
 class PreettyMenu{
	public:
		...
		void changeBackground(std::istream& imgSrc);
		...
	private:
		Mutex mutex;//互斥器
		std::tr1::shared_ptr<Image> bgImage;//目前的背景图片
		int imageChange;//背景图片改动的次数
};
void PreetyMenu::changeBackground(std::istream& imgSrc)
	{
		Lock m1(&mutex);//锁定互斥器
		bgImage.reset(new Image(imgSrc));
		++imageChanges;//修改图像变更次数
		
		
	}