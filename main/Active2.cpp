#include "Active2.h"
#include "Midi.h"
#include "Neopixel.h"

constexpr uint8_t Active2::_localWeights[];

uint8_t Active2::getNumRepeats()
{
	return random(4, 9); // 4 - 8
}

void Active2::loop(uint32_t dt)
{
	Midi::setSound(random(0x20, 0x25));
	Neopixel::setLight(7);
}

const uint8_t *Active2::getLocalWeights()
{
	return this->_localWeights;
}

float Active2::getStartleFactor()
{
	return 0.0015f;
}