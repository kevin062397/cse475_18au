#include "Ambient2.h"
#include "Debug.h"

constexpr uint8_t Ambient2::_localWeights[];

uint8_t Ambient2::getNumRepeats() {
	return 32;
}

void Ambient2::loop(uint32_t dt) {
	dprintln(F("Ambient2ing..."));
}

const uint8_t* Ambient2::getLocalWeights() {
	return this->_localWeights;
}

float Ambient2::getStartleFactor() {
	return 9999999999;
}

bool Ambient2::rxStartle(int8_t rssi, uint8_t len, uint8_t* payload) {}

void Ambient2::PIR() {
	dprintln("PIR triggered!");
}

void Ambient2::startled(uint8_t strength, uint8_t id) {}
