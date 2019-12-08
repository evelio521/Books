#ifndef _CONTEXT_H_ 
#define _CONTEXT_H_
class State;

class Context

{
public:
	Context();
	Context(State* st);
	~Context();

	void OperationInterface();
	void OperationChangeState();

private:
	friend class State;
	bool ChangeState(State* st);
	State* _state;
};
#endif
