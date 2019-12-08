/*了解隐式接口和编译期多态*/

//对于template而言，接口是隐式的，基于有效表达式。
//多态则是通过template具现化和函数重载解析发生于编译期
template<typename T>
void doProcessing(T& w)
{
	if(w.size()>0 && w!=someNastyWidget){
		{
			T temp(w);
			temp.normalize();
			temp.swap(w);
		}
}