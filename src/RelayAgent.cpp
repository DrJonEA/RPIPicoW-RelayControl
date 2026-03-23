/*
 * RelayAgent.cpp
 *
 * Manage Relay and latched switch software behaviour
 *
 *  Created on: 23 Oct 2022
 *      Author: jondurrant
 */

#include "RelayAgent.h"
#include "MQTTTopicHelper.h"

//Local enumerator of the actions to be queued
enum RelayAction {RelayOff, RelayOn, RelayToggle};

/***
 * Constructor
 * @param relayGP - GPIO Pad of Relay to control
 * @param spstGP - GPIO Pad of SPST non latched switch
 * @param interface - MQTT Interface that state will be notified to
 * @param relayState - state object for the device
 */
RelayAgent::RelayAgent(
		uint8_t relayGP,
		uint8_t spstGP,
		MQTTInterface *interface,
		RelayControllerState *relayState
		){
	xRelayGP  = relayGP;
	xSpstGP = spstGP;
	pInterface = interface;
	pState = relayState;

	//Initialise the Relay
	pRelay = new Relay(xRelayGP, xSpstGP);
	pRelay->setObserver(this);

	// Queue for actions commands for the class
	xCmdQ = xQueueCreate( Relay_QUEUE_LEN, sizeof(RelayAction));
	if (xCmdQ == NULL){
		LogError(("Unable to create Queue\n"));
	}

	//Construct a message buffer
	xBuffer = xMessageBufferCreate(Relay_BUFFER_LEN);
	if (xBuffer == NULL){
		LogError(("Buffer could not be allocated\n"));
	}

	//Construct the TOPIC for status messages
	if (pInterface != NULL){
		if (pTopicRelayState == NULL){
			pTopicRelayState = (char *)pvPortMalloc( MQTTTopicHelper::lenThingTopic(pInterface->getId(), MQTT_TOPIC_Relay_STATE));
			if (pTopicRelayState != NULL){
				MQTTTopicHelper::genThingTopic(pTopicRelayState, pInterface->getId(), MQTT_TOPIC_Relay_STATE);
			} else {
				LogError( ("Unable to allocate topic") );
			}
		}
	}

	pState->attach(this);

}

/***
 * Destructor
 */
RelayAgent::~RelayAgent() {
	if (pRelay != NULL){
		delete pRelay;
	}
	if (xCmdQ != NULL){
		vQueueDelete(xCmdQ);
	}
	if (pTopicRelayState != NULL){
		vPortFree(pTopicRelayState);
		pTopicRelayState = NULL;
	}
	if (xBuffer != NULL){
		vMessageBufferDelete(xBuffer);
	}
}


void RelayAgent::relayState(Relay *r, uint8_t gp, bool state){
	setOn(state);
}


/***
 * Set the states of the Relay to - on
 * @param on - boolean if the Relay should be on or off
 */
void RelayAgent::setOn(bool on){
	RelayAction action = RelayOff;

	if (on){
		action = RelayOn;
	}

	BaseType_t res = xQueueSendToBack(xCmdQ, (void *)&action, 0);
	if (res != pdTRUE){
		LogWarn(("Queue is full\n"));
	}
}


/***
 * Toggle the state of the Relay. so On becomes Off, etc.
 */
void RelayAgent::toggle(){
	RelayAction action = RelayToggle;
	BaseType_t res = xQueueSendToBack(xCmdQ, (void *)&action, 0);
	if (res != pdTRUE){
		LogWarn(("Queue is full\n"));
	}
}

/***
 * Toggle Relay state from within an intrupt
 */
void RelayAgent::intToggle(){
	RelayAction action = RelayToggle;
	BaseType_t res = xQueueSendToFrontFromISR(xCmdQ, (void *)&action, NULL);
	if (res != pdTRUE){
		LogWarn(("Queue is full\n"));
	}
}


/***
  * Main Run Task for agent
  */
void RelayAgent::run(){
	BaseType_t res;
	RelayAction action = RelayOff;
	char jsonStr[Relay_JSON_LEN];
	size_t readLen;

	if (xCmdQ == NULL){
		return;
	}

	while (true) { // Loop forever
		readLen = xMessageBufferReceive(
						 xBuffer,
				         jsonStr,
						 Relay_JSON_LEN,
						 0
				    );
		if (readLen > 0){
			jsonStr[readLen] = 0;
	        parseJSON(jsonStr);
		}

		res = xQueueReceive(xCmdQ, (void *)&action, 0);
		if (res == pdTRUE){
			switch(action){
				case RelayOff:{
					execRelay(false);
					break;
				}
				case RelayOn:{
					execRelay(true);
					break;
				}
				case RelayToggle:{
					execRelay(!pState->getOn());
					break;
				}
			}
			taskYIELD();
		}
	}
}


/***
 * Execute the state on the Relay and notify MQTT interface
 * @param state
 */
void RelayAgent::execRelay(bool state){

	if (state != pRelay->getState()){
		pRelay->on(state);
		notify();
	}

	if (state != pState->getOn()){
		pState->setOn(state);
	}
}

/***
 * Notify MQTT topic of state change
 */
void RelayAgent::notify(){
	char payload[16];
	if (pRelay->getState()){
		sprintf(payload, "{\"on\"=True}");
	} else {
		sprintf(payload, "{\"on\"=False}");
	}
	if (pInterface != NULL){
		pInterface->pubToTopic(
			pTopicRelayState,
			payload,
			strlen(payload),
			1,
			false
			);
	}
}



/***
 * Get the static depth required in words
 * @return - words
 */
 configSTACK_DEPTH_TYPE RelayAgent::getMaxStackSize(){
	 return 250;
 }


/***
* Parse a JSON string and add request to queue
* @param str - JSON Strging
*/
void RelayAgent::parseJSON(char *str){
	 json_t const* json = json_create( str, pJsonPool, Relay_JSON_POOL);
	 if ( !json ) {
		 LogError(("Error json create."));
		 return ;
	 }
	 json_t const* on = json_getProperty( json, "on" );
	 if ( !on || JSON_BOOLEAN != json_getType( on ) ) {
		 LogError(("Error, the on property is not found."));
		 return ;
	 }
	 bool b = (int)json_getBoolean( on );

	 setOn(b);
}


/***
 * Add a JSON string action
 * @param jsonStr
 */
void RelayAgent::addJSON(const void  *jsonStr, size_t len){
	if (xBuffer != NULL){
		size_t res = xMessageBufferSend(
			xBuffer,
			jsonStr,
			len,
			0);

		if (res != len){
			LogError(("Fairelay to write"));
		}
	}
}


/***
 * Notification of a change of a state item with the State object.
 * @param dirtyCode - Representation of item changed within state. Used to pull back delta
 */
void RelayAgent::notifyState(uint16_t dirtyCode){
	if ((dirtyCode & ( 1 << ONSLOT)) > 0){
		setOn(pState->getOn());

	}
}
