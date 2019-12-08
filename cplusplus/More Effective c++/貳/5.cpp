/*
template<class T>
		class Array {
			public:
				
				explicit Array(int size){} // 注意使用"explicit"
				
		};
		
	int main(){
		Array<int> a(10); // 正确, explicit 构造函数
											// 在建立对象时能正常使用
		Array<int> b(10); // 也正确
		int i=0;
		int c[10]={1};
		//if (a == b[i]) ... // 错误! 没有办法
											// 隐式转换
											// int 到 Array<int>
		
		if (a == Array<int>(c[i])){} // 正确,显式从int到 Array<int>转换
							        // （但是代码的逻辑不合理）
		                            //vc2005编译无法通过
		if (a == static_cast< Array<int> >(c[i])){

		}// 同样正确， 不合理
		//vc2005编译无法通过
		if (a == (Array<int>)c[i]) {} //C风格的转换也正确，
									// 但是逻辑 依旧不合理
		}							//vc2005编译无法通过				


		*/

template<class T>
		class Array {
			public:
				class ArraySize { // 这个类是新的
					public:
						ArraySize(int numElements): theSize(numElements) {}
						int size() const { return theSize; }
					private:
						int theSize;
					};
				Array(int lowBound, int highBound){}
				Array(ArraySize size){} // 注意新的声明
				
		};
    /*
     * 你的编译器要求用int参数调用Array<int>里的构造函数，但是没有这样的构造函数。
     * 编译器意识到它能从int参数转换成一个临时ArraySize对象，ArraySize对象只是
     * Array<int>构造函数所需要的，这样编译器进行了转换。函数调用（及其后的对象建立）也就成功了。
     */
     
		bool operator==( const Array<int>& lhs,const Array<int>& rhs){

				return lhs==rhs;
		}

		int main(){
			Array<int> a(10);
			Array<int> b(10);
			
		for (int i = 0; i < 10; ++i)
		{
		}
			//if (a == b[i]) {} // 哎呦! "a" 应该是 "a[i]";
														// 现在是一个错误。

		}