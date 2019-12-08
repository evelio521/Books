ITEM35:�궨��
	//1.ʾ�����дһ���򵥵�Ԥ�����max(),�������������������ͨ����ͨ��<Ԫ��
	//���Ƚϳ����еĽϴ�ֵ����д����һ���꣬һ�������д�׷�����
	
	//1.��Ҫ����Ϊ������������
	// Example 35-1(a): Paren pitfall #1: arguments
	//
	#define max(a,b) a < b ? b : a
  //����д
  max( i += 3, j )
	//չ����
	i += 3 < j ? j : i += 3
	//��������ʵ������˳��Ϊ
	i += ((3 < j) ? j : i += 3)
  
  //2.��Ҫ����Ϊ����չ��ʽ��������
  // Example 35-1(b): Paren pitfall #2: expansion
	//
	#define max(a,b) (a) < (b) ? (b) : (a)
	
	 //����д
  k = max( i, j ) + 42;
	//չ����
	k = (i) < (j) ? (j) : (i) + 42;
	//��������ʵ������˳��Ϊ
	k = (((i) < (j)) ? (j) : ((i) + 42));
	
	//3.���Ķ����������
	// Example 35-1(c): Multiple argument evaluation
	//
	#define max(a,b) ((a) < (b) ? (b) : (a))
	//����д
	max( ++i, j )
	//ʵ��Ϊ
	((++i) < (j) ? (j) : (++i))

	//����д
	max( f(), pi )
  //ʵ��Ϊ
  ((f()) < (pi) ? (pi) : (f()))

	//4.���ֳ�ͻ
	// Example 35-1(d): Name tromping
	//
	#define max(a,b) ((a) < (b) ? (b) : (a))
	
	#include <algorithm> // oops!
	//��������ͷ�ļ������������Ķ���ʱ
	template<typename T> const T&
	max(const T& a, const T& b);
	//���Ǻ�ͽ�Դ�����滻��һ����
	template<typename T> const T&
  ((const T& a) < (const T& b) ? (const T& b) : (const T& a));
  //5.�겻�ܵݹ�
  //6.��û�е�ַ
  //7.���а�����








	

  
