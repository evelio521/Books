Class HashTable {
	public:
		void Insert(const std::string &str); 
		int Remove(const std::string &str); 
		bool Has(const std::string &str) const; 
		int GetSize() const;
	private:
		mutable int mCachedSize; 
		mutable bool mSizeIsDirty; 
};

构造 复制 赋值 和析构应同时出现
尽量不要使用参数默认值，应该使用函数重载
使用静态const数据成员 非#define定义常量
避免使用友元
namespace {
	const int INTERNAL_CONSTANT = 42;
	std::string Filename = "file.txt";
	void FreeFunction() {
		std::cout << "Free function called" << std::endl; 
	}

}



#if defined _WIN32 || defined __CYGWIN__
	#ifdef _EXPORTING // define this when generating DLL
		#ifdef __GNUC__
			#define DLL_PUBLIC __attribute__((dllexport))
		#else
			#define DLL_PUBLIC __declspec(dllexport)
		#endif 
	#else
		#ifdef __GNUC__
			#define DLL_PUBLIC __attribute__((dllimport))
		#else
			#define DLL_PUBLIC __declspec(dllimport)
		#endif
	#endif
	#define DLL_HIDDEN 
#else
	#if __GNUC__ >= 4
		#define DLL_PUBLIC __attribute__ ((visibility("default"))) 
		#define DLL_HIDDEN __attribute__ ((visibility("hidden")))
	#else
		#define DLL_PUBLIC #define DLL_HIDDEN
	#endif
#endif

DLL_PUBLIC void MyFunction(); 
class DLL_PUBLIC MyClass;
