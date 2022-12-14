#include "Transition.h"
#include "Condition.h"

bool Transition::isTriggered(){
    return condition->test();
}
