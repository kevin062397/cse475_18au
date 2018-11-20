#include "Neopixel.h"
#include "Debug.h"

// Initialize fixed list of light gestures.
constexpr void (*Neopixel::LIGHTS[])(uint32_t);

uint8_t Neopixel::_currentIdx = 0;
uint32_t Neopixel::_lastLoop = 0;
Adafruit_NeoPixel_ZeroDMA Neopixel::_strip = Adafruit_NeoPixel_ZeroDMA(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRBW);

void Neopixel::setup()
{
	_strip.begin();
	_strip.setBrightness(5);
	_strip.show();
}

void Neopixel::loop()
{
	uint32_t thisLoop = millis();
	uint32_t dt;
	if (_lastLoop)
	{
		dt = thisLoop - _lastLoop;
	}
	else
	{
		dt = 0;
	}
	_lastLoop = thisLoop;

	if (LIGHTS[_currentIdx] != nullptr)
	{
		LIGHTS[_currentIdx](dt);
	}
}

void Neopixel::setLight(uint8_t lightIdx)
{
	// lightIdx soundIdx. Anything outside of the bounds of the array is 0.
	if (lightIdx >= sizeof(LIGHTS) / sizeof(void *))
	{
		lightIdx = 0;
	}

	if (_currentIdx != lightIdx)
	{
		_strip.clear();
		_strip.show();

		_currentIdx = lightIdx;
	}
}

uint8_t Neopixel::getLight()
{
	return _currentIdx;
}

void Neopixel::rainbow(uint32_t dt)
{
	static uint8_t offset = 0;
	static uint32_t rainbowColors[NEOPIXEL_COUNT] = {16711680, 13578240, 10444800, 7311360, 4177920, 1044480, 56865, 44625,
													 32385, 20145, 7905, 1179885, 4325565, 7471245, 10616925, 13762605};

	if (dt > 50)
	{
		for (uint8_t pix = 0; pix < NEOPIXEL_COUNT; pix++)
		{
			_strip.setPixelColor((pix + offset) % NEOPIXEL_COUNT, rainbowColors[pix]);
		}
		_strip.show();
		offset++;
	}
}

// Custom Startle
void Neopixel::startle(uint32_t dt) {
	int numPixels = _strip.numPixels();
	for (int j = 0; j < 256; j++) {
		for (int i = 0; i < numPixels; i++) {
			int r = 255 / numPixels * (i + numPixels / 3 * 0) % 255;
			int g = 255 / numPixels * (i + numPixels / 3 * 1) % 255;
			int b = 255 / numPixels * (i + numPixels / 3 * 2) % 255;
			_strip.setPixelColor(i, r * j / 255, g * j / 255, b * j / 255, j / 2);
		}
		_strip.show();
		delay(10);
	}
}

// Custom Ambient 1
void Neopixel::ambient1(uint32_t dt) {
	int wait = 10;
	int numPixels = _strip.numPixels();
	for (int j = 64; j < 256; j++) {
		for (int i = 0; i < numPixels; i++) {
			_strip.setPixelColor(i, j, j, j, j);
		}
		_strip.show();
		delay(wait);
	}
	for (int j = 255; j >= 64; j--) {
		for (int i = 0; i < numPixels; i++) {
			_strip.setPixelColor(i, j, j, j, j);
		}
		_strip.show();
		delay(wait);
	}
}

// Custom Ambient 2
void Neopixel::ambient2(uint32_t dt) {
	int numPixels = _strip.numPixels();
	for (int j = 0; j < numPixels; j++) {
		for (int i = 0; i < numPixels; i++) {
			int light1 = j;
			int light2 = (j - 1) % numPixels;
			int light3 = (j + 1) % numPixels;
			int brightness = 0;
			if (i == light1 || i == light2 || i == light3) {
				brightness = 255;
			}
			_strip.setPixelColor(i, brightness, brightness, brightness, brightness);
		}
		_strip.show();

		int delayFactor = abs(j - numPixels / 2);
		delay(delayFactor * 20);
	}
}

// Custom Ambient 3
void Neopixel::ambient3(uint32_t dt) {
	int numPixels = _strip.numPixels();
	for (int j = 0; j < numPixels; j++) {
		for (int i = 0; i < numPixels; i++) {
			int brightness = 0;
			if ((i + j) % 4 == 0) {
				brightness = 255;
			}
			_strip.setPixelColor(i, brightness, brightness, brightness, brightness);
		}
		_strip.show();
		delay(200);
	}
}

// Custom Active 1
void Neopixel::active1(uint32_t dt) {
	int numPixels = _strip.numPixels();
	int brightness = 255;
	for (int i = 0; i < numPixels; i++) {
		_strip.setPixelColor(i, brightness, brightness, brightness, brightness);
	}
	_strip.show();
	delay(50);
	brightness = 0;
	for (int i = 0; i < numPixels; i++) {
		_strip.setPixelColor(i, brightness, brightness, brightness, brightness);
	}
	_strip.show();
	delay(500);
}

// Custom Active 2
void Neopixel::active2(uint32_t dt) {
	int numPixels = _strip.numPixels();
	for (int i = 0; i < numPixels; i++) {
		int r = random(256);
		int g = random(256);
		int b = random(256);
		_strip.setPixelColor(i, r, g, b);
	}
	_strip.show();
	delay(200);
}

// Custom Active 3
void Neopixel::active3(uint32_t dt) {
	int wait = 2;
	int numPixels = _strip.numPixels();
	int r = 255;
	int g = 0;
	int b = 0;
	for (int j = 0; j < 255; j++) {
		for (int i = 0; i < numPixels; i++) {
			_strip.setPixelColor(i, r, g, b);
		}
		g++;
		_strip.show();
		delay(wait);
	}
	for (int j = 0; j < 255; j++) {
		for (int i = 0; i < numPixels; i++) {
			_strip.setPixelColor(i, r, g, b);
		}
		r--;
		_strip.show();
		delay(wait);
	}
	for (int j = 0; j < 255; j++) {
		for (int i = 0; i < numPixels; i++) {
			_strip.setPixelColor(i, r, g, b);
		}
		b++;
		_strip.show();
		delay(wait);
	}
	for (int j = 0; j < 255; j++) {
		for (int i = 0; i < numPixels; i++) {
			_strip.setPixelColor(i, r, g, b);
		}
		g--;
		_strip.show();
		delay(wait);
	}
	for (int j = 0; j < 255; j++) {
		for (int i = 0; i < numPixels; i++) {
			_strip.setPixelColor(i, r, g, b);
		}
		r++;
		_strip.show();
		delay(wait);
	}
	for (int j = 0; j < 255; j++) {
		for (int i = 0; i < numPixels; i++) {
			_strip.setPixelColor(i, r, g, b);
		}
		b--;
		_strip.show();
		delay(wait);
	}
}
