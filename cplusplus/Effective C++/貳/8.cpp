
class DBConnection{
	public:
		static DBConnection create();
		void close();
};
class DBConn{
	public:
		void close()
		{
			db.close();
			closed=true;
		}
		~DBConn()
		{
			if(!closed)
				{
					try{
						db.closed();
					}
					catch(...)
					{
						//������ת��¼�����¶�close�ĵ���ʧ��
					}
				}
		}
	private:
		DBconnection db;
		bool closed;
};
//�����������Բ�Ҫ�³��쳣�����һ�����������³��쳣�Ļ���
//��������Ӧ�ò����쳣���������ǻ��������
