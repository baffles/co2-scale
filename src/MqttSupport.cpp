#include "MqttSupport.hpp"

// global forced on us; not used
void onConnectionEstablished() {
}

MqttSupport::MqttSupport(const CO2ScaleLogic &co2, MqttConfiguration config) :
	mqtt(config.server, config.port, config.username, config.password, config.clientName),
	co2(co2), config(config) {
}

void MqttSupport::setup() {
	mqtt.setOnConnectionEstablishedCallback([this]() {
		scheduleMqttPublish();
	});
}

void MqttSupport::loop() {
	mqtt.loop();
}

void MqttSupport::scheduleMqttPublish() {
	mqtt.executeDelayed(config.publishIntervalMs, [this]() {
		mqtt.publish(config.weightTopic, co2.getCurrentPayload().toJson());
		scheduleMqttPublish();
	});
}
