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
void Neopixel::startle(uint32_t dt)
{
	static uint8_t loopCounter = 0;
	if (dt > 10)
	{
		for (uint8_t i = 0; i < NEOPIXEL_COUNT; i++)
		{
			uint8_t r = 255 / NEOPIXEL_COUNT * (i + NEOPIXEL_COUNT / 3 * 0) % 255;
			uint8_t g = 255 / NEOPIXEL_COUNT * (i + NEOPIXEL_COUNT / 3 * 1) % 255;
			uint8_t b = 255 / NEOPIXEL_COUNT * (i + NEOPIXEL_COUNT / 3 * 2) % 255;
			_strip.setPixelColor(i, r * loopCounter / 255, g * loopCounter / 255, b * loopCounter / 255, loopCounter / 2);
		}
		_strip.show();
		loopCounter++;
	}
}

// Custom Ambient 1
void Neopixel::ambient1(uint32_t dt)
{
	static uint8_t loopCounter = 64;
	static bool incr = true;
	if (dt > 10)
	{
		for (uint8_t i = 0; i < NEOPIXEL_COUNT; i++)
		{
			_strip.setPixelColor(i, loopCounter, loopCounter, loopCounter, loopCounter);
		}
		_strip.show();

		loopCounter += incr ? 1 : -1;
		if (loopCounter == 64 || loopCounter == 255)
		{
			incr = !incr;
		}
	}
}

// Custom Ambient 2
void Neopixel::ambient2(uint32_t dt)
{
	static uint8_t loopCounter = 0;
	uint16_t delayFactor = abs(loopCounter - NEOPIXEL_COUNT / 2) * 20;
	if (dt > delayFactor)
	{
		for (uint8_t i = 0; i < NEOPIXEL_COUNT; i++)
		{
			uint8_t light1 = loopCounter;
			uint8_t light2 = (loopCounter - 1) % NEOPIXEL_COUNT;
			uint8_t light3 = (loopCounter + 1) % NEOPIXEL_COUNT;
			uint8_t brightness = 0;
			if (i == light1 || i == light2 || i == light3)
			{
				brightness = 255;
			}
			_strip.setPixelColor(i, brightness, brightness, brightness, brightness);
		}
		_strip.show();

		loopCounter++;
		if (loopCounter == NEOPIXEL_COUNT)
		{
			loopCounter = 0;
		}
	}
}

// Custom Ambient 3
void Neopixel::ambient3(uint32_t dt)
{
	static uint8_t loopCounter = 0;
	if (dt > 200)
	{
		for (uint8_t i = 0; i < NEOPIXEL_COUNT; i++)
		{
			uint8_t brightness = 0;
			if ((i + loopCounter) % 4 == 0)
			{
				brightness = 255;
			}
			_strip.setPixelColor(i, brightness, brightness, brightness, brightness);
		}
		_strip.show();

		loopCounter++;
		if (loopCounter == NEOPIXEL_COUNT)
		{
			loopCounter = 0;
		}
	}
}

// Custom Active 1
void Neopixel::active1(uint32_t dt)
{
	if (dt > 500)
	{
		uint8_t brightness = 255;
		for (uint8_t i = 0; i < NEOPIXEL_COUNT; i++)
		{
			_strip.setPixelColor(i, brightness, brightness, brightness, brightness);
		}
		_strip.show();
		delay(50);
		brightness = 0;
		for (uint8_t i = 0; i < NEOPIXEL_COUNT; i++)
		{
			_strip.setPixelColor(i, brightness, brightness, brightness, brightness);
		}
		_strip.show();
	}
}

// Custom Active 2
void Neopixel::active2(uint32_t dt)
{
	if (dt > 200)
	{
		for (uint8_t i = 0; i < NEOPIXEL_COUNT; i++)
		{
			uint8_t r = random(256);
			uint8_t g = random(256);
			uint8_t b = random(256);
			_strip.setPixelColor(i, r, g, b);
		}
		_strip.show();
	}
}

// Custom Active 3
void Neopixel::active3(uint32_t dt)
{
	static uint16_t loopCounter = 0;
	static uint16_t r = 255;
	static uint16_t g = 0;
	static uint16_t b = 0;

	if (dt > 2)
	{
		for (int i = 0; i < NEOPIXEL_COUNT; i++)
		{
			_strip.setPixelColor(i, r, g, b);
		}
		_strip.show();

		switch (++loopCounter % 256)
		{
		case 0:
			g++;
			break;
		case 1:
			r--;
			break;
		case 2:
			b++;
			break;
		case 3:
			g--;
			break;
		case 4:
			r++;
			break;
		case 5:
			b--;
			break;
		default:
			loopCounter = 1;
			g++;
			break;
		}
	}
}
