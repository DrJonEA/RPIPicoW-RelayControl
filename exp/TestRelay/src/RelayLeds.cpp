/*
 * RelayLeds.cpp
 *
 *  Created on: 18 Mar 2026
 *      Author: jondurrant
 */

#include "RelayLeds.h"

RelayLeds::RelayLeds() {
	for (int i=0; i <LED_COUNT; i++){
		xMapping[i] =0xFF;
	}
}

RelayLeds::~RelayLeds() {
	// TODO Auto-generated destructor stub
}

void RelayLeds::relayState(Relay *r, uint8_t gp, bool state){
	if (pLeds == NULL)
		return;
	for (int i=0; i <LED_COUNT; i++){
		if (xMapping[i] == gp){
			if (state){
				pLeds->setPixelColor(i,  PicoLed::RGB(0x00, 0xff, 0x00 ));
			} else {
				pLeds->setPixelColor(i,  PicoLed::RGB(0xff, 0x00, 0x00 ));
			}
		}
	}
}


void RelayLeds::setLeds(PicoLed::PicoLedController *ctr){
	pLeds = ctr;
}

void RelayLeds::map(uint8_t gp, uint8_t ledIndex){
	if (ledIndex < LED_COUNT){
		xMapping[ledIndex] = gp;
		pLeds->setPixelColor(ledIndex,  PicoLed::RGB(0x00, 0x00, 0xff ));
	}
}
