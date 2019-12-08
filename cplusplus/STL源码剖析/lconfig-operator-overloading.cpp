//increment/decrement/deference”¶”√
#include<iostream>
using namespace std;

class INT{
	
	friend ostream& operator<<(ostream& os,const INT& i);
	public:
		INT& operator++(){
			
			++(this->m_i);
			return *this;
		}
		const INT operator++(int)
		{
			INT tmp=*this;
			++(*this);
			return tmp;
		}
		INT& operator--(){
			
			--(this->m_i);
			return *this;
		}
		const INT operator--(int)
		{
			INT tmp=*this;
			--(*this);
			return tmp;
		}
		int operator*()const{
			return (int&)m_i;
		}
		private:
			int m_i;
};
ostream& operator<<(ostream& os,const INT& i)
{
	os<<'['<<i.m_i<<']';
	return os;
	
}

int main(){
	
	INT T(5);
	cout<<I++;*//5
	cout<<++I;//7
	cout<<I--;//7
	cout<<--I;//5
	cout<<*I;//5
	
|}