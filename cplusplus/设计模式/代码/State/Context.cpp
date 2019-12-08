#include "Context.h"
#include "State.h"

Context::Context()
{
}
Context::Context(State *st)
{
	_state=st;
}
Context::~Context()
{
	delete _state;
}

void Context::OperationInterface()
{
	_state->OperationInterface(this);
}
bool Context::ChangeState(State *st)
{
	this->_state=st;
	return true;
}

void Context::OperationChangeState()
{
	_state->OperationChangeState(this);
}