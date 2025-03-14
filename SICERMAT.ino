#include <Wire.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "HTML/jump_html.h"
#include "HTML/pushup_html.h"
#include "HTML/punch_html.h"
#include "JS/jump_js.h"
#include "JS/pushup_js.h"
#include "JS/punch_js.h"
#include "icon/jump_svg.h"
#include "icon/pushup_svg.h"
#include "icon/punch_svg.h"
#include "icon/hr_svg.h"
#include "icon/spo_svg.h"
#include "index_html.h"
#include "style_css.h"
#include "script_js.h"
// #include "three_js.h"

// Constants
#define PUNCH_THRESHOLD 15.0
#define RETRACTION_THRESHOLD 8.0
#define MIN_PUNCH_INTERVAL 250
#define PUNCH_TIMEOUT 500
#define WIFI_SSID "test"
#define WIFI_PASSWORD "12345678"
#define MAX_REPEATED_READINGS 10
#define SENSOR_HEALTH_TOLERANCE 0.01

// Global Variables
Adafruit_MPU6050 mpu;
AsyncWebServer server(80);
AsyncEventSource events("/events");

int punchCount = 0;
float maxPunchPower = 0;
float avgPunchPower = 0;
float totalPunchPower = 0;
float lastRetractionTime = 0;
float avgRetractionTime = 0;
float totalRetractionTime = 0;

float accX, accY, accZ;
float gyroX, gyroY, gyroZ;
float roll_rate_calibration, pitch_rate_calibration, yaw_rate_calibration;
float accX_calibration, accY_calibration, accZ_calibration;

uint32_t LoopTimer;
uint32_t lastWebSocketReconnectAttempt = 0;

// Definisi struktur untuk menyimpan informasi file
struct Route {
  const char *path;
  const char *mimeType;
  const char *content;
};

struct SensorReading {
  float accX, accY, accZ;
  float gyroX, gyroY, gyroZ;
};

struct PunchState {
  bool inPunch = false;
  bool waitingForRetraction = false;
  unsigned long punchStartTime = 0;
  unsigned long lastPunchTime = 0;
  float maxPunchAcc = 0;
} punchState;

// Daftar file yang disajikan oleh server
Route routes[] = {
  {"/", "text/html", index_html},
  {"/style.css", "text/css", style_css},
  {"/script.js", "application/javascript", script_js},

  // JavaScript Files
  {"/JS/jump.js", "application/javascript", jump_js},
  {"/JS/punch.js", "application/javascript", punch_js},
  {"/JS/pushup.js", "application/javascript", pushup_js},

  // HTML Files
  {"/HTML/jump.html", "text/html", jump_html},
  {"/HTML/punch.html", "text/html", punch_html},
  {"/HTML/pushup.html", "text/html", pushup_html},

  // Icon Files
  {"/icon/hr.svg", "image/svg+xml", hr_svg},
  {"/icon/spo.svg", "image/svg+xml", spo_svg},
  {"/icon/punch.svg", "image/svg+xml", punch_svg},
  {"/icon/pushup.svg", "image/svg+xml", pushup_svg},
  {"/icon/jump.svg", "image/svg+xml", jump_svg},
};

SensorReading previousReading = {0};
int repeatCount = 0;

// Fungsi untuk mendaftarkan semua route
void registerRoutes(AsyncWebServer &server) {
  for (Route &route : routes) {
      server.on(route.path, HTTP_GET, [route](AsyncWebServerRequest *request) {
          request->send_P(200, route.mimeType, route.content);
      });
  }
}

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

void sensorSignals(float &accX, float &accY, float &accZ, float &gyroX, float &gyroY, float &gyroZ, bool calibrate) {
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

void calibrateSensors() {
  float accX_data = 0, accY_data = 0, accZ_data = 0, gyroX_data = 0, gyroY_data = 0, gyroZ_data = 0;
  const int n = 2000;

  for (int i = 0; i < n; i++) {
    sensorSignals(accX, accY, accZ, gyroX, gyroY, gyroZ, false);

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

void sensorInit() {
  if (!mpu.begin()) {
    Serial.println("MPU6050 not detected. Check wiring or address!");
    while (1);
  }
  Serial.println("MPU6050 initialized.");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Wire.begin();
  Wire.setClock(400000);

  // Initialize Networking
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected to " + String(WIFI_SSID) + " with IP " + WiFi.localIP().toString());

  registerRoutes(server);

  server.addHandler(&events);
  server.begin();

  // Initialize Sensors
  sensorInit();
  calibrateSensors();

  LoopTimer = micros();
}

void sendSensorData() {
  StaticJsonDocument<200> acc;
  acc["type"] = "accelerometer";
  acc["accX"] = accX;
  acc["accY"] = accY;
  acc["accZ"] = accZ;

  String accJson;
  serializeJson(acc, accJson);
  events.send(accJson.c_str(), "sensorData", millis());

  StaticJsonDocument<200> gyro;
  gyro["type"] = "gyroscope";
  gyro["gyroX"] = gyroX * M_PI / 180.0;
  gyro["gyroY"] = gyroY * M_PI / 180.0;
  gyro["gyroZ"] = gyroZ * M_PI / 180.0;

  String gyroJson;
  serializeJson(gyro, gyroJson);
  events.send(gyroJson.c_str(), "sensorData", millis());
}

void loop() {
  // Check WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Reconnecting...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(5000);
    return;
  }

  // Read sensor data
  sensorSignals(accX, accY, accZ, gyroX, gyroY, gyroZ, true);

  // Create current reading
  SensorReading currentReading = {accX, accY, accZ, gyroX, gyroY, gyroZ};

  // Check sensor health
  if (!isSensorHealthy(currentReading, previousReading)) {
    repeatCount++;
  } else {
    repeatCount = 0;
  }

  if (repeatCount >= MAX_REPEATED_READINGS) {
    Serial.println("Sensor data appears to be stuck. Check wiring and connections.");
    return;
  }

  previousReading = currentReading;

  // Send sensor data via WebSocket
  sendSensorData();

  // Maintain loop timing
  while (millis() - LoopTimer < 100);
  LoopTimer = millis();
}