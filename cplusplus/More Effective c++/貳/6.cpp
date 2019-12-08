#include<iostream>
using namespace std;
class UPInt { // "unlimited precision int"
		public:
			UPInt(int val):value(val){}
			UPInt& operator++(); // ++ ǰ׺
			const UPInt operator++(int); // ++ ��׺
			UPInt& operator--(); // -- ǰ׺
			const UPInt operator--(int); // -- ��׺
			
			UPInt& operator+=(int  val) // += ��������UPInts
			{
				(*this).value =(*this).value+val;
				return *this;
			}
			UPInt& operator-=(int  val) // -= ��������UPInts
			{
				(*this).value =(*this).value-val;
				return *this;
			}
			// ��ints ������
			void display()
			{
				cout<<value<<endl;
			}
		 private:
		 	int value; 
		};
			
			
			// ǰ׺��ʽ������Ȼ��ȡ��ֵ
			UPInt& UPInt::operator++()
			{
				*this += 1; // ����
				return *this; // ȡ��ֵ
			}
			// postfix form: fetch and increment
			const UPInt UPInt::operator++(int)
			{
				UPInt oldValue = *this; // ȡ��ֵ
				++(*this); // ����
				return oldValue; // ���ر�ȡ�ص�ֵ
			}
			
			// ǰ׺��ʽ������Ȼ��ȡ��ֵ
			UPInt& UPInt::operator--()
			{
				*this -= 1; // ����
				return *this; // ȡ��ֵ
			}
			// postfix form: fetch and increment
			const UPInt UPInt::operator--(int)
			{
				UPInt oldValue = *this; // ȡ��ֵ
				++(*this); // ����
				return oldValue; // ���ر�ȡ�ص�ֵ
			}
			
			int main(){
				
				UPInt i(15);;
			++i; // ���� i.operator++();
			i.display();
			i++; // ���� i.operator++(0);
			i.display();
			--i; // ���� i.operator--();
			i.display();
			i--; // ���� i.operator--(0);
			i.display();
			}
			
			
			//++p ��p++ Ч�ʸ� ����ʹ��++p