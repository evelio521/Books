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
  	//��һ�п����ǲ��Ϸ���  last������e.end()����һ�����������iterator
  	copy(first,end,ostream_iterator<Date>(cout,"\n"));
  	//[first,last)��������Ч�ķ�Χ��last������first֮ǰ
  	e.insert(--e.end(),TodayDate() );
  	//��һ��--e.end()�����ǲ��Ϸ��ģ�c++�������޸��ڽ��ͱ����ʱ�����
  	//e.insert(e.end()-1,TodayDate() );
  	//��һ�л��п����Ǵ�� ��Ϊ���eΪ�յĻ� ������ --e.end()���� e.end()-1���Ǵ��
  	copy( first,end,ostream_iterator<Date>(cout,"\n"));
  	//first,last��������Ч������
  	//���п����� vector�Ŀ��ܳɳ� ���µ�����ȫ��ʧЧ 		   
  	
  }
  
  //���׼��
  //ʹ��iteratorʱ�����ע������4��
  1.��Ч����ֵ:�����������������?���д��*e.end()���Դ���
  2.��Ч������:�����������ʹ��ʱ����Ч��?������ΪĳЩ�����Ѿ���Ч
  3.��Ч�ķ�Χ:һ�Ե������Ƿ����һ����Ч�ķ�Χ���Ƿ�first�����last֮��?�Ƿ�����ָ��ͬһ��������
  4.���Ϸ��Ĳ�����Ϊ:����ʽ�Ĵ����Ƿ���ͼ�޸��ڽ��ͱ��ռʱ�������--e.end()������