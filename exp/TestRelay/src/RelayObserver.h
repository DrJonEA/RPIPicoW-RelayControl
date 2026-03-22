/*
 * RelayObserver.h
 *
 *  Created on: 18 Mar 2026
 *      Author: jondurrant
 */

#ifndef SUBSYSTEMS_REXP1_POWER_EXPV2_RELAYOBSERVER_H_
#define SUBSYSTEMS_REXP1_POWER_EXPV2_RELAYOBSERVER_H_

#include "pico/stdlib.h"

class Relay;

class RelayObserver {
public:
	RelayObserver();
	virtual ~RelayObserver();

	virtual void relayState(Relay *r, uint8_t gp, bool state) = 0;
};

#endif /* SUBSYSTEMS_REXP1_POWER_EXPV2_TESTRELAYS_SRC_RELAYOBSERVER_H_ */
