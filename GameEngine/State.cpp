#include "State.h"

void State::addTransition(Ref<Transition> transition_){

	transitions.push_back(transition_);

}
