#include <Arduino.h>

#include <ESP8266WebServer.h>
#include <LittleFS.h>

#include <ESP8266HTTPUpdateServer.h>
#include <ArduinoOTA.h>

#include <iomanip>
#include <sstream>

#include "MqttSupport.hpp"
#include "Scale.hpp"
#include "CO2ScaleLogic.hpp"

using namespace std;

ESP8266WebServer http(80);
ESP8266HTTPUpdateServer updateServer;

const ScaleHardware ScaleHw {
	.dataPin = 4,
	.clockPin = 0,
	.gain = 128,
};

const ScaleConfiguration ScaleConfig {
	.tare = -339543,
	// .calibrationFactor = -9142.3f,
	.calibrationFactor = -9479.97f,
	
	// calculate rolling averages over the last minute, reading twice a second
	.sampleIntervalMs = 500,
	.sampleCount = 120,
};

Scale scale(ScaleConfig);

CO2ScaleLogic co2(
	CO2ScaleConfiguration {
		.TankWeight = 26.4f, // 26.4 pounds tank tare weight
		.AdditionalWeight = 2.3f, // 2.3 pounds of measured weight from regulator/etc
	},
	scale
);

MqttConfiguration config = {
	.clientName = "co2-scale",
	.server = "icefog.vortex",
	.port = 1883,
	.username = NULL,
	.password = NULL,
	
	.publishIntervalMs = 30000,
	.weightTopic = "brewery/co2/weight",
};

MqttSupport mqtt(co2, config);

void setup() {
	Serial.begin(9600);

	ArduinoOTA.begin();

	WiFi.mode(WIFI_STA);
	WiFi.hostname("CO2-Scale");
	WiFi.persistent(false);
	WiFi.setAutoReconnect(true);
	WiFi.begin("geosynchronous", "8pp9wpje65zeyaqydr");

	scale.setup(ScaleHw);

	Serial.println("Connecting to wifi...");

	while (!WiFi.isConnected()) {
		Serial.print(".");
		delay(500);
	}

	Serial.print("Connected! IP: ");
	Serial.println(WiFi.localIP());

	Serial.println("Starting web server...");
	http.begin();

	updateServer.setup(&http, "/update");

	http.on("/", HTTP_GET, []() {
		stringstream response;

		auto co2Data = co2.getCurrentPayload();

		const auto currentScaleConfig = scale.configuration();
		const auto currentCO2Config = co2.configuration();

		response
			<< fixed << setprecision(2)
			<< "<!DOCTYPE html>" << endl
			<< "<html lang=\"en\">" << endl
			<< "	<head><title>CO2 Scale</title></head>" << endl
			<< "	<body>" << endl
			<< "		<h1>CO<sub>2</sub> Scale</h1>" << endl
			<< "		<div><strong>Current Estimated CO2 Level:</strong> " << co2Data.CO2Weight << " pounds</div>" << endl
			<< "		<div>Current reading: " << co2Data.TotalWeight << " pounds (" << co2Data.ReadingAgeMillis << "ms ago)</div>" << endl
			<< "		<div>CO2 Tank Weight: " << currentCO2Config.TankWeight << " pounds plus " << currentCO2Config.AdditionalWeight << " additional pounds</div>" << endl
			<< "		<div>Raw Tare: " << currentScaleConfig.tare << " | Calibration Factor: " << currentScaleConfig.calibrationFactor << "</div>" << endl
			<< "		<div><form method=\"post\" action=\"/cmd/tare\"><input type=\"submit\" value=\"tare\"></form></div>" << endl
			<< "		<div>Calibrate: <form method=\"post\" action=\"/cmd/calibrate\"><input type=\"text\" name=\"calibration\" /> <input type=\"submit\" value=\"calibrate\"></form></div>" << endl
			<< "	</body>" << endl
			<< "</html>";

		http.sendHeader("Cache-Control", "no-cache");
		http.send(200, "text/html", response.str().c_str());
	});

	http.on("/cmd/tare", HTTP_POST, []() {
		Serial.println("received tare request");

		scale.tare();

		http.sendHeader("Location", "/");
		http.sendHeader("Cache-Control", "no-cache");
		http.send(303);
	});

	http.on("/cmd/calibrate", HTTP_POST, []() {
		Serial.println("received calibration request");

		if (http.hasArg("calibration")) {
			scale.calibrate(http.arg("calibration").toFloat());
		}

		http.sendHeader("Location", "/");
		http.sendHeader("Cache-Control", "no-cache");
		http.send(303);
	});

	http.on("/x/stats", HTTP_GET, []() {
		http.sendHeader("Cache-Control", "no-cache");
		http.send(200, "application/json", co2.getCurrentPayload().toJson());
	});

	http.on("/x/info", HTTP_GET, []() {
		StaticJsonDocument<256> doc;
		doc["last_reset_reason"] = ESP.getResetReason();
		doc["free_heap"] = ESP.getFreeHeap();
		doc["heap_fragmentation"] = ESP.getHeapFragmentation();

		stringstream response;
		serializeJson(doc, response);

		http.sendHeader("Cache-Control", "no-cache");
		http.send(200, "application/json", response.str().c_str());
	});

	LittleFS.begin();

	http.on("/test.html", HTTP_GET, []() {
		auto file = LittleFS.open("/test.html", "r");
		http.streamFile(file, "text/html");
		file.close();
	});

	mqtt.setup();

}

void loop() {

	ArduinoOTA.handle();
	http.handleClient();
	scale.loop();
	mqtt.loop();

}