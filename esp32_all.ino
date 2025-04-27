#include <Wire.h>
#include <MPU6050.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

// WiFi
const char* ssid = "iphone13";
const char* password = "nikhils23j";

// Telegram
String botToken = "user_token";
String chatID = "1519496749";

// Buzzer
#define BUZZER_PIN 4

// Accelerometer
MPU6050 accel;
#define SAMPLE_COUNT 200
#define CRASH_THRESHOLD 1.0
float offset_ax = 0, offset_ay = 0, offset_az = 0;

// GPS
#define GPS_RX 16
#define GPS_TX 17
TinyGPSPlus gps;
HardwareSerial gpsSerial(2);

// --- SETUP ---
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Buzzer and I2C Init
  pinMode(BUZZER_PIN, OUTPUT);
  Wire.begin(21, 22);  // SDA, SCL
  
  // WiFi Connect
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    Serial.print(".");
    retries++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected.");
    sendTelegramMessage("📡 ESP32 Connected to WiFi.");
  } else {
    Serial.println("\nFailed to connect to WiFi.");
  }

  // Accelerometer Init & Calibration
  accel.initialize();
  Serial.println("Calibrating accelerometer... Please keep still.");

  long sum_ax = 0, sum_ay = 0, sum_az = 0;
  int16_t ax, ay, az;
  for (int i = 0; i < SAMPLE_COUNT; i++) {
    accel.getAcceleration(&ax, &ay, &az);
    sum_ax += ax;
    sum_ay += ay;
    sum_az += az;
    delay(50);
  }
  offset_ax = sum_ax / (float)SAMPLE_COUNT;
  offset_ay = sum_ay / (float)SAMPLE_COUNT;
  offset_az = sum_az / (float)SAMPLE_COUNT;
  Serial.println("Accelerometer calibrated.");

  // GPS Setup
  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
  Serial.println("Waiting for GPS fix...");
  unsigned long startTime = millis();
  bool fixAcquired = false;
  while (!fixAcquired && millis() - startTime < 60000) {
    while (gpsSerial.available()) gps.encode(gpsSerial.read());
    if (gps.location.isValid()) {
      fixAcquired = true;
    } else {
      Serial.print(".");
      delay(500);
    }
  }
  if (fixAcquired) {
    Serial.println("\nGPS fix acquired!");
  } else {
    Serial.println("\nGPS fix NOT acquired.");
  }
}

// --- LOOP ---
void loop() {
  // Read Accel
  int16_t ax_raw, ay_raw, az_raw;
  accel.getAcceleration(&ax_raw, &ay_raw, &az_raw);

  float ax = (ax_raw - offset_ax) / 16384.0;
  float ay = (ay_raw - offset_ay) / 16384.0;
  float az = (az_raw - offset_az) / 16384.0;

  float g_total = sqrt(ax * ax + ay * ay + az * az);
  Serial.print("g_total: "); Serial.println(g_total);

  // Feed GPS
  while (gpsSerial.available()) gps.encode(gpsSerial.read());

  // Detect Crash
  if (g_total > CRASH_THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);

    String message;
    if (gps.location.isUpdated()) {
      String lat = String(gps.location.lat(), 6);
      String lng = String(gps.location.lng(), 6);
      message = "🚨 Crash Detected! \n📍 Location: https://maps.google.com/?q=" + lat + "," + lng;
    } else {
      message = "🚨 Crash Detected! \n📍 Location not available.";
    }

    sendTelegramMessage(message);
    delay(3000); // Avoid spamming
  }

  delay(100);  // 10 Hz
}

// --- TELEGRAM ---
void sendTelegramMessage(String message) {
  String safeMessage = urlencode(message);
  String url = "https://api.telegram.org/bot" + botToken +
               "/sendMessage?chat_id=" + chatID + "&text=" + safeMessage;

  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.println("Message sent! HTTP code: " + String(httpCode));
  } else {
    Serial.println("Failed to send message. HTTP error: " + String(httpCode));
  }
  http.end();
}

String urlencode(String str) {
  String encoded = "";
  char c;
  char code0, code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (isalnum(c)) {
      encoded += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) code1 = (c & 0xf) - 10 + 'A';
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) code0 = c - 10 + 'A';
      encoded += '%';
      encoded += code0;
      encoded += code1;
    }
  }
  return encoded;
}
