//以独立的语句将newed对象植入智能指针

//下面的写法可能造成内存泄漏
 processWidget(std::tr1::shared_ptr<Widget>(new Widget),priority());
 	
//改进的算法 不会造成内存泄漏
std::tr1::shared_ptr<Widget> pw(new Widget);
processWidget(pw,priority());

//以独立的对象将newed的对象存储在智能指针内，如果不这样做一旦异常抛出，将
//导致难以察觉的资源泄漏