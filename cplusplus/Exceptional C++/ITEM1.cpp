ITEM1: Iterators
	 
  int main(){
  	
  	vector<int> e;
  	copy(istream_iterator<Date>(cin),
  	     istream_iterator<Date>(),
  	     back_inserter(e));
  	vector<Date>::iterator first=
  		   find(e.begin(),e.end(),"01/01/95");
    vector<Date>::iterator last=
  		   find(e.begin(),e.end(),"12/31/95");
  	*last="12/30/95";
  	//上一行可能是不合法的  last可能是e.end()，是一个不可提领的iterator
  	copy(first,end,ostream_iterator<Date>(cout,"\n"));
  	//[first,last)可能是无效的范围，last可能在first之前
  	e.insert(--e.end(),TodayDate() );
  	//上一行--e.end()可能是不合法的，c++不允许修改内建型别的暂时物件。
  	//e.insert(e.end()-1,TodayDate() );
  	//上一行还有可能是错的 因为如果e为空的话 无论是 --e.end()还是 e.end()-1都是错的
  	copy( first,end,ostream_iterator<Date>(cout,"\n"));
  	//first,last可能是无效迭代器
  	//还有可能是 vector的可能成长 导致迭代器全部失效 		   
  	
  }
  
  //设计准则
  //使用iterator时，务必注意以下4点
  1.有效的数值:这个迭代器可提领吗?如果写成*e.end()绝对错误
  2.有效的寿命:这个迭代器被使用时还有效吗?或者因为某些操作已经无效
  3.有效的范围:一对迭代器是否组成一个有效的范围？是否first真的在last之后?是否两者指向同一个迭代器
  4.不合法的操作行为:程序式的代码是否试图修改内建型别的占时物件。想--e.end()这样吗？