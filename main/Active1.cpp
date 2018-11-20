#include "Active1.h"
#include "Midi.h"

constexpr uint8_t Active1::_localWeights[];

uint8_t Active1::getNumRepeats()
{
	return random(4, 9); // 4 - 8
}

void Active1::loop(uint32_t dt)
{
	playSound(getId());
	playEffect(getId());
}

const uint8_t *Active1::getLocalWeights()
{
	return this->_localWeights;
}

float Active1::getStartleFactor()
{
	return 0.0015f;
}
