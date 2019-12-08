/*������ʹ�� const*/
   /*
    *  1.const���ε�����
    */
    
    char greeting[]="hello";
    char* p=greeting;//non-const pointer,non-const data
    const char* p=greeting;//non-const pointer,const data
    char* const p=greeting;//const pointer,non-const data
    const char* const p=greeting;//const pointer,const data
    
    
    void f(const Widget* pw);//��ȡһ��ָ�룬ֻ��һ��������Widget����
    void f(Widget const* pw);//ͬ��
    
    
    //����һ��������Ϊconst����ʾ�������������ָ���Ķ����������Ըı���ֵ
    vector<int>  vec;
    
    const vector<int>::iterator iter=vec.begin;
    *iter=10;//��ȷ ���Ըı���ֵ
    ++iter;//���� �����Ըı�ָ��
    
    vector<int>::const_iterator iter=vec.begin;
    *iter=10;//���� �����Ըı���ֵ
    ++iter;//��ȷ  ���Ըı�ָ��
    
    /*�������һ������ֵ ���Լ���Ī������Ĵ���
     *���������Ϊconst
     * Rational a,b,c;
       ...
       (a*b)=c;//�Ϸ�
     */
     
    class Rational{
    	
    };
    const Rational operator*(const Rational& lhs,const Rational &rhs)
    {
    	return Rational();
    	
    }
    
    /*
    *  2.const ��Ա����
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
    cout<<tb[0];//���� non-const TextBlock::operator[]
    
    const Textblock  ctb("world");
    cout<<ctb[0];//���� const TextBlock::operator[]
    
    
    
    /*
     * non-const operator[] �ķ���������reference to char ����char�����operator[]
     * ֻ�Ƿ���һ��char ��tb[0]='x';�޷�ͨ�����룬��Ϊ�����������������ͣ��Ķ���������ֵ�����Ͳ��Ϸ�
     * 
     *
     
    cout<<tb[0];//ok ��һ��non-const TextBlock
    tb[0]='x';//ok дһ��non-const TextBlock
    cout<<ctb[0];//ok ��һ��const TextBlock
    ctb[0]='x';//wrong дһ��const TextBlock
    */
    
    /*
     * 3.�����const��Ա�����У����ĳ�Ա����
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
    				textLength=std:strlen(pText);//��const��Ա�����в��ܸ�textLength��lengthIsValid��ֵ
    					lengthIsValid=true;
    			}
    		return textLength;
    	}
    
    //��������ǽ�Ҫ�ı��ֵ����Ϊmutable
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
    				textLength=std:strlen(pText);//��const��Ա�����в��ܸ�textLength��lengthIsValid��ֵ
    					lengthIsValid=true;
    			}
    		return textLength;
    	}
    	
    /*
     * 4.const��non-const��Ա���������ظ�
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
    	
    		//�����ظ��ļ�����
    		return const_cast<char&>(static_cast<const TextBlock&>(*this)[position];;
    		
    	}
        		
    };
    
    