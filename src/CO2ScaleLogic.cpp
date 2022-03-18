#include "CO2ScaleLogic.hpp"

#include <ArduinoJson.h>

auto CO2ScaleData::buildJsonDocument(JsonDocument &doc) const -> void {
	doc["weight"] = TotalWeight;
	doc["co2_weight"] = CO2Weight;
	doc["reading_age"] = ReadingAgeMillis;
}

auto CO2ScaleData::toJson() const -> const String {
	StaticJsonDocument<128> payload;
	buildJsonDocument(payload);

	String statusStr;
	serializeJson(payload, statusStr);

	return statusStr;
}

auto CO2ScaleData::serializeJsonTo(std::ostream &output) const -> void {
	StaticJsonDocument<128> payload;
	buildJsonDocument(payload);
	serializeJson(payload, output);
}


CO2ScaleLogic::CO2ScaleLogic(CO2ScaleConfiguration config, const Scale& scale) :
	_configuration(config), scale(scale) {
}

auto CO2ScaleLogic::configuration()       ->       CO2ScaleConfiguration& { return _configuration; }
auto CO2ScaleLogic::configuration() const -> const CO2ScaleConfiguration& { return _configuration; }

auto CO2ScaleLogic::getCurrentPayload() const -> const CO2ScaleData {
	auto currentReading = scale.currentReading();

	return CO2ScaleData {
		.TotalWeight = currentReading,
		.CO2Weight = currentReading - _configuration.TankWeight - _configuration.AdditionalWeight,
		.ReadingAgeMillis = scale.readingAge(),
	};
}
