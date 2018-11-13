#ifndef _ACTIVE3_H_
#define _ACTIVE3_H_

#include "Creature.h"
#include "State.h"

class Active3 : public State {
public:
	Active3(Creature& creature) : State(creature, "Active 3", ACTIVE3) {}
	uint8_t getNumRepeats();
	void loop(uint32_t dt);
	const uint8_t* getLocalWeights();
	float getStartleFactor();
protected:
	bool rxStartle(int8_t rssi, uint8_t len, uint8_t* payload);
	void PIR();
	void startled(uint8_t strength, uint8_t id);
private:
	static constexpr uint8_t _localWeights[ACTIVE_STATES + AMBIENT_STATES] = { 0 };
};

#endif // _ACTIVE3_H_
