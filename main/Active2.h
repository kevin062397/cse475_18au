#ifndef _ACTIVE2_H_
#define _ACTIVE2_H_

#include "Creature.h"
#include "State.h"

class Active2 : public State
{
  public:
	Active2(Creature &creature) : State(creature, "Active 2", ACTIVE2) {}
	uint8_t getNumRepeats();
	void loop(uint32_t dt);
	const uint8_t *getLocalWeights();
	float getStartleFactor();

  private:
	static constexpr uint8_t _localWeights[ACTIVE_STATES + AMBIENT_STATES] = {2, 4, 2, 1, 2, 4};
};

#endif // _ACTIVE2_H_
