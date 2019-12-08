/*成对的使用 new 和delete */

std::string* stringPtr1=new std::string;
std::string* stringPtr1=new std::string[100];
	
	delete stringPtr1;
	delete [] stringPtr2;
	
	typedef std::string AddressLines[4];
	std::string * pa1=new AddressLines;
		
		delete  [] pal;