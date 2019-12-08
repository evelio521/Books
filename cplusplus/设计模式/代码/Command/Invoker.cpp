#include "Invoker.h"
#include "Command.h"
#include<iostream>
Invoker::Invoker(Command *cmd)
{
	this->cmd=cmd;

}
Invoker::~Invoker()
{
	
}
void Invoker::Invoke()
{
	std::cout<<"Invoker..."<<std::endl;
	cmd->Excute();
	
}