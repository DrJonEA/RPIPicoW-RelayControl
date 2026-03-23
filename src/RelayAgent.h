/*
 * RelayAgent.h
 *
 * Manage Relay and latched switch software behaviour
 *
 *  Created on: 23 Oct 2022
 *      Author: jondurrant
 */

#ifndef _RelayAGENT_H_
#define _RelayAGENT_H_


#include "Agent.h"
#include "RelayObserver.h"
#include "Relay.h"
#include "tiny-json.h"

#include "pico/stdlib.h"
#include "queue.h"
#include "message_buffer.h"
#include "MQTTConfig.h"
#include "MQTTInterface.h"

#include "RelayControllerState.h"
#include "StateObserver.h"

#define Relay_QUEUE_LEN 	5
#define MQTT_TOPIC_Relay_STATE "Relay/state"
#define Relay_BUFFER_LEN 	256
#define Relay_JSON_LEN 	20
#define Relay_JSON_POOL 	5


class RelayAgent : public Agent, public RelayObserver, public StateObserver {
public:
	/***
	 * Constructor
	 * @param relayGP - GPIO Pad of Relay to control
	 * @param spstGP - GPIO Pad of SPST non latched switch
	 * @param interface - MQTT Interface that state will be notified to
	 * @param relayState - state object for the device
	 */
	RelayAgent(
			uint8_t relayGP,
			uint8_t spstGP,
			MQTTInterface *interface,
			RelayControllerState *relayState
			);

	/***
	 * Destructor
	 */
	virtual ~RelayAgent();

	/***
	 * Set the states of the Relay to - on
	 * @param on - boolean if the Relay should be on or off
	 */
	void setOn(bool on);

	/***
	 * Toggle the state of the Relay. so On becomes Off, etc.
	 */
	void toggle();


	/***
	 * Add a JSON string action
	 * @param jsonStr
	 */
	void addJSON(const void  *jsonStr, size_t len);


	/***
	 * Notifcation that Relay state has changed
	 * @param r Relay that was changed
	 * @param gp The Relays GP port
	 * @param state the new state of the relay
	 */
	virtual void relayState(Relay *r, uint8_t gp, bool state);

	/***
	 * Notification of a change of a state item with the State object.
	 * @param dirtyCode - Representation of item changed within state. Used to pull back delta
	 */
	virtual void notifyState(uint16_t dirtyCode);

protected:
	/***
	 * Task main run loop
	 */
	virtual void run();

	/***
	 * Get the static depth required in words
	 * @return - words
	 */
	virtual configSTACK_DEPTH_TYPE getMaxStackSize();

private:
	/***
	 * Toggle Relay state from within an intrupt
	 */
	void intToggle();

	/***
	 * Execute the state on the Relay and notify MQTT interface
	 * @param state
	 */
	void execRelay(bool state);

	/***
	 * Parse a JSON string and add request to queue
	 * @param str - JSON Strging
	 */
	void parseJSON(char *str);

	/***
	 * Notify MQTT topic of state change
	 */
	void notify();

	//Interface to publish state to MQTT
	MQTTInterface *pInterface = NULL;

	// Topic to publish on
	char * pTopicRelayState = NULL;

	//Relay and switch pads
	uint8_t xRelayGP;
	uint8_t xSpstGP;




	//Queue of commands
	QueueHandle_t xCmdQ;

	// Message buffer handle
	MessageBufferHandle_t xBuffer = NULL;

	// Json decoding buffer
	json_t pJsonPool[ Relay_JSON_POOL ];


	Relay *pRelay = NULL;
	RelayControllerState *pState = NULL;

};


#endif /* _RelayAGENT_H_ */
