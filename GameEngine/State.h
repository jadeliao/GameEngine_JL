#ifndef STATE_H
#define STATE_H

#include <memory>
#include <vector>

template<typename T>
using Ref = std::shared_ptr<T>;

class Transition;
class Action;

class State{

private:
	std::vector<Ref<Transition>> transitions;
	Ref<Action> action;

public:

	State(){}
	~State(){}
	std::vector<Ref<Transition>> getTransitions() { return transitions; }
	void addTransition(Ref<Transition> transition_);
	void setAction(Ref<Action> action_) { action = action_; }
	Ref<Action> getAction() { return action; }
};

#endif
