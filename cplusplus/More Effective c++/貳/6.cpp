#include<iostream>
using namespace std;
class UPInt { // "unlimited precision int"
		public:
			UPInt(int val):value(val){}
			UPInt& operator++(); // ++ 前缀
			const UPInt operator++(int); // ++ 后缀
			UPInt& operator--(); // -- 前缀
			const UPInt operator--(int); // -- 后缀
			
			UPInt& operator+=(int  val) // += 操作符，UPInts
			{
				(*this).value =(*this).value+val;
				return *this;
			}
			UPInt& operator-=(int  val) // -= 操作符，UPInts
			{
				(*this).value =(*this).value-val;
				return *this;
			}
			// 与ints 相运算
			void display()
			{
				cout<<value<<endl;
			}
		 private:
		 	int value; 
		};
			
			
			// 前缀形式：增加然后取回值
			UPInt& UPInt::operator++()
			{
				*this += 1; // 增加
				return *this; // 取回值
			}
			// postfix form: fetch and increment
			const UPInt UPInt::operator++(int)
			{
				UPInt oldValue = *this; // 取回值
				++(*this); // 增加
				return oldValue; // 返回被取回的值
			}
			
			// 前缀形式：减少然后取回值
			UPInt& UPInt::operator--()
			{
				*this -= 1; // 减少
				return *this; // 取回值
			}
			// postfix form: fetch and increment
			const UPInt UPInt::operator--(int)
			{
				UPInt oldValue = *this; // 取回值
				++(*this); // 减少
				return oldValue; // 返回被取回的值
			}
			
			int main(){
				
				UPInt i(15);;
			++i; // 调用 i.operator++();
			i.display();
			i++; // 调用 i.operator++(0);
			i.display();
			--i; // 调用 i.operator--();
			i.display();
			i--; // 调用 i.operator--(0);
			i.display();
			}
			
			
			//++p 比p++ 效率高 尽量使用++p