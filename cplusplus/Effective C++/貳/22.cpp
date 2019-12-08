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
	   //protected不必public更有封装性

	private:
		int noAccess;//对此int无任何访问
		int readOnly;//对此int只读访问
		int readWrite;//对此int读写访问
		int writeOnly;//对此int惟写访问
	
};

int main()
{
	AccessLevels al;
	al.setReadWrite(6);
	al.setWriteOnly(7);
	al.getReadOnly();
	al.getReadWrite();
}