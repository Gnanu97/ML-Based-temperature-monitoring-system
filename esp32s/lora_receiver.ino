#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define SS    5
#define RST   14
#define DIO0  26
#define SCK   18
#define MISO  19
#define MOSI  23

#define LED_BUILTIN 2  // ESP32 built-in LED

const char* ssid = "Nothing Phone (2a)";
const char* password = "Gnanu@2005";
const char* serverURL = "http://192.168.148.234:5000/data";

// Data arrays
float temperatureData[100];
float humidityData[100];
int dataIndex = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // WiFi Connection
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize LoRa
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed!");
    while (1);
  }
  Serial.println("LoRa Init Successful!");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);

    String incoming = "";
    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }

    int tempIndex = incoming.indexOf("Temperature:");
    int humidityIndex = incoming.indexOf("Humidity:");

    if (tempIndex != -1 && humidityIndex != -1) {
      String temperature = incoming.substring(tempIndex + 12, incoming.indexOf("\xC2\xB0C"));
      String humidity = incoming.substring(humidityIndex + 10, incoming.indexOf("%"));

      temperatureData[dataIndex] = temperature.toFloat();
      humidityData[dataIndex] = humidity.toFloat();

      int rssi = LoRa.packetRssi();
      Serial.printf("Entry #%d: Temp = %.1f C, Humidity = %.1f%%, RSSI = %d dBm\n", dataIndex + 1, temperatureData[dataIndex], humidityData[dataIndex], rssi);

      dataIndex++;
      if (dataIndex >= 100) {
        triggerAction();
        clearData();
      }
    } else {
      Serial.println("Invalid data format. Skipping this packet.");
    }
  }
  delay(100);
}

void triggerAction() {
  Serial.println("Sending collected data to Raspberry Pi...");

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverURL);
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"temperature\": [";
    for (int i = 0; i < 100; i++) {
      payload += String(temperatureData[i]);
      if (i < 99) payload += ",";
    }
    payload += "], \"humidity\": [";
    for (int i = 0; i < 100; i++) {
      payload += String(humidityData[i]);
      if (i < 99) payload += ",";
    }
    payload += "]}";

    int httpResponseCode = http.POST(payload);
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response from Raspberry Pi:");
      Serial.println(response);

      StaticJsonDocument<256> doc;
      DeserializationError error = deserializeJson(doc, response);
      if (!error) {
        float avgTemp = doc["prediction"]["avg_temperature"];
        float avgHumi = doc["prediction"]["avg_humidity"];

        Serial.print("Predicted Avg Temp: ");
        Serial.println(avgTemp);
        Serial.print("Predicted Avg Humi: ");
        Serial.println(avgHumi);

        if (avgTemp > 30.0) {
          Serial.println("Warning: High Temperature!");
        }
      } else {
        Serial.print("Failed to parse response: ");
        Serial.println(error.c_str());
      }
    } else {
      Serial.printf("Failed to send data. Code: %d\n", httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi not connected. Cannot send data.");
  }
}

void clearData() {
  for (int i = 0; i < 100; i++) {
    temperatureData[i] = 0;
    humidityData[i] = 0;
  }
  dataIndex = 0;
  Serial.println("Data cleared. Ready for next cycle.");
}
