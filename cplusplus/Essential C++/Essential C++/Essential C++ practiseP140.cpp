#include<iostream>
using namespace std;
class LibMat{
	public:
		LibMat(){cout<<"Libmat::libmat() default constructor!\n";}
		virtual ~LibMat(){"LibMat::~Libmat() destructor!\n";}
		virtual void print() const{cout<<"LibMat::print()--I am a LibMat object!\n";}
}; 
void print(const LibMat &mat)
{
	cout<<"in global print():about to print mat.print()\n";
	mat.print() ;
}
class Book:public LibMat{
	public:
		Book(const string &title,const string &author)
		:_title(title),_author(author){
			cout<<"Book::Book("<<_title
			    <<", "<<_author<<" ) constructor\n";
		}
	virtual ~Book(){
		cout<<"Book::~Book() destructor!\n";
	}
	virtual void print()const{
	cout<<"Book::print()--I am a Book object!\n"
	    <<"My title is: "<<_title<<'\n'
		<<"My author is: "<<_author<<endl;}
	const string& title() const{return _title;}
	const string& author() const{return _author;}
	protected:
		string _title;
		string _author;
};
class AudioBook:public Book{
	public:
		AudioBook(const string &title,const string &author,const string &narrator):Book(title,author),_narrator(narrator)
		{
			cout<<"AudioBook::AudioBook( "<<title
			    <<", "<<_author<<", "<<_narrator
			    <<") constructor\n";
		}
		~AudioBook()
		{
			cout<<"AudioBook::~AudioBook() destructor!\n";
		}
		virtual void print() const{
		    cout<<"AudioBook::print()--I am an AudioBook object!\n"
			    <<"My title is: "<<_title<<'\n'
				<<"My author is: "<<_author<<'\n'
				<<"My narrator is: "<<_narrator<<endl;}
		const string& narrator() const{return _narrator;}
	protected:
		string _narrator;
};
int main()
{
	cout<<"\n"<<"creating a LibMat object to print()\n";
	LibMat libmat;
	print(libmat);
	cout<<"\n"<<"Creating a Book object to print()\n";
	Book b("The Castle","Franz Kafka");
	print(b); 
	cout<<"\n"<<"Creating an AudioBook object to print()\n";
	//AudioBook ab("Man without qualities","Robert Musil","Kenneth Meyer"); 
	AudioBook ab("Mason and Dixon","Thomas Pynchon","Edwin Leonard");
	cout<<"The title is "<<ab.title() <<'\n'
	    <<"The author is "<<ab.author() <<'\n'
	    <<"The narrator is "<<ab.narrator() <<endl;
}
