#include "Ambient3.h"
#include "Midi.h"
#include "Neopixel.h"

constexpr uint8_t Ambient3::_localWeights[];

uint8_t Ambient3::getNumRepeats()
{
	return random(8, 17); // 8 - 16
}

void Ambient3::loop(uint32_t dt)
{
	Midi::setSound(random(0x14, 0x16));
	Neopixel::setLight(5);
}

const uint8_t *Ambient3::getLocalWeights()
{
	return this->_localWeights;
}

float Ambient3::getStartleFactor()
{
	return 0.0005f;
}