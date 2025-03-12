#include <SoftwareSerial.h>
#include <ArduinoJson.h>

// ESP8266 Serial
#define ESP_RX 2  // Connect to ESP TX
#define ESP_TX 3  // Connect to ESP RX
SoftwareSerial espSerial(ESP_RX, ESP_TX);

// Define which sensors are actually connected
const bool CAR_SENSOR_CONNECTED[3] = {
  true,   // Car Slot 1 is connected
  true,   // Car Slot 2 is connected
  false   // Car Slot 3 not connected
};

const bool MOTORCYCLE_SENSOR_CONNECTED[3] = {
  false,  // Motorcycle Slot 1 not connected
  false,  // Motorcycle Slot 2 not connected
  false   // Motorcycle Slot 3 not connected
};

// Pin definitions for ultrasonic sensors (shifted down by 2)
const int CAR_SENSORS[3][2] = {
  {4, 5},   // Car Slot 1 (TRIG_PIN, ECHO_PIN)
  {6, 7},   // Car Slot 2
  {8, 9}    // Car Slot 3
};

const int MOTORCYCLE_SENSORS[3][2] = {
  {10, 11}, // Motorcycle Slot 1
  {12, 13}, // Motorcycle Slot 2
  {14, 15}  // Motorcycle Slot 3
};

// RGB LED pins for Car 1
const int RGB1_RED = A0;    // Red pin for Car 1
const int RGB1_GREEN = A1;  // Green pin for Car 1
const int RGB1_BLUE = A2;   // Blue pin for Car 1

// RGB LED pins for Car 2
const int RGB2_RED = A3;    // Red pin for Car 2
const int RGB2_GREEN = A4;  // Green pin for Car 2
const int RGB2_BLUE = A5;   // Blue pin for Car 2

// Constants for vehicle detection
const float CAR_THRESHOLD = 100.0;        // Distance in cm for car detection
const float MOTORCYCLE_THRESHOLD = 50.0;  // Distance in cm for motorcycle detection
const int READINGS_COUNT = 3;             // Number of readings to average
const int BULK_UPDATE_INTERVAL = 5000;    // 5 seconds between bulk updates
const float VACANCY_MARGIN = 20.0;        // Margin to prevent flickering

// WiFi settings - replace with your network details
const char* WIFI_SSID = "Egg's Den";
const char* WIFI_PASSWORD = "3ggW@rd.0112";
const char* SERVER_HOST = "192.168.15.105";  // Your computer's local IP
const int SERVER_PORT = 8000;

// Timing variables
unsigned long lastBulkUpdateTime = 0;

void setup() {
  Serial.begin(9600);
  espSerial.begin(115200);
  
  // Setup RGB LED pins for Car 1
  pinMode(RGB1_RED, OUTPUT);
  pinMode(RGB1_GREEN, OUTPUT);
  pinMode(RGB1_BLUE, OUTPUT);
  
  // Setup RGB LED pins for Car 2
  pinMode(RGB2_RED, OUTPUT);
  pinMode(RGB2_GREEN, OUTPUT);
  pinMode(RGB2_BLUE, OUTPUT);
  
  // Initial state - Green (no object detected) for both LEDs
  setLEDColor(1, false);
  setLEDColor(2, false);
  
  // Setup all sensor pins
  for (int i = 0; i < 3; i++) {
    pinMode(CAR_SENSORS[i][0], OUTPUT);      // TRIG
    pinMode(CAR_SENSORS[i][1], INPUT);       // ECHO
    pinMode(MOTORCYCLE_SENSORS[i][0], OUTPUT);
    pinMode(MOTORCYCLE_SENSORS[i][1], INPUT);
  }

  // Initialize ESP8266
  initWiFi();
  
  // Get IP Address after WiFi initialization
  getESP8266IPAddress();
}

void initWiFi() {
  Serial.println("Initializing ESP8266...");
  
  // Reset ESP8266
  sendCommand("AT+RST", 2000);
  
  // Configure as client
  sendCommand("AT+CWMODE=1", 1000);
  
  // Connect to WiFi
  String cwjap = "AT+CWJAP=\"";
  cwjap += WIFI_SSID;
  cwjap += "\",\"";
  cwjap += WIFI_PASSWORD;
  cwjap += "\"";
  sendCommand(cwjap.c_str(), 10000);
  
  // Configure for multiple connections
  sendCommand("AT+CIPMUX=1", 1000);
  
  Serial.println("ESP8266 initialized!");
}

void sendCommand(const char* command, int timeout) {
  Serial.print("Sending command: ");
  Serial.println(command);
  
  espSerial.println(command);
  delay(timeout);
  
  while(espSerial.available()) {
    Serial.write(espSerial.read());
  }
  Serial.println();
}

float getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;  // Convert to centimeters
}

void sendSensorData(const char* data) {
  String cmd = "AT+CIPSTART=0,\"TCP\",\"";
  cmd += SERVER_HOST;
  cmd += "\",";
  cmd += SERVER_PORT;
  sendCommand(cmd.c_str(), 5000);
  
  // Prepare HTTP POST request
  String httpRequest = "POST /api/sensor-reading/ HTTP/1.1\r\n";
  httpRequest += "Host: ";
  httpRequest += SERVER_HOST;
  httpRequest += "\r\n";
  httpRequest += "Content-Type: application/json\r\n";
  httpRequest += "Content-Length: ";
  httpRequest += strlen(data);
  httpRequest += "\r\n\r\n";
  httpRequest += data;
  
  // Send data length command
  cmd = "AT+CIPSEND=0,";
  cmd += httpRequest.length();
  sendCommand(cmd.c_str(), 1000);
  
  // Send the actual data
  sendCommand(httpRequest.c_str(), 5000);
  
  // Close connection
  sendCommand("AT+CIPCLOSE=0", 1000);
}

void setLEDColor(int carNumber, bool objectDetected) {
  int redPin, greenPin, bluePin;
  
  // Select the correct LED pins based on car number
  if (carNumber == 1) {
    redPin = RGB1_RED;
    greenPin = RGB1_GREEN;
    bluePin = RGB1_BLUE;
  } else if (carNumber == 2) {
    redPin = RGB2_RED;
    greenPin = RGB2_GREEN;
    bluePin = RGB2_BLUE;
  } else {
    return; // Invalid car number
  }
  
  if (objectDetected) {
    // Red - object detected
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
  } else {
    // Green - no object detected
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, LOW);
  }
}

void getESP8266IPAddress() {
  Serial.println("Retrieving IP Address...");
  espSerial.println("AT+CIFSR");
  
  // Wait for and print the response
  unsigned long startTime = millis();
  while (millis() - startTime < 5000) {  // 5-second timeout
    if (espSerial.available()) {
      String response = espSerial.readStringUntil('\n');
      Serial.println("IP Response: " + response);
      
      // Look for IP address in the response
      if (response.indexOf("+CIFSR:STAIP,") != -1) {
        Serial.println("IP Address Found!");
        break;
      }
    }
  }
}

void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastBulkUpdateTime >= BULK_UPDATE_INTERVAL) {
    Serial.println("Starting sensor readings...");
    
    // Process car sensors
    for (int i = 0; i < 3; i++) {
      if (!CAR_SENSOR_CONNECTED[i]) {
        Serial.print("Car sensor ");
        Serial.print(i + 1);
        Serial.println(" not connected, skipping");
        continue;
      }
      
      Serial.print("Reading car sensor ");
      Serial.println(i + 1);
      
      float totalDistance = 0;
      int validReadings = 0;
      
      // Take multiple readings
      for (int j = 0; j < READINGS_COUNT; j++) {
        float distance = getDistance(CAR_SENSORS[i][0], CAR_SENSORS[i][1]);
        Serial.print("  Reading ");
        Serial.print(j + 1);
        Serial.print(": ");
        Serial.println(distance);
        
        if (distance > 0 && distance < 400) {
          totalDistance += distance;
          validReadings++;
        }
        delay(10);
      }
      
      // Only send data if we got valid readings
      if (validReadings > 0) {
        float avgDistance = totalDistance / validReadings;
        Serial.print("  Average distance: ");
        Serial.println(avgDistance);
        
        // Set LED color based on object detection
        bool isOccupied = avgDistance < CAR_THRESHOLD;
        setLEDColor(i + 1, isOccupied);  // i+1 gives us car number (1 or 2)
        
        // Create JSON for this sensor
        JsonDocument doc;
        doc["sensor_id"] = String("CAR_SENSOR_") + (i + 1);
        doc["distance"] = avgDistance;
        doc["is_occupied"] = isOccupied;
        
        // Send to server
        String jsonString;
        serializeJson(doc, jsonString);
        Serial.print("  Sending data: ");
        Serial.println(jsonString);
        sendSensorData(jsonString.c_str());
        
        delay(100);
      } else {
        Serial.println("  No valid readings!");
      }
    }
    
    // Process motorcycle sensors
    for (int i = 0; i < 3; i++) {
      if (!MOTORCYCLE_SENSOR_CONNECTED[i]) {
        Serial.print("Motorcycle sensor ");
        Serial.print(4 + i);
        Serial.println(" not connected, skipping");
        continue;
      }
      
      Serial.print("Reading motorcycle sensor ");
      Serial.println(4 + i);
      
      float totalDistance = 0;
      int validReadings = 0;
      
      // Take multiple readings
      for (int j = 0; j < READINGS_COUNT; j++) {
        float distance = getDistance(MOTORCYCLE_SENSORS[i][0], MOTORCYCLE_SENSORS[i][1]);
        Serial.print("  Reading ");
        Serial.print(j + 1);
        Serial.print(": ");
        Serial.println(distance);
        
        if (distance > 0 && distance < 400) {
          totalDistance += distance;
          validReadings++;
        }
        delay(10);
      }
      
      // Only send data if we got valid readings
      if (validReadings > 0) {
        float avgDistance = totalDistance / validReadings;
        Serial.print("  Average distance: ");
        Serial.println(avgDistance);
        
        // Set LED color based on object detection
        bool isOccupied = avgDistance < MOTORCYCLE_THRESHOLD;
        setLEDColor(2, isOccupied);  // Use car slot 2's LED for motorcycle detection
        
        // Create JSON for this sensor
        JsonDocument doc;
        doc["sensor_id"] = String("MOTO_SENSOR_") + (4 + i);
        doc["distance"] = avgDistance;
        
        // Add hysteresis to prevent flickering
        bool currentlyOccupied = avgDistance < MOTORCYCLE_THRESHOLD;
        bool currentlyVacant = avgDistance > (MOTORCYCLE_THRESHOLD + VACANCY_MARGIN);
        doc["is_occupied"] = currentlyOccupied;
        
        // Send to server
        String jsonString;
        serializeJson(doc, jsonString);
        Serial.print("  Sending data: ");
        Serial.println(jsonString);
        sendSensorData(jsonString.c_str());
        
        delay(100);
      } else {
        Serial.println("  No valid readings!");
      }
    }
    
    lastBulkUpdateTime = currentTime;
    Serial.println("Finished sensor readings");
  }
  
  // Optional: Periodically check IP (uncomment if needed)
  // static unsigned long lastIPCheckTime = 0;
  // if (millis() - lastIPCheckTime > 60000) {  // Check every minute
  //   getESP8266IPAddress();
  //   lastIPCheckTime = millis();
  // }
}