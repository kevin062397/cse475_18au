#ifndef _STARTLE_H_
#define _STARTLE_H_

#include "Creature.h"
#include "State.h"

class Startle : public State {
public:
	Startle(Creature& creature) : State(creature, "Startle", STARTLE) {}
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

#endif // _STARTLE_H_
