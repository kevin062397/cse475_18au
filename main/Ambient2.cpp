#include "Ambient2.h"
#include "Midi.h"

constexpr uint8_t Ambient2::_localWeights[];

uint8_t Ambient2::getNumRepeats()
{
	return random(8, 17); // 8 - 16
}

void Ambient2::loop(uint32_t dt)
{
	playSound(getId());
	playEffect(getId());
}

const uint8_t *Ambient2::getLocalWeights()
{
	return this->_localWeights;
}

float Ambient2::getStartleFactor()
{
	return 0.0005f;
}