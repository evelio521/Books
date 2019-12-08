ITEM1:��
	 //1.std::cin��std::cout��������?
	   std::basic_istream<char,std::char_traits<char> >
	   	std::basic_ostream<char,std::char_traits<char> >
	 //2.дһ��ECHO���� �������򵥵���Ӧ���룬��ͨ�����·�ʽ��Ч�ĵ���
	 ECHO<infile> outfile;
	 ECHO infile outfile;
	 
	 //��򵥵Ľ������
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
		//���ϴ���̫������ �ɶ��Բ��ߡ������׼���Ǿ�������Ŀɶ��ԣ�����׫д����Ĵ���
		
		//ʵ�ַ���
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
		 
		 //��Ч�Ĺ������׼�򣭣�������߿�������
		 ����//����д���Ĵ���ֻ�ܽ����ǰ�����⣬�����κ�ʱ������д����
		 ����//����ķ����������Ǹ��ӵ�ѡ��
		 //��Ч�Ĺ������׼�򣭣�������߷�װ�ԣ�����ϵ����
		 ����//������ܣ�һ�δ��룭���������࣭��Ӧ��ֻ֪������һ������
		 
		 
		//Process��ʵ��
		 //����A��ģ��
		 // Example 1-2(a): A templatized Process()
			//
			template<typename In, typename Out>
			void Process( In& in, Out& out )
			{
			  // ... do something more sophisticated,
			  //     or just plain "out << in.rdbuf();"...
			}
		//����B���麯��
		//ȱ����Ҫ��������������basic_istream<char>��basic_ostream<char>��������
			// Example 1-2(b): First attempt, sort of okay
			//
			void Process( basic_istream<char>& in,
			              basic_ostream<char>& out )
			{
			  // ... do something more sophisticated,
			  //     or just plain "out << in.rdbuf();"...
			}
		//����B�ļ�ǿ��  �����÷���Bͬʱ����ģ��
		//�ñ������Ƶ����ʵĲ���
		// Example 1-2(c): Better solution
		//
		template<typename C, typename T>
		void Process( basic_istream<C,T>& in,
		              basic_ostream<C,T>& out )
		{
		  // ... do something more sophisticated,
		  //     or just plain "out << in.rdbuf();"...
		}



			
					 
			
					
