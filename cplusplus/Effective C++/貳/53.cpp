/*��Ҫ���ӱ�����Ϣ*/
class B{
	public:
		virtual void f() const;
};

class D:public B{
	
	public:
		virtual void f();//����δ����Ϊconst  ֻ�Ƕ�B::f�ĸ��� ��������������
};