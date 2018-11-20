#ifndef _ACTIVE1_H_
#define _ACTIVE1_H_

#include "Creature.h"
#include "State.h"

class Active1 : public State
{
  public:
	Active1(Creature &creature) : State(creature, "Active 1", ACTIVE1) {}
	uint8_t getNumRepeats();
	void loop(uint32_t dt);
	const uint8_t *getLocalWeights();
	float getStartleFactor();

  private:
	static constexpr uint8_t _localWeights[ACTIVE_STATES + AMBIENT_STATES] = {2, 1, 2, 4, 2, 4};
};

#endif // _ACTIVE1_H_
