#include <Wire.h>
#include <WiFi.h>
#include <unordered_map>
#include <ESPmDNS.h>

#include "lib/http.h"
#include "lib/websocket.h"
#include "lib/imu.h"
#include "lib/heart.h"
#include "lib/jump.h"
#include "lib/pushup.h"
#include "lib/punch.h"

// Constants
#define WIFI_SSID "test"
#define WIFI_PASSWORD "12345678"
#define ESP_HOSTNAME "sicermat"

float jumpHeight = 0;

float heartRate = 0;
float spo2 = 0;

uint32_t LoopTimer;
uint32_t lastWebSocketReconnectAttempt = 0;

int repeatCount = 0;

void terminateWithError(const String& errorMessage) {

  // Print error message
  Serial.println("\n!!! SENSOR ERROR !!!");
  Serial.println(errorMessage);
  Serial.println("Program terminated due to sensor malfunction.");

  // Optional: Perform any cleanup or safety actions
  WiFi.disconnect(true);
  for(;;){
      delay(10000);
  };
}

void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to WiFi
  connectToWiFi();

  // Inisialisasi mDNS
  if (!MDNS.begin(ESP_HOSTNAME)) {
      Serial.println("Error setting up mDNS!");
  } else {
      Serial.printf("mDNS responder started: http://%s.local\n", ESP_HOSTNAME);
  }
  
  registerRoutes(server);

  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);
  server.begin();

  // Initialize Sensors
  sensorInit(mpu);

  // Calibrate Sensors
  calibrateSensors(mpu);

  // xTaskCreatePinnedToCore(sendDataTask, "SendDataTask", 4096, NULL, 1, NULL, 0);
  // xTaskCreatePinnedToCore(resetQueueTask, "ResetQueueTask", 2048, NULL, 1, NULL, 1);
  // xTaskCreatePinnedToCore(readSensorTask, "ReadSensorTask", 4096, NULL, 1, NULL, 0);

  LoopTimer = micros();
}

void loop() {
  // Check WiFi connection (Core 1)
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }

  // Read sensor data (Core 2)
  sensorSignals(mpu, accX, accY, accZ, gyroX, gyroY, gyroZ, true);

  // Create current reading (Core 2)
  SensorReading currentReading = {accX, accY, accZ, gyroX, gyroY, gyroZ};

  // Check sensor health (Core 2)
  if (!isSensorHealthy(currentReading, previousReading)) {
    repeatCount++;
  } else {
    repeatCount = 0;
  }

  //Core 2
  if (repeatCount >= MAX_REPEATED_READINGS) {
    terminateWithError("Data sensornya ngestuck, coba cek wiring sama wiringnya dulu!");
  }

  //Core 2
  previousReading = currentReading;

  // Get sport analysis(Core 2)
  PunchMetrics punchMetrics = analyzePunch(accX, accY, accZ);
  if (punchMetrics.valid){
    String punchData = getPunchMetrics(punchMetrics.punchPower, punchMetrics.retractionTime);
    for (auto& entry : clients) {
      ClientData& clientData = entry.second;
      sendJsonToClient(clientData, entry.first, punchData, "punch");
    }
  }

  // Maintain loop timing
  while (millis() - LoopTimer < 100);
  LoopTimer = millis();
}