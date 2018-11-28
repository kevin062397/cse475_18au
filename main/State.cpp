#include "Creature.h"
#include "State.h"
#include "Startle.h"
#include "Midi.h"
#include "Neopixel.h"
#include "Debug.h"

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

bool State::rxPlaySound(uint8_t len, uint8_t *payload)
{
	if (len < 1)
	{
		return false;
	}
	Midi::setSound(payload[0]);
	return true;
}

bool State::rxPlayEffect(uint8_t len, uint8_t *payload)
{
	if (len < 1)
	{
		return false;
	}
	Neopixel::setLight(payload[0]);
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

	dprintln(F("State transition weights:"));
	dprint(stateLikelihoods[0]);
	dprint(F("\t"));
	for (uint8_t i = 1; i < ACTIVE_STATES + AMBIENT_STATES; i++)
	{
		stateLikelihoods[i] += stateLikelihoods[i - 1];
		dprint(stateLikelihoods[i]);
		dprint(F("\t"));
	}
	dprintln();

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

	dprint(F("Generated "));
	dprint(randomVal);
	dprint(F(" -->  "));
	dprintln(stateID);

	return _creature.getState(stateID);
}

void State::PIR()
{
	uint8_t startleRandom = random(_creature.GLOBALS.STARTLE_RAND_MIN, _creature.GLOBALS.STARTLE_RAND_MAX + 1);
	uint8_t strength = min(255, startleRandom * (1.0 - 0.5 * 255.0 / _creature.GLOBALS.STARTLE_THRESHOLD + 1.0));
	startled(strength, random(0, 256));
}

void State::startled(uint8_t strength, uint8_t id)
{
	uint8_t last = _creature.getLastStartleId();
	if (id != last)
	{
		_creature.updateThreshold();
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
