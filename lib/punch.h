#ifndef PUNCH_H
#define PUNCH_H

#include <Arduino.h>
#include <ArduinoJson.h>

#define PUNCH_THRESHOLD 15.0
#define RETRACTION_THRESHOLD 8.0
#define MIN_PUNCH_INTERVAL 250
#define PUNCH_TIMEOUT 500

float punchPower = 0;
float retractionTime = 0;

struct PunchState {
    bool inPunch = false;
    bool waitingForRetraction = false;
    unsigned long punchStartTime = 0;
    unsigned long lastPunchTime = 0;
    float maxPunchAcc = 0;
} punchState;

// Definisikan struct untuk menampung metrics
struct PunchMetrics {
  bool valid;             // true jika siklus punch selesai
  float punchPower;
  float retractionTime;
};

int punchCount = 0;
// float maxPunchPower = 0;
// float avgPunchPower = 0;
// float totalPunchPower = 0;
// float lastRetractionTime = 0;
// float avgRetractionTime = 0;
// float totalRetractionTime = 0;

// Ubah fungsi analyzePunch menjadi fungsi yang mengembalikan PunchMetrics
PunchMetrics analyzePunch(float accX, float accY, float accZ) {
  PunchMetrics result = { false, 0, 0 };
  
  // Calculate total acceleration magnitude
  float accMagnitude = sqrt(accX * accX + accY * accY + accZ * accZ);
  unsigned long currentTime = millis();
  
  // State machine for punch detection
  if (!punchState.inPunch && !punchState.waitingForRetraction) {
      // Looking for punch initiation
      if (accMagnitude > PUNCH_THRESHOLD && 
          (currentTime - punchState.lastPunchTime) > MIN_PUNCH_INTERVAL) {
          punchState.inPunch = true;
          punchState.punchStartTime = currentTime;
          punchState.maxPunchAcc = accMagnitude;
      }
  }
  else if (punchState.inPunch) {
      // During punch, track maximum acceleration
      if (accMagnitude > punchState.maxPunchAcc) {
          punchState.maxPunchAcc = accMagnitude;
      }
      
      // Check if punch phase is complete
      if (accMagnitude < PUNCH_THRESHOLD) {
          punchState.inPunch = false;
          punchState.waitingForRetraction = true;
      }
      
      // Timeout check
      if (currentTime - punchState.punchStartTime > PUNCH_TIMEOUT) {
          punchState.inPunch = false;
          punchState.waitingForRetraction = false;
      }
  }
  else if (punchState.waitingForRetraction) {
      // Detect retraction (negative acceleration)
      if (accMagnitude > RETRACTION_THRESHOLD) {
          // Complete punch cycle
          punchCount++;
          
          // Calculate punch power (normalized based on max acceleration)
          float punchPower = (punchState.maxPunchAcc - PUNCH_THRESHOLD) / (50.0 - PUNCH_THRESHOLD) * 100;
          punchPower = constrain(punchPower, 0, 100);
          
          // Update power statistics
        //   totalPunchPower += punchPower;
        //   avgPunchPower = totalPunchPower / punchCount;
        //   maxPunchPower = max(maxPunchPower, punchPower);
          
          // Calculate retraction time
          float retractionTime = (currentTime - punchState.punchStartTime);
        //   totalRetractionTime += retractionTime;
        //   avgRetractionTime = totalRetractionTime / punchCount;
          
          // Update last punch time
          punchState.lastPunchTime = currentTime;
          
          // Siapkan nilai untuk return
          result.valid = true;
          result.punchPower = punchPower;
          result.retractionTime = retractionTime;
          
          // Reset state
          punchState.waitingForRetraction = false;
          punchState.maxPunchAcc = 0;
      }
      
      // Timeout check
      if (currentTime - punchState.punchStartTime > PUNCH_TIMEOUT) {
          punchState.waitingForRetraction = false;
      }
  }
  
  return result;
}

String getPunchMetrics(float punchPower, float retractionTime) {
  JsonDocument metrics;
  metrics["type"] = "punch";
  metrics["punchPower"] = punchPower;
  metrics["retractionTime"] = retractionTime;
  
  String metricsJson;
  serializeJson(metrics, metricsJson);
  return metricsJson;
}
  
#endif