/*
 * Relay.cpp
 *
 *  Created on: 18 Mar 2026
 *      Author: jondurrant
 */

#include "Relay.h"
#include "GPIOInputMgr.h"

Relay::Relay(uint8_t gp, uint8_t sw) {
	xGP = gp;
	xSW = sw;

	gpio_init(xGP);
	gpio_set_dir(xGP, GPIO_OUT);

	if (xSW <= 28 ){
		gpio_pull_up(xSW);
		GPIOInputMgr::getMgr()->addObserver(xSW, this);
	}


}

Relay::~Relay() {
	// TODO Auto-generated destructor stub
}



void Relay::on(bool on){
	xState = on;
	gpio_put(xGP, on);
	if (pObs != NULL){
		pObs->relayState(this, xGP, xState);
	}
}
void Relay::off(){
	on(false);
}

bool Relay::getState(){
	return xState;
}

void Relay::setObserver(RelayObserver *obs){
	pObs = obs;
}


void Relay::handleGPIO(uint gpio, uint32_t events){
	if ((gpio == xSW) && ((events & GPIO_IRQ_EDGE_FALL) > 0)){
		uint32_t now = to_ms_since_boot (get_absolute_time());

		if ((now - xPress) > RELAY_SWITCH_BOUNCE){
			xPress = now;
			on(!xState);
		}
	}
}
