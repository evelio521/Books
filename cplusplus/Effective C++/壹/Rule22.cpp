/*将成员变量设为private*/

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
		int noAccess;//对此int无任何访问
		int readOnly;//对此int只读访问
		int readWrite;//对此int读写访问
		int writeOnly;//对此int惟写访问
	
};
//如果成员变量设为protected和public 会造成不可预知的维护成本