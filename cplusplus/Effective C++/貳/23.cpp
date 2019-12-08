#include "webbrowser.h"
#include "webbrowserbookmarks.h"
#include "webbrowsercookies.h"

using namespace WebbrowserStuff;

int main(){
	
	WebBrowser web;
	clearBorowser(web);
	BookMarks();
	Cookies();
}
