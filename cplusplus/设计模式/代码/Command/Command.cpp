#include "command.h"
#include "Reciever.h"
#include <iostream>
Command::Command()
{
}
Command::~Command()
{
}

ConcreteCommand::ConcreteCommand(Reciever *rev)
{
	this->rev=rev;
}
ConcreteCommand::~ConcreteCommand()
{
	
}

void ConcreteCommand::Excute()

{
	
	std::cout<<"ConcreteCommand..."<<std::endl;
	rev->Action();
}