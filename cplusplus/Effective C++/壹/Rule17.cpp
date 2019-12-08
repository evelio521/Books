/*以独立的语句将newed对象植入智能指针*/

//下面的写法可能造成内存泄漏
 processWidget(std::tr1::shared_ptr<Widget>(new Widget),priority());
 	
//改进的算法 不会造成内存泄漏
std::tr1::shared_ptr<Widget> pw(new Widget);
processWidget(pw,priority());