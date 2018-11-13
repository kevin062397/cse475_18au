#ifndef _AMBIENT3_H_
#define _AMBIENT3_H_

#include "Creature.h"
#include "State.h"

class Ambient3 : public State {
public:
	Ambient3(Creature& creature) : State(creature, "Ambient 3", AMBIENT3) {}
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

#endif // _AMBIENT3_H_
