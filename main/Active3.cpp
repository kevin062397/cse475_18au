#include "Active3.h"
#include "Midi.h"
#include "Neopixel.h"

constexpr uint8_t Active3::_localWeights[];

uint8_t Active3::getNumRepeats()
{
	return random(4, 9); // 4 - 8
}

void Active3::loop(uint32_t dt)
{
	Midi::setSound(random(0x19, 0x20));
	Neopixel::setLight(8);
}

const uint8_t *Active3::getLocalWeights()
{
	return this->_localWeights;
}

float Active3::getStartleFactor()
{
	return 0.0015f;
}