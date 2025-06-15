#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>

// LoRa module pin connections for ESP8266 or ESP32
#define SS    15   // LoRa NSS (Chip Select) -> GPIO15 (D8)
#define RST   16   // LoRa RESET -> GPIO16 (D0)
#define DIO0  5    // LoRa DIO0 -> GPIO5 (D1)
#define SCK   14   // LoRa SCK -> GPIO14 (D5)
#define MISO  12   // LoRa MISO -> GPIO12 (D6)
#define MOSI  13   // LoRa MOSI -> GPIO13 (D7)

// DHT22 Sensor Configuration
#define DHT_PIN     2  // GPIO4 (D4) -> DHT22 DATA
#define DHT_TYPE DHT22

// Onboard LED for ESP8266 or ESP32
#define LED_BUILTIN 2  // Built-in LED for ESP32/ESP8266 (GPIO2)

DHT dht(DHT_PIN, DHT_TYPE);  // Initialize DHT sensor

unsigned long lastSendTime = 0;  // Variable to store the last time the packet was sent
unsigned long sendInterval = 2000;  // Send data every 3 seconds (3000 milliseconds)

void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);  // Make sure the LED is off initially

    Serial.println("ESP8266/ESP32 LoRa Sender with DHT22");

    // Use the correct pins for the LoRa module
    LoRa.setPins(SS, RST, DIO0);  // NSS, RESET, DIO0

    if (!LoRa.begin(433E6)) { // 433 MHz frequency
        Serial.println("LoRa init failed!");
        while (1);  // Halt if initialization fails
    }

    Serial.println("LoRa Init Successful!");

    // Initialize DHT22 sensor
    dht.begin();
    Serial.println("DHT22 Init Successful!");
}

void loop() {
    unsigned long currentMillis = millis();  // Get the current time

    // Only send data once every 5 seconds
    if (currentMillis - lastSendTime >= sendInterval) {
        lastSendTime = currentMillis;  // Update the last send time

        // Get the temperature and humidity from DHT22
        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();

        // Check if the reading is valid
        if (isnan(temperature) || isnan(humidity)) {
            Serial.println("Failed to read from DHT sensor! Retrying...");
            return;
        }

        // Format data as a string to send via LoRa
        String payload = "Temperature: " + String(temperature, 1) + "°C, Humidity: " + String(humidity, 1) + "%";
        Serial.print("Sending packet: ");
        Serial.println(payload);

        // Blink LED before sending data
        digitalWrite(LED_BUILTIN, HIGH);  // Turn on the LED
        delay(200);  // Wait for 200ms
        digitalWrite(LED_BUILTIN, LOW);   // Turn off the LED

        // Send the data over LoRa
        LoRa.beginPacket();
        LoRa.print(payload);
        LoRa.endPacket();
    }
}
