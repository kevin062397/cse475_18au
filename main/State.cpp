#include <math.h>
#include "Creature.h"
#include "Debug.h"
#include "State.h"

State::State(Creature& creature, char* const name, const uint8_t id) : _creature(creature), _id(id) {
	strncpy(_name, name, MAX_NAME_LEN);
	_name[MAX_NAME_LEN] = 0;
};

uint8_t State::getId() {
	return _id;
}

char* State::getName() {
	return _name;
}

void State::playSound(uint8_t sound_idx) {
	// TODO: implement
}

void State::playEffect(uint8_t effect_idx) {
	// TODO: implement
}

bool State::rxPlaySound(uint8_t len, uint8_t* payload) {
	// TODO: implement
}

bool State::rxPlayEffect(uint8_t len, uint8_t* payload) {
	// TODO: implement
}

bool State::rxStartle(int8_t rssi, uint8_t len, uint8_t* payload) {
	double x = 1.0 * (_creature.GLOBALS.STARTLE_DECAY - rssi) / _creature.GLOBALS.STARTLE_DECAY;
	double sigma = 1.0 / (1 + exp(-x));
	double decay = sigma * getStartleFactor();
	this->startled((uint8_t)decay * payload[0], payload[1]);
	return true;
}

void State::txStartle(uint8_t strength, uint8_t id) {
	uint8_t payload[] = {strength, id};
	_creature.tx(PID_STARTLE, BROADCAST_ADDR, sizeof(payload), payload);
}

State* State::transition() {
	// TODO: implement
}

void State::PIR() {
	uint8_t startleRandom = random(_creature.GLOBALS.STARTLE_RAND_MIN, _creature.GLOBALS.STARTLE_RAND_MAX);
	uint8_t strength = min(255, startleRandom * (1 - 0.5 * 255 / _creature.GLOBALS.STARTLE_THRESHOLD + 1));
	this->startled(strength, random(0, 255));
}

void State::startled(uint8_t strength, uint8_t id) {
	if (id != _creature.getLastStartleId()) {
		uint64_t time = millis();
		_creature.setStartleThreshold(_creature.getStartleThreshold() - _creature.getStartleThreshold() * (time - _creature.getLastStartle()) * _creature.GLOBALS.STARTLE_THRESHOLD_DECAY * this->getStartleFactor());
		if (strength >= _creature.GLOBALS.STARTLE_THRESHOLD) {
			dprintln("Will enter Startle state.");
			this->txStartle(strength, id);
			_creature.setLastStartleId(id);
		}
		_creature.setLastStartle(time);
	}
}

int8_t* State::getGlobalWeights() {
	return _globalWeights;
}

uint8_t random(uint8_t lower, uint8_t upper) {
	return rand() % (upper - lower + 1) + lower;
}
