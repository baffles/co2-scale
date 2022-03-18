// #ifndef Configuration_hpp
// #define Configuration_hpp

// #include "MqttSupport.hpp"
// #include "Scale.hpp"

// /** Handles storage of configuration in the EEPROM. */
// class Configuration {
// public:

// 	Configuration();

// 	void load();
// 	void save() const;

// 	auto scaleConfiguration()       ->       ScaleConfiguration&;
// 	auto scaleConfiguration() const -> const ScaleConfiguration&;

// 	auto mqttConfiguration()       ->       MqttConfiguration&;
// 	auto mqttConfiguration() const -> const MqttConfiguration&;

// private:

// 	/** Current version; increments as things are added. */
// 	const byte CurrentVersion = 1;

// 	ScaleConfiguration _scaleConfig;
// 	MqttConfiguration _mqttConfig;

// };

// #endif