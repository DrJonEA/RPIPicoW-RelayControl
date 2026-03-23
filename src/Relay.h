/*
 * Relay.h
 *
 *  Created on: 18 Mar 2026
 *      Author: jondurrant
 */

#ifndef SUBSYSTEMS_REXP1_POWER_EXPV2_TESTRELAYS_SRC_RELAY_H_
#define SUBSYSTEMS_REXP1_POWER_EXPV2_TESTRELAYS_SRC_RELAY_H_

#include "pico/stdlib.h"
#include "RelayObserver.h"
#include "GPIOObserver.h"

#define RELAY_SWITCH_BOUNCE 200

class Relay : public GPIOObserver{
public:
	Relay(uint8_t gp, uint8_t sw = 0xFF);
	virtual ~Relay();

	void on(bool on = true);
	void off();
	bool getState();

	void setObserver(RelayObserver *obs);


	virtual void handleGPIO(uint gpio, uint32_t events);

private:
	uint8_t xGP;
	uint8_t xSW;
	uint32_t xPress = 0;

	bool xState = false;

	RelayObserver *pObs = NULL;


};

#endif /* SUBSYSTEMS_REXP1_POWER_EXPV2_TESTRELAYS_SRC_RELAY_H_ */
