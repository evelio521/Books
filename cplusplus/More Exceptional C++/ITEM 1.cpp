ITEM1:流
	 //1.std::cin和std::cout的类型是?
	   std::basic_istream<char,std::char_traits<char> >
	   	std::basic_ostream<char,std::char_traits<char> >
	 //2.写一个ECHO程序 ，让他简单的相应输入，并通过以下方式等效的调用
	 ECHO<infile> outfile;
	 ECHO infile outfile;
	 
	 //最简单的解决方案
	 	// Example 1-1: A one-statement wonder
		//
		#include <fstream>
		#include <iostream>
		
		int main( int argc, char* argv[] )
		{
		  using namespace std;
		
		  (argc > 2
		    ? ofstream(argv[2], ios::out | ios::binary)
		    : cout)
		  <<
		  (argc > 1
		    ? ifstream(argv[1], ios::in | ios::binary)
		    : cin)
		  .rdbuf();
		}
		//以上代码太过精简 可读性不高。而设计准则是尽量的体改可读性，避免撰写精简的代码
		
		//实现方法
		#include <fstream>
		#include <iostream>
		
		 int main( int argc, char* argv[] )
		 {
		   using namespace std;
		
		   fstream in, out;
		   if( argc > 1 ) in.open ( argv[1], ios::in  | ios::binary );
		   if( argc > 2 ) out.open( argv[2], ios::out | ios::binary );
		
		   Process( in.is_open()  ? in  : cin,
		            out.is_open() ? out : cout );
		 }
		 
		 //有效的工程设计准则－－尽量提高可扩充性
		 　　//避免写出的代码只能解决当前的问题，几乎任何时候，若能写出可
		 　　//扩充的方案，按将是更加的选择
		 //有效的工程设计准则－－尽量提高封装性，将关系分离
		 　　//如果可能，一段代码－－函数或类－－应该只知道或负责一件事情
		 
		 
		//Process的实现
		 //方法A　模版
		 // Example 1-2(a): A templatized Process()
			//
			template<typename In, typename Out>
			void Process( In& in, Out& out )
			{
			  // ... do something more sophisticated,
			  //     or just plain "out << in.rdbuf();"...
			}
		//方法B　虚函数
		//缺点是要求输入和输出流是basic_istream<char>和basic_ostream<char>的派生类
			// Example 1-2(b): First attempt, sort of okay
			//
			void Process( basic_istream<char>& in,
			              basic_ostream<char>& out )
			{
			  // ... do something more sophisticated,
			  //     or just plain "out << in.rdbuf();"...
			}
		//方法B的加强版  及采用方法B同时采用模版
		//让编译器推导合适的参数
		// Example 1-2(c): Better solution
		//
		template<typename C, typename T>
		void Process( basic_istream<C,T>& in,
		              basic_ostream<C,T>& out )
		{
		  // ... do something more sophisticated,
		  //     or just plain "out << in.rdbuf();"...
		}



			
					 
			
					
