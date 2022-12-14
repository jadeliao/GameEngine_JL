#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

class State;
class Actor;

class StateMachine{

private:
	Ref<State> initialState;
	Ref<State> currentState;
	Ref<Actor> owner;

public:
	StateMachine(): owner(nullptr), initialState(nullptr), currentState(nullptr) {}
	~StateMachine();
	void Update();
	void setInitialState(Ref<State> state_);
	void setOwner(Ref<Actor> owner_) { owner = owner_; }
	Ref<State> getCurrentState() { return currentState; }
};

#endif

