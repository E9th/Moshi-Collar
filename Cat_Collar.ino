#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <DHT.h>

// Define Firebase Data and API Key
#define FIREBASE_HOST "https://cat-monitoring-project-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "AIzaSyB3oiAFNvqXvT0KmV0AtD_40l5dGcG0Lm8"

// Define WiFi credentials
#define WIFI_SSID "Earth"
#define WIFI_PASSWORD "eyeearth"

// Initialize Firebase Data
FirebaseData firebaseData;

// DHT11 Sensor
#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// HC-SR04 Sensor
#define TRIGPIN D5
#define ECHOPIN D6

// Active Buzzer
#define BUZZER_PIN D7

// Threshold values
const float TEMP_COMFORT_LOW = 24.0;
const float TEMP_COMFORT_HIGH = 28.0;
const float TEMP_WARNING = 29.0;
const float TEMP_CRITICAL = 33.0;
const float HUMIDITY_COMFORT_LOW = 40.0;
const float HUMIDITY_COMFORT_HIGH = 55.0;
const float HUMIDITY_WARNING = 55.0;
const float HUMIDITY_CRITICAL = 65.0;

// Timing
unsigned long lastMeasurement = 0;
const unsigned long measurementInterval = 10000; // every 10 seconds

void setup() {
  Serial.begin(115200);

  // Initialize DHT sensor
  dht.begin();

  // Initialize Buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Initialize HC-SR04
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Connect to Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  if (millis() - lastMeasurement > measurementInterval) {
    lastMeasurement = millis();
    
    // Read DHT11 sensor
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Read HC-SR04 sensor
    long duration, distance;
    digitalWrite(TRIGPIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGPIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGPIN, LOW);
    duration = pulseIn(ECHOPIN, HIGH);
    distance = duration * 0.0344 / 2;

    // Determine status
    String status;
    float heatStrokePercentage;
    if (temperature >= TEMP_CRITICAL || humidity >= HUMIDITY_CRITICAL) {
      status = "Danger";
      heatStrokePercentage = 100.0;
      digitalWrite(BUZZER_PIN, HIGH);
    } else if (temperature >= TEMP_WARNING || (humidity >= HUMIDITY_WARNING && humidity < HUMIDITY_CRITICAL)) {
      status = "Hot";
      heatStrokePercentage = map(temperature, TEMP_WARNING, TEMP_CRITICAL, 0, 100);
      digitalWrite(BUZZER_PIN, LOW);
    } else if ((temperature >= TEMP_COMFORT_LOW && temperature <= TEMP_COMFORT_HIGH) &&
               (humidity >= HUMIDITY_COMFORT_LOW && humidity <= HUMIDITY_COMFORT_HIGH)) {
      status = "Comfort";
      heatStrokePercentage = 0.0;
      digitalWrite(BUZZER_PIN, LOW);
    } else {
      status = "Normal";
      heatStrokePercentage = 0.0;
      digitalWrite(BUZZER_PIN, LOW);
    }

    // Send data to Firebase
    Firebase.setFloat(firebaseData, "/temperature", temperature);
    Firebase.setFloat(firebaseData, "/humidity", humidity);
    Firebase.setFloat(firebaseData, "/distance", distance);
    Firebase.setString(firebaseData, "/status", status);
    Firebase.setFloat(firebaseData, "/heatStrokePercentage", heatStrokePercentage);
    
    if (firebaseData.errorReason() != "") {
      Serial.println("Firebase error: " + firebaseData.errorReason());
    }
  }
}
