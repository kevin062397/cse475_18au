#include "Midi.h"
#include "Neopixel.h"
#include "Startle.h"

constexpr uint8_t Startle::_localWeights[];

uint8_t Startle::getNumRepeats()
{
	return random(1, 5); // 1 - 4
}

void Startle::loop(uint32_t dt)
{
	Midi::setSound(18);
	Neopixel::setLight(2);
}

const uint8_t *Startle::getLocalWeights()
{
	return this->_localWeights;
}

float Startle::getStartleFactor()
{
	return 9999999999;
}

bool Startle::rxStartle(uint8_t len, uint8_t *payload) {}

void Startle::PIR() {}

void Startle::startled() {}
