#ifndef TRANSITION_H
#define TRANSITION_H


#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

class State;
class Condition;

class Transition{

private:
	Ref<State> targetState;
	Ref<Condition> condition;

public:
	Transition(Ref<State> targetState_, Ref<Condition> condition_): targetState(targetState_), condition(condition_){}
	~Transition(){}
	bool isTriggered();
	Ref<State> getTargetState() { return targetState; }

};

#endif