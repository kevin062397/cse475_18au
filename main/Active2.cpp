#include "Active2.h"
#include "Debug.h"

constexpr uint8_t Active2::_localWeights[];

uint8_t Active2::getNumRepeats() {
	return 32;
}

void Active2::loop(uint32_t dt) {
	dprintln(F("Active2ing..."));
}

const uint8_t* Active2::getLocalWeights() {
	return this->_localWeights;
}

float Active2::getStartleFactor() {
	return 9999999999;
}

bool Active2::rxStartle(int8_t rssi, uint8_t len, uint8_t* payload) {}

void Active2::PIR() {
	dprintln("PIR triggered!");
}

void Active2::startled(uint8_t strength, uint8_t id) {}
