// #include "Configuration.hpp"

// #include <EEPROM.h>

// Configuration::Configuration() : _scaleConfig(DefaultScaleConfiguration), _mqttConfig(DefaultMqttConfiguration) {
// }

// void Configuration::load() {
// 	int address = 0;

// 	byte version;
// 	EEPROM.get(address, version);
// 	address += sizeof(version);

// 	// read the scale configuration
// 	if (version >= 1) {
// 		long scaleOffset;
// 		EEPROM.get(address, scaleOffset);
// 		address += sizeof(scaleOffset);

// 		float scaleScale;
// 		EEPROM.get(address, scaleScale);
// 		address += sizeof(scaleScale);

// 		int scaleSampleCount;
// 		EEPROM.get(address, scaleSampleCount);
// 		address += sizeof(scaleSampleCount);

// 		scaleConfiguration() = ScaleConfiguration {
// 			.offset = scaleOffset,
// 			.scale = scaleScale,
// 			.sampleCount = scaleSampleCount,
// 		};

// 		const char *mqttClientName = (const char *)(EEPROM.getDataPtr() + address);
// 		address += strlen(mqttClientName) + 1;

// 		const char *mqttServer = (const char *)(EEPROM.getDataPtr() + address);
// 		address += strlen(mqttServer) + 1;

// 		short mqttPort;
// 		EEPROM.get(address, mqttPort);
// 		address += sizeof(mqttPort);

// 		const char *mqttUsername = EEPROM.read(address) == NULL ? NULL : (const char *)(EEPROM.getDataPtr() + address);
// 		address += (mqttUsername == NULL ? 0 : strlen(mqttUsername)) + 1;

// 		const char *mqttPassword = EEPROM.read(address) == NULL ? NULL : (const char *)(EEPROM.getDataPtr() + address);
// 		address += (mqttPassword == NULL ? 0 : strlen(mqttPassword)) + 1;

// 		int publishIntervalMs

// 		/*struct MqttConfiguration {
// 	char *clientName;
// 	char *server;
// 	short port;
// 	char *username;
// 	char *password;

// 	int publishIntervalMs;
// 	char *weightTopic;
// };*/
// 	}
// }

// void Configuration::save() const {
// }

// auto Configuration::scaleConfiguration() -> ScaleConfiguration& {
// 	return _scaleConfig;
// }

// auto Configuration::scaleConfiguration() const -> const ScaleConfiguration& {
// 	return _scaleConfig;
// }

// auto Configuration::mqttConfiguration() -> MqttConfiguration& {
// 	return _mqttConfig;
// }

// auto Configuration::mqttConfiguration() const -> const MqttConfiguration& {
// 	return _mqttConfig;
// }
