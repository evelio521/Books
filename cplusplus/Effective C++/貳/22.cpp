class AccessLevels{
	
	public:
		
		int getReadOnly()const
		{
			return readOnly;
		}
		void setReadWrite(int val)
		{
			readWrite=val;
		}
		int getReadWrite() const
		{
			return readWrite;
		}
		void setWriteOnly(int val)
		{
			writeOnly=val;
		}
   protected:
	   //protected����public���з�װ��

	private:
		int noAccess;//�Դ�int���κη���
		int readOnly;//�Դ�intֻ������
		int readWrite;//�Դ�int��д����
		int writeOnly;//�Դ�intΩд����
	
};

int main()
{
	AccessLevels al;
	al.setReadWrite(6);
	al.setWriteOnly(7);
	al.getReadOnly();
	al.getReadWrite();
}