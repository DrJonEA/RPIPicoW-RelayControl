/*
 * RelayLeds.h
 *
 *  Created on: 18 Mar 2026
 *      Author: jondurrant
 */

#ifndef SUBSYSTEMS_REXP1_POWER_EXPV2_TESTRELAYS_SRC_RELAYLEDS_H_
#define SUBSYSTEMS_REXP1_POWER_EXPV2_TESTRELAYS_SRC_RELAYLEDS_H_

#include "RelayObserver.h"
#include <PicoLed.hpp>

#define LED_COUNT 4

class RelayLeds : public RelayObserver{
public:
	RelayLeds();
	virtual ~RelayLeds();

	void setLeds(PicoLed::PicoLedController *ctr);

	virtual void relayState(Relay *r, uint8_t gp, bool state);

	void map(uint8_t gp, uint8_t ledIndex);

private:
	PicoLed::PicoLedController *pLeds;

	uint8_t xMapping[LED_COUNT];
};

#endif /* SUBSYSTEMS_REXP1_POWER_EXPV2_TESTRELAYS_SRC_RELAYLEDS_H_ */
