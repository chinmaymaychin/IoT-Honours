#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <WiFi.h>
#include <ThingSpeak.h>

#define PIN_SDA 21
#define PIN_SCL 22

#define PIN_SOIL_SENSOR A0

#define WIFI_SSID "vyas"
#define WIFI_PASSWORD "jrv@2363"
#define THINGSPEAK_CHANNEL_ID 2435670 // Replace 123456 with your actual ThingSpeak channel ID

#define THINGSPEAK_API_KEY "GDYEA434S9NKMNR1"
WiFiClient client;

Adafruit_PCD8544 display = Adafruit_PCD8544(5, 18, 23, 19, 17);

void setup() {
  Serial.begin(115200);
  pinMode(PIN_SOIL_SENSOR, INPUT);

  display.begin();
  display.setContrast(50);
  display.clearDisplay();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  ThingSpeak.begin(client);
}

void loop() {
  int soilMoisture = analogRead(PIN_SOIL_SENSOR);
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("Soil Moisture:");
  display.println(soilMoisture);
  display.display();
  
  Serial.print("Soil Moisture: ");
  Serial.println(soilMoisture);

  ThingSpeak.setField(1, soilMoisture);
  int status = ThingSpeak.writeFields(THINGSPEAK_CHANNEL_ID, THINGSPEAK_API_KEY);
  if (status == 200) {
    Serial.println("Data sent to ThingSpeak successfully");
  } else {
    Serial.println("Problem sending data to ThingSpeak");
  }

  delay(6000); // Wait for 1 minute before sending the next data
}
