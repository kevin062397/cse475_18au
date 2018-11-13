#include "Ambient3.h"
#include "Debug.h"

constexpr uint8_t Ambient3::_localWeights[];

uint8_t Ambient3::getNumRepeats() {
	return 32;
}

void Ambient3::loop(uint32_t dt) {
	dprintln(F("Ambient3ing..."));
}

const uint8_t* Ambient3::getLocalWeights() {
	return this->_localWeights;
}

float Ambient3::getStartleFactor() {
	return 9999999999;
}

bool Ambient3::rxStartle(int8_t rssi, uint8_t len, uint8_t* payload) {}

void Ambient3::PIR() {
	dprintln("PIR triggered!");
}

void Ambient3::startled(uint8_t strength, uint8_t id) {}
