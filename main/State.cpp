#include <math.h>
#include "Creature.h"
#include "Midi.h"
#include "Neopixel.h"
#include "Startle.h"
#include "State.h"

State::State(Creature &creature, char *const name, const uint8_t id) : _creature(creature), _id(id)
{
	strncpy(_name, name, MAX_NAME_LEN);
	_name[MAX_NAME_LEN] = 0;
};

uint8_t State::getId()
{
	return _id;
}

char *State::getName()
{
	return _name;
}

void State::playSound(uint8_t sound_idx)
{
	bool soundExists = true;
	switch (sound_idx)
	{
	case STARTLE:
		Midi::setSound(18);
		break;
	case AMBIENT1:
		Midi::setSound(19);
		break;
	case AMBIENT2:
		Midi::setSound(20);
		break;
	case AMBIENT3:
		Midi::setSound(21);
		break;
	case ACTIVE1:
		Midi::setSound(22);
		break;
	case ACTIVE2:
		Midi::setSound(23);
		break;
	case ACTIVE3:
		Midi::setSound(24);
		break;
	default:
		soundExists = false;
		break;
	}

	if (soundExists)
	{
		Serial.print("Playing sound ");
		Serial.print(sound_idx);
		Serial.println("...");
	}
	else
	{
		Serial.print("No sound of ID ");
		Serial.println(sound_idx);
	}
}

void State::playEffect(uint8_t effect_idx)
{
	bool effectExists = true;
	switch (effect_idx)
	{
	case STARTLE:
		Neopixel::setLight(2);
		break;
	case AMBIENT1:
		Neopixel::setLight(3);
		break;
	case AMBIENT2:
		Neopixel::setLight(4);
		break;
	case AMBIENT3:
		Neopixel::setLight(5);
		break;
	case ACTIVE1:
		Neopixel::setLight(6);
		break;
	case ACTIVE2:
		Neopixel::setLight(7);
		break;
	case ACTIVE3:
		Neopixel::setLight(8);
		break;
	default:
		effectExists = false;
		break;
	}

	if (effectExists)
	{
		Serial.print("Playing effect ");
		Serial.print(effect_idx);
		Serial.println("...");
	}
	else
	{
		Serial.print("No effect of ID ");
		Serial.println(effect_idx);
	}
}

bool State::rxPlaySound(uint8_t len, uint8_t *payload)
{
	if (len < 1)
	{
		return false;
	}
	playSound(payload[0]);
	return true;
}

bool State::rxPlayEffect(uint8_t len, uint8_t *payload)
{
	if (len < 1)
	{
		return false;
	}
	playEffect(payload[0]);
	return true;
}

bool State::rxStartle(int8_t rssi, uint8_t len, uint8_t *payload)
{
	if (len != 2)
	{
		return false;
	}
	double x = 1.0 * (_creature.GLOBALS.STARTLE_DECAY - rssi) / _creature.GLOBALS.STARTLE_DECAY;
	double sigma = 1.0 / (1.0 + exp(-x));
	double decay = sigma * getStartleFactor();
	startled((uint8_t)round(decay * payload[0]), payload[1]);
	return true;
}

void State::txStartle(uint8_t strength, uint8_t id)
{
	uint8_t payload[] = {strength, id};
	_creature.tx(PID_STARTLE, BROADCAST_ADDR, sizeof(payload), payload);
}

State *State::transition()
{
	// Get total number of active creatures (i.e. they've recently communicated & are not in Wait or Startle)
	// Get the total number of creatures in each state
	// Get the total sum of the inverse absolute value of the RSSI
	uint8_t numActiveCreature = 0;
	uint8_t stateSums[ACTIVE_STATES + AMBIENT_STATES] = {0};
	float distanceStateSums[ACTIVE_STATES + AMBIENT_STATES] = {0};
	for (uint8_t i = 1; i < _creature.GLOBALS.NUM_CREATURES + 1; i++)
	{
		if (_creature.getCreatureStates()[i] > 0 && _creature.getCreatureStates()[i] <= (ACTIVE_STATES + AMBIENT_STATES))
		{
			numActiveCreature += 1;
			float creatureInverseDistance = _creature.getCreatureDistances()[i] ? -1.f / _creature.getCreatureDistances()[i] : 0;
			stateSums[_creature.getCreatureStates()[i] - 1] += 1;
			distanceStateSums[_creature.getCreatureStates()[i] - 1] += creatureInverseDistance;
		}
	}

	// Calculate the global scalar values taking into account the states of other creatures
	float stateGlobalScalars[ACTIVE_STATES + AMBIENT_STATES] = {0};
	for (uint8_t i = 0; i < ACTIVE_STATES + AMBIENT_STATES; i++)
	{
		stateGlobalScalars[i] = numActiveCreature ? _globalWeights[i] * ((numActiveCreature - stateSums[i]) / (float)numActiveCreature) : 0;
	}

	float stateLikelihoods[ACTIVE_STATES + AMBIENT_STATES] = {0};
	for (uint8_t i = 0; i < ACTIVE_STATES + AMBIENT_STATES; i++)
	{
		stateLikelihoods[i] = getLocalWeights()[i] + stateGlobalScalars[i] * distanceStateSums[i];
	}

	Serial.print(stateLikelihoods[0]);
	Serial.print("\t");
	for (uint8_t i = 1; i < ACTIVE_STATES + AMBIENT_STATES; i++)
	{
		stateLikelihoods[i] += stateLikelihoods[i - 1];
		Serial.print(stateLikelihoods[i]);
		Serial.print("\t");
	}
	Serial.println();

	float randomVal = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (stateLikelihoods[ACTIVE_STATES + AMBIENT_STATES - 1])));

	uint8_t stateID = 0;
	for (uint8_t i = 0; i < ACTIVE_STATES + AMBIENT_STATES; i++)
	{
		if (randomVal < stateLikelihoods[i])
		{
			stateID = i + 1;
			break;
		}
	}

	Serial.print(randomVal);
	Serial.print(" --> ");
	Serial.println(stateID);

	return _creature.getStateByID(stateID);
}

void State::PIR()
{
	uint8_t startleRandom = random(_creature.GLOBALS.STARTLE_RAND_MIN, _creature.GLOBALS.STARTLE_RAND_MAX + 1);
	uint8_t strength = min(255, startleRandom * (1.0 - 0.5 * 255.0 / _creature.GLOBALS.STARTLE_THRESHOLD + 1.0));
	startled(strength, random(0, 256));
}

void State::startled(uint8_t strength, uint8_t id)
{
	if (id != _creature.getLastStartleId())
	{
		_creature.updatedThreshold();
		if (strength >= _creature.getStartleThreshold())
		{
			_creature.setNextState(new Startle(_creature));
			txStartle(strength, id);
			_creature.setLastStartleId(id);
			_creature.setStartleThreshold(255);
		}
	}
}

int8_t *State::getGlobalWeights()
{
	return _globalWeights;
}
