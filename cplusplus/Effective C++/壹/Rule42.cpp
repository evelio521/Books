/*�˽�typename�ľ�������*/
//���¶����ǵȼ۵�
template<class T> class Widget;
template<typename T> class Widget;


tempalte<typename C>
void print2nd(const C& container)
{
	if(container.size()>=2){
		typename C::const_iterator iter(container.begin());//�˴�������typename
		...
	}
}

tempalte<typename C>
void print2nd(const C& container,//������ʹ��typename
typename C::iterator iter��//һ��Ҫʹ��typename
{
	
}


//typename������ΪǶ�״����������Ƶ�ǰ׺�ʵ������ǣ�typename�����Գ�����base class list�ڵ�Ƕ�״�������֮ǰ
//Ҳ�������ڳ�Ա��ֵ������Ϊbase class�����η�

template<typename T>
class Derived: public Base<T>::Nested{//������ʹ��typename
	
	public:
		explicit Derived(int x):Base<T>::Nested(x)//������ʹ��typename
			{
				typename Base<T>::Nested temp;//����ʹ��typename
					..
			}
			..
};


//ע������д��
template<typename IterT>
void workWithIterator(IterT iter)
{
	typedef typename std::iterator_traits<IterT>::value_type value_type;
	value_type temp(*iter);//IterT��ʲô����  temp����ʲô����
}