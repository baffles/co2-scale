#ifndef CO2ScaleLogic_hpp
#define CO2ScaleLogic_hpp

#include <ostream>
#include <ArduinoJson.h>
#include "Scale.hpp"

struct CO2ScaleConfiguration {
	/** The tare weight of the tank, in units of calibration. */
	float TankWeight;

	/** Any extra tare weight, e.g., regulators and hoses. */
	float AdditionalWeight;
};

struct CO2ScaleData {
	/** The total weight of everything (tank, accessories, CO2). */
	float TotalWeight;

	/** The weight of CO2 estimated to be in the tank. */
	float CO2Weight;

	/** The age of data used, at generation time of this payload, in ms. */
	int ReadingAgeMillis;

	/** Form the JSON for this payload. */
	auto toJson() const -> const String;

	/** Serialize the JSON for this scale data to the provided stream. */
	auto serializeJsonTo(std::ostream &output) const -> void;

private:

	auto buildJsonDocument(JsonDocument &doc) const -> void;

};

/** Handles the extra logic for dealing with scale readings for a CO2 cylinder on the scale. This
  * includes taring for the tank/regulator/hoses (beyond the base scale tare).
  */
class CO2ScaleLogic {
public:

	CO2ScaleLogic(CO2ScaleConfiguration config, const Scale &scale);

	auto configuration()       ->       CO2ScaleConfiguration&;
	auto configuration() const -> const CO2ScaleConfiguration&;

	auto getCurrentPayload() const -> const CO2ScaleData;

private:

	CO2ScaleConfiguration _configuration;
	const Scale &scale;

};

#endif