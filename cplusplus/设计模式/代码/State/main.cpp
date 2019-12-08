#include "Context.h" 
#include "State.h"
#include <iostream>
using namespace std;

int main(int argc,char* argv[])

{

	State *st=new ConcreteStateA();
	Context* con=new Context(st);
	//con->ChangeState(new ConcreteStateB());
	con->OperationChangeState();
	con->OperationChangeState();
	con->OperationChangeState();
	
	if (con != NULL) delete con;
	if (st != NULL) st = NULL;


}