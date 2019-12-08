/*尽可能使用 const*/
   /*
    *  1.const修饰的意义
    */
    
    char greeting[]="hello";
    char* p=greeting;//non-const pointer,non-const data
    const char* p=greeting;//non-const pointer,const data
    char* const p=greeting;//const pointer,non-const data
    const char* const p=greeting;//const pointer,const data
    
    
    void f(const Widget* pw);//获取一个指针，只想一个常量的Widget对象
    void f(Widget const* pw);//同上
    
    
    //声明一个迭代器为const，表示这个迭代器不能指向别的东西，但可以改变其值
    vector<int>  vec;
    
    const vector<int>::iterator iter=vec.begin;
    *iter=10;//正确 可以改变其值
    ++iter;//错误 不可以改变指向
    
    vector<int>::const_iterator iter=vec.begin;
    *iter=10;//错误 不可以改变其值
    ++iter;//正确  可以改变指向
    
    /*令函数返回一个常量值 可以减少莫名其妙的错误
     *如果不声明为const
     * Rational a,b,c;
       ...
       (a*b)=c;//合法
     */
     
    class Rational{
    	
    };
    const Rational operator*(const Rational& lhs,const Rational &rhs)
    {
    	return Rational();
    	
    }
    
    /*
    *  2.const 成员函数
    */
    
    
    class TextBlock{
    	
    	public:
    		...
    	const char& operator[](size_t position)const
    	{
    		return text[position];
    		
    	}
    char& operator[](size_t position)
    	{
    		return text[position];
    		
    	}
    	private:
    		string text;
    		
    };
    
    TextBlock tb("hello");
    cout<<tb[0];//调用 non-const TextBlock::operator[]
    
    const Textblock  ctb("world");
    cout<<ctb[0];//调用 const TextBlock::operator[]
    
    
    
    /*
     * non-const operator[] 的返回类型是reference to char 不是char。如果operator[]
     * 只是返回一个char 则tb[0]='x';无法通过编译，因为函数返回是内置类型，改动函数返回值本来就不合法
     * 
     *
     
    cout<<tb[0];//ok 读一个non-const TextBlock
    tb[0]='x';//ok 写一个non-const TextBlock
    cout<<ctb[0];//ok 读一个const TextBlock
    ctb[0]='x';//wrong 写一个const TextBlock
    */
    
    /*
     * 3.如何在const成员函数中，更改成员变量
     */
     
     
     class CtextBlock{
     	
     	public:
     		
     	std::size_t length() const;
     	private:
     		char *pText;
     		std::dize_t textLength;
     		bool lengthIsValid;
    };
    std::size_t CTextblock::length()const
    	{
    		if(!lengthIsValid)
    			{
    				textLength=std:strlen(pText);//在const成员函数中不能给textLength和lengthIsValid赋值
    					lengthIsValid=true;
    			}
    		return textLength;
    	}
    
    //解决方法是将要改变的值声明为mutable
     class CtextBlock{
     	
     	public:
     		...
     	std::size_t length() const;
     	private:
     		char *pText;
     		mutable std::size_t textLength;
     		mutable bool lengthIsValid;
    };
    std::size_t CTextblock::length()const
    	{
    		if(!lengthIsValid)
    			{
    				textLength=std:strlen(pText);//在const成员函数中不能给textLength和lengthIsValid赋值
    					lengthIsValid=true;
    			}
    		return textLength;
    	}
    	
    /*
     * 4.const和non-const成员函数避免重复
     */
     
     
    	class TextBlock{
    	
    	public:
    		...
    	const char& operator[](size_t position)const
    	{
    		...
    		...
    		...
    		return text[position];
    		
    	}
    char& operator[](size_t position)
    	{
    	
    		//避免重复的简洁代码
    		return const_cast<char&>(static_cast<const TextBlock&>(*this)[position];;
    		
    	}
        		
    };
    
    