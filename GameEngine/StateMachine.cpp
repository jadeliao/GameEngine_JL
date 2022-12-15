#include "StateMachine.h"
#include "State.h"
#include "Transition.h"
#include "Action.h"

StateMachine::~StateMachine(){}

void StateMachine::Update(){

	Ref<Transition> triggered = nullptr;
	//Loop through the transition in the current state to check if any transition is triggered
	for (Ref<Transition> transition_ : currentState->getTransitions()) {
		if (transition_->isTriggered()) {
			triggered = transition_;
			break;
		}
	}
	//Perform actions when the transition is triggered
	if (triggered) {
		currentState = triggered->getTargetState();
		//Perform action for the current state
		if (currentState) {
			currentState->getAction()->Perform();
		}
	}

}

void StateMachine::setInitialState(Ref<State> state_){
	initialState = state_;
	currentState = initialState;
	initialState->getAction()->Perform();
}
