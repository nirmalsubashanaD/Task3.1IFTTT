#include <Wire.h>
#include <BH1750.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>

// WiFi Credentials
#define WIFI_SSID "Home WiFi"
#define WIFI_PASSWORD "akith12345"

// HiveMQ Credentials
#define MQTT_SERVER "e0a61b4b45f149c78c01a7587c9deb7a.s1.eu.hivemq.cloud"
#define MQTT_PORT 8883
#define MQTT_TOPIC "sensor/light"
#define MQTT_USER "hivemq.webclient.1742519149295"  
#define MQTT_PASSWORD ";f!1*be,0ah29AQDCRIg" 

// Light Threshold
#define LIGHT_THRESHOLD 500

// BH1750 Sensor
BH1750 lightMeter;

// WiFi & MQTT Clients
WiFiSSLClient wifiSSLClient;
PubSubClient mqttClient(wifiSSLClient);

// Variables to track light state
bool sunlightDetected = false;

void connectWiFi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("Connecting to WiFi...");
        delay(2000);
    }
    Serial.println("Connected to WiFi");
}

void connectMQTT() {
    while (!mqttClient.connected()) {
        Serial.println("Connecting to MQTT...");
        if (mqttClient.connect("ArduinoNanoIoT", MQTT_USER, MQTT_PASSWORD)) {
            Serial.println("Connected to MQTT Broker!");
        } else {
            Serial.print("Failed, rc=");
            Serial.println(mqttClient.state());
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    Wire.begin();
    lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);

    connectWiFi();
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    connectMQTT();
}

void loop() {
    if (!mqttClient.connected()) {
        connectMQTT();
    }
    mqttClient.loop();

    float lux = lightMeter.readLightLevel();
    Serial.print("Light Intensity: ");
    Serial.print(lux);
    Serial.println(" lx");

    char luxStr[10];
    sprintf(luxStr, "%.2f", lux);

    if (lux > LIGHT_THRESHOLD && !sunlightDetected) {
        mqttClient.publish(MQTT_TOPIC, "Sunlight Detected");
        sunlightDetected = true;
    } 
    else if (lux <= LIGHT_THRESHOLD && sunlightDetected) {
        mqttClient.publish(MQTT_TOPIC, "Sunlight Stopped");
        sunlightDetected = false;
    }

    delay(5000);
}
