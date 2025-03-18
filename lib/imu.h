#ifndef IMU_H
#define IMU_H

#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <cmath>

#define SENSOR_HEALTH_TOLERANCE 0.01
#define MAX_REPEATED_READINGS 10

struct SensorReading {
    float accX, accY, accZ;
    float gyroX, gyroY, gyroZ;
};

Adafruit_MPU6050 mpu;
SensorReading previousReading = {0};

float accX, accY, accZ;
float gyroX, gyroY, gyroZ;
float roll_rate_calibration, pitch_rate_calibration, yaw_rate_calibration;
float accX_calibration, accY_calibration, accZ_calibration;

// Function to check sensor health
bool isSensorHealthy(const SensorReading &current, const SensorReading &previous) {
    return !(
        (abs(current.accX - previous.accX) < SENSOR_HEALTH_TOLERANCE) &&
        (abs(current.accY - previous.accY) < SENSOR_HEALTH_TOLERANCE) &&
        (abs(current.accZ - previous.accZ) < SENSOR_HEALTH_TOLERANCE) &&
        (abs(current.gyroX - previous.gyroX) < SENSOR_HEALTH_TOLERANCE) &&
        (abs(current.gyroY - previous.gyroY) < SENSOR_HEALTH_TOLERANCE) &&
        (abs(current.gyroZ - previous.gyroZ) < SENSOR_HEALTH_TOLERANCE));
}

void sensorSignals(Adafruit_MPU6050 &mpu, float &accX, float &accY, float &accZ, float &gyroX, float &gyroY, float &gyroZ, bool calibrate) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
  
    // Apply accelerometer calibration offsets
    accX = a.acceleration.x;
    accY = a.acceleration.y;
    accZ = a.acceleration.z;
  
    // Apply gyroscope calibration offsets
    gyroX = (g.gyro.x * (180.0 / PI));
    gyroY = (g.gyro.y * (180.0 / PI));
    gyroZ = (g.gyro.z * (180.0 / PI));
  
    if (calibrate) {
      gyroX -= roll_rate_calibration;
      gyroY -= pitch_rate_calibration;
      gyroZ -= yaw_rate_calibration;
    }
}

void sensorInit(Adafruit_MPU6050 &mpu) {
    if (!mpu.begin()) {
      Serial.println("MPU6050 not detected. Check wiring or address!");
      while (1);
    }
    Serial.println("MPU6050 initialized.");
}

void calibrateIMU(Adafruit_MPU6050 &mpu) {
    float accX_data = 0, accY_data = 0, accZ_data = 0, gyroX_data = 0, gyroY_data = 0, gyroZ_data = 0;
    const int n = 2000;
  
    for (int i = 0; i < n; i++) {
      sensorSignals(mpu, accX, accY, accZ, gyroX, gyroY, gyroZ, false);
  
      accX_data += accX;
      accY_data += accY;
      accZ_data += accZ;
  
      gyroX_data += gyroX;
      gyroY_data += gyroY;
      gyroZ_data += gyroZ;
  
      delay(1);
    }
  
    accX_calibration = accX_data / n;
    accY_calibration = accY_data / n;
    accZ_calibration = accZ_data / n;
  
    roll_rate_calibration = gyroX_data / n;
    pitch_rate_calibration = gyroY_data / n;
    yaw_rate_calibration = gyroZ_data / n;
  
    Serial.println("Kalibrasi aman");
}

// void readSensorTask(void *parameter) {
//   while (true) {
//       punchPower = random(10, 100);
//       retractionTime = random(200, 1000);

//       jumpHeight = random(500, 1500);
//       vTaskDelay(pdMS_TO_TICKS(50)); 
//   }
// }

#endif