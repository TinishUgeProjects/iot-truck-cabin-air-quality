#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_DEVICE_NAME "TruckCabinAirMonitor"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// ---- PIN CONFIGURATION ----
#define DHTPIN D2
#define DHTTYPE DHT11
#define MQ135PIN A0
#define COPIN A0  // If using analog-only CO sensor on same pin, otherwise assign different

// ---- OBJECTS ----
DHT dht(DHTPIN, DHTTYPE);
char ssid[] = "YourWiFiSSID";         
char pass[] = "YourWiFiPassword";     

BlynkTimer timer;

// ---- SENSOR READING FUNCTION ----
void readSensors() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int airQualityRaw = analogRead(MQ135PIN); // MQ135 analog value
  int coLevel = analogRead(COPIN);          // CO sensor analog value

  // --- Simple Calibration Logic (Customize as needed) ---
  String airQualityStatus;
  if (airQualityRaw < 300) airQualityStatus = "Good";
  else if (airQualityRaw < 600) airQualityStatus = "Moderate";
  else airQualityStatus = "Poor";

  // --- Sending Data to Blynk Dashboard ---
  Blynk.virtualWrite(V0, temperature); 
  Blynk.virtualWrite(V1, humidity);    
  Blynk.virtualWrite(V2, airQualityRaw); 
  Blynk.virtualWrite(V3, coLevel);       
  Blynk.virtualWrite(V4, airQualityStatus); 

  Serial.print("Temp: "); Serial.print(temperature); Serial.print("°C, ");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.print("%, ");
  Serial.print("Air Quality: "); Serial.print(airQualityRaw); Serial.print(" (");
  Serial.print(airQualityStatus); Serial.print("), ");
  Serial.print("CO Level: "); Serial.println(coLevel);
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();
  timer.setInterval(3000L, readSensors); // Every 3 seconds
}

void loop() {
  Blynk.run();
  timer.run();
}
