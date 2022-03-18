#ifndef MqttSupport_hpp
#define MqttSupport_hpp

#include <EspMQTTClient.h>

#include "CO2ScaleLogic.hpp"

struct MqttConfiguration {
	const char *clientName;
	const char *server;
	const short port;
	const char *username;
	const char *password;

	const int publishIntervalMs;
	const char *weightTopic;
};

//const MqttConfiguration DefaultMqttConfiguration;

/** Provides MQTT support for periodically publishing the scale statistics */
class MqttSupport {
public:

	MqttSupport(const CO2ScaleLogic &co2, MqttConfiguration config);
			

	void setup();
	void loop();

private:

	EspMQTTClient mqtt;

	const CO2ScaleLogic &co2;
	MqttConfiguration config;

	void scheduleMqttPublish();

};

#endif