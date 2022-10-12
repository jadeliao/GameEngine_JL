#ifndef LOOKWHEREYOUREGOING_H
#define LOOKWHEREYOUREGOING_H

#include "Align.h"

class LookWhereYoureGoing : public Align {
private:

public:

	LookWhereYoureGoing(Ref<Body> character_, Ref<Body> target_);
	virtual ~LookWhereYoureGoing();

	Ref<SteeringOutput> getSteering() override;

};


#endif

