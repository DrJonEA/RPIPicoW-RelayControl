/*
 * RelayControllerState.cpp
 *
 *  Created on: 22 Mar 2026
 *      Author: jondurrant
 */

#include "RelayControllerState.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "json-maker/json-maker.h"

RelayControllerState::RelayControllerState() {
	elements=4;

	jsonHelpers[ONSLOT] = (StateFunc)&RelayControllerState::jsonOn;
}

RelayControllerState::~RelayControllerState() {
	// NOP
}

/***
 * Copy constructor
 * @param other
 */
RelayControllerState::RelayControllerState(const RelayControllerState &other): StateTemp(other) {
	on = other.getOn();
}

/***
 * Set On status of switch
 * @param bool
 */
void RelayControllerState::setOn(bool b){
	on = b;
	setDirty(ONSLOT);
}

/***
 * Get on status of switch
 * @return
 */
bool RelayControllerState::getOn() const{
	return on;
}

/***
 * Retrieve on status in JSON format
 * @param buf
 * @param len
 * @return
 */
char* RelayControllerState::jsonOn(char *buf, unsigned int len){
	char *p = buf;
    p = json_bool( p, "on", getOn(), &len);
    return p;
}

/***
* Update state data from a json structure
* @param json
*/
void RelayControllerState::updateFromJson(json_t const *json){
	StateTemp::updateFromJson(json);

	json_t const *jp;
	uint8_t newRGB[3]={0, 0, 0};
	bool rgbOK = true;

	jp = json_getProperty(json, "on");
	if (jp){
		if (JSON_BOOLEAN == json_getType(jp)){
			bool b = json_getBoolean(jp);
			setOn(b);
		}
	}
}

/***
 * Retrieve state of object in JSON format
 * @param buf - buffer to write to
 * @param len - length of buffer
 * @return length of json or zero if we ran out of space
 */
unsigned int RelayControllerState::state(char *buf, unsigned int len){
	updateTemp();
	return StateTemp::state(buf, len);
}


