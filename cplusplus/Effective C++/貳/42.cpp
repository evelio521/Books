/*了解typename的具体意义*/
//以下定义是等价的
template<class T> class Widget;
template<typename T> class Widget;


tempalte<typename C>
void print2nd(const C& container)
{
	if(container.size()>=2){
		typename C::const_iterator iter(container.begin());//此处必须用typename
		...
	}
}

tempalte<typename C>
void print2nd(const C& container,//不容许使用typename
typename C::iterator iter）//一定要使用typename
{
	
}


//typename必须做为嵌套从属类型名称的前缀词的例外是，typename不可以出现在base class list内的嵌套从属类型之前
//也不可以在成员初值列中做为base class的修饰符

template<typename T>
class Derived: public Base<T>::Nested{//不容许使用typename
	
	public:
		explicit Derived(int x):Base<T>::Nested(x)//不容许使用typename
			{
				typename Base<T>::Nested temp;//必须使用typename
					..
			}
			..
};


//注意以下写法
template<typename IterT>
void workWithIterator(IterT iter)
{
	typedef typename std::iterator_traits<IterT>::value_type value_type;
	value_type temp(*iter);//IterT是什么类型  temp就是什么类型
}