#include "Startle.h"

constexpr uint8_t Startle::_localWeights[];

uint8_t Startle::getNumRepeats()
{
	return random(1, 5); // 1 - 4
}

void Startle::loop(uint32_t dt) {
	playSound(getId());
	playEffect(getId());
}

const uint8_t *Startle::getLocalWeights()
{
	return this->_localWeights;
}

float Startle::getStartleFactor()
{
	return 9999999999;
}

bool Startle::rxStartle(int8_t rssi, uint8_t len, uint8_t *payload) {}

void Startle::PIR() {}

void Startle::startled(uint8_t strength, uint8_t id) {}
