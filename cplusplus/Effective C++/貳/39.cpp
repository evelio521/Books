/*���Ƕ�������ʹ��private*/

//�����֮����private�̳й�ϵ����ô���������Ὣderived classת��Ϊbase class 
//�����ܵ�ʹ�ø��ϣ���Ҫʱ��ʹ��private�̳У���protected��Ա��virtual����ǣ��������ʱ��

class Timer{
	public:
		explicit Timer(int tickFrequency);
		virtual void onTick();//��ʱ��ÿ�δ�һ�� �˺�������һ��
		...
};

class Widget : private Timer{
	private:
		virtual void onTick();
};

//�������Ĵ��� ����Ĵ�����ܸ���һЩ

class Widget{
	
	private:
		class WidgetTimer: public Timer{
			public:
			  virtual void onTick()const;//��ʱ��ÿ�δ�һ�� �˺�������һ��
			
		};
		WidgetTimer timer;
		...
};

//����������Ƶ��ռ����Ż���ʱ�� ��Ҫѡ��private�̳�

class Empty{//û������ ��Ӧ��ռ�ڴ�

};
class HoldAnInt{//Ӧ��ֵ��Ҫһ��int�ռ�
	private:
		int x;
		Empty e;//Ӧ���ǲ���Ҫ�ڴ�
};//��ʵsizeof( HoldAnInt)>sizeof(int)  �����������sizeof(Empty)==1

//ѡ��private�̳�
class HoldAnInt:private Empty {//Ӧ��ֵ��Ҫһ��int�ռ�
	private:
		int x;
};//sizeof( HoldAnInt)==sizeof(int) 
