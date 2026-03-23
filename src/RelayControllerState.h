/*
 * RelayControllerState.h
 *
 *  Created on: 22 March 2026
 *      Author: jondurrant
 */

#ifndef _RELATCTROLLERSTATE_H_
#define _RELATCTROLLERSTATE_H_

#include "StateTemp.h"
#include <stdbool.h>
#include "pico/stdlib.h"


#define ONSLOT  3


class RelayControllerState : public StateTemp{
public:
	RelayControllerState();
	RelayControllerState(const RelayControllerState &other);
	virtual ~RelayControllerState();
	/***
	 * Gets the On status of the Relay
	 * @return
	 */
	bool getOn() const;

	/***
	 * Sets the on status of the Relay
	 * @param b
	 */
	void setOn(bool b);

	/***
	 * Update state data from a json structure
	 * @param json
	 */
	virtual void updateFromJson(json_t const *json);

	/***
	 * Retrieve state of object in JSON format
	 * @param buf - buffer to write to
	 * @param len - length of buffer
	 * @return length of json or zero if we ran out of space
	 */
	virtual unsigned int state(char *buf, unsigned int len) ;


protected:
	/***
	 * Retried On status in JSON format
	 * @param buf
	 * @param len
	 * @return
	 */
	char* jsonOn(char *buf, unsigned int len);

private:

	//Is light on
	bool on = false;

};


#endif /* _RELATCTROLLERSTATE_H_ */
