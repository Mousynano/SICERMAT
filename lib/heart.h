#ifndef HEART_H
#define HEART_H

#include <Arduino.h>
#include "MAX30105.h"
#include "heartRate.h"

MAX30105 particleSensor;

void calibrateHeartSensor(MAX30105 &particleSensor)
{
  if (particleSensor.begin(Wire, I2C_SPEED_FAST) == false)
  {
    Serial.println("MAX30105 not detected. Check wiring or address!");
    while (1)
      ;
  }
  Serial.println("Place your index finger on the sensor with steady pressure for calibration.");
  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeGreen(0);
}

String getHeartMetrics(float heartRate, float spo2)
{
  JsonDocument metrics;
  metrics["type"] = "heart";
  metrics["heartRate"] = heartRate;
  metrics["spo2"] = spo2;
  String metricsJson;
  serializeJson(metrics, metricsJson);
  return metricsJson;
}
void heartCalculation(MAX30105 &particleSensor, float &heartRate, float &spo2)
{
  int HR_RES;
  float SPO_RES;
  float beatsPerMinute;
  long irValue = particleSensor.getIR();
  long redValue = particleSensor.getRed();

  float R = (float)redValue / (float)irValue;
  float SPO_RES = 104.0 - 17.0 * R; // SPO

  if (checkForBeat(irValue) == true)
  {
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute;
      rateSpot %= RATE_SIZE;

      HR_RES = 0;
      for (byte x = 0; x < RATE_SIZE; x++)
        HR_RES += rates[x];
      HR_RES /= RATE_SIZE;
    }
  }

  if (HR_RES > 60)
  {
    heartRate = HR_RES;
    spo2 = SPO_RES;
  }
  // fill in how to calculate heart measures here
  // CAP_FUZZY should be a good reference
  // should end like below
  //  heartRate = HR_RES;
  //  spo2 = SPO_RES;
}

// Fuzzy Sugeno function to determine status based on SpO2 and Heart Rate
String fuzzySugeno(float spo2, int hr)
{
  String spo2Category, hrCategory;

  // Determine SpO2 category
  if (spo2 >= 95)
    spo2Category = "Normal";
  else if (spo2 >= 91)
    spo2Category = "Mild";
  else if (spo2 >= 86)
    spo2Category = "Moderate";
  else
    spo2Category = "Severe";

  // Determine Heart Rate category
  if (hr < 60)
    hrCategory = "Brad";
  else if (hr <= 90)
    hrCategory = "Normal";
  else
    hrCategory = "Tach";

  // Apply fuzzy rules
  if (spo2Category == "Normal")
  {
    if (hrCategory == "Brad")
      return "NORMAL";
    else if (hrCategory == "Normal")
      return "NORMAL";
    else if (hrCategory == "Tach")
      return "LELAH";
  }
  else if (spo2Category == "Mild")
  {
    if (hrCategory == "Brad")
      return "LELAH";
    else if (hrCategory == "Normal")
      return "LELAH";
    else if (hrCategory == "Tach")
      return "OVERWORK";
  }
  else if (spo2Category == "Moderate")
  {
    if (hrCategory == "Brad")
      return "OVERWORK";
    else if (hrCategory == "Normal")
      return "OVERWORK";
    else if (hrCategory == "Tach")
      return "STOP LATIHAN";
  }
  else if (spo2Category == "Severe")
  {
    return "STOP LATIHAN"; // All cases for Severe SpO2
  }

  return "UNKNOWN"; // Default case
}

#endif