#include "Ambient1.h"
#include "Midi.h"
#include "Neopixel.h"

constexpr uint8_t Ambient1::_localWeights[];

uint8_t Ambient1::getNumRepeats()
{
	return random(8, 17); // 8 - 16
}

void Ambient1::loop(uint32_t dt)
{
	Midi::setSound(random(0, 2) ? random(0x12, 0x14) : random(0x16, 0x19) );
	Neopixel::setLight(3);
}

const uint8_t *Ambient1::getLocalWeights()
{
	return this->_localWeights;
}

float Ambient1::getStartleFactor()
{
	return 0.0005f;
}