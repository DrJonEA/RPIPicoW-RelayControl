/*
 * MQTTRouterRelay.h
 *
 *  Created on: 4 Oct 2022
 *      Author: jondurrant
 */

#ifndef _MQTTROUTERRelay_H_
#define _MQTTROUTERRelay_H_

#include "pico/stdlib.h"
#include "MQTTRouter.h"
#include "MQTTRouterTwin.h"

#include "tiny-json.h"
#include "RelayAgent.h"

#define MQTT_Relay_REQ_TOPIC 	"Relay/req"

class MQTTRouterRelay : public MQTTRouterTwin{
public:
	/***
	 * Constructor
	 */
	MQTTRouterRelay(RelayAgent *agent);
	virtual ~MQTTRouterRelay();

	/***
	 * Use the interface to setup all the subscriptions
	 * @param interface
	 */
	virtual void subscribe(MQTTInterface *interface);

	/***
	 * Route the message the appropriate part of the application
	 * @param topic
	 * @param topicLen
	 * @param payload
	 * @param payloadLen
	 * @param interface
	 */
	virtual void route(const char *topic, size_t topicLen, const void * payload,
			size_t payloadLen, MQTTInterface *interface);



private:
	RelayAgent *pAgent = NULL;
	char *pLedTopic = NULL;

};


#endif /*_MQTTROUTERRelay_H_ */
