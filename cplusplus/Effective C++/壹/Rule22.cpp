/*����Ա������Ϊprivate*/

class AccessLevels{
	
	public:
		...
		int getReadOnly()const
		{
			return readOnly;
		}
		void setReadWrite(int val)
		{
			readWrite=value;
		}
		int getReadWrite() const
		{
			return readWrite;
		}
		void setWriteOnly(int val)
		{
			writeOnly=val;
		}
	private:
		int noAccess;//�Դ�int���κη���
		int readOnly;//�Դ�intֻ������
		int readWrite;//�Դ�int��д����
		int writeOnly;//�Դ�intΩд����
	
};
//�����Ա������Ϊprotected��public ����ɲ���Ԥ֪��ά���ɱ�