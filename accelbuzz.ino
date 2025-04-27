#include <Wire.h>
#include <MPU6050.h>
#include <math.h>

// Pin for the buzzer
#define BUZZER_PIN 4

// Sampling settings
#define SAMPLE_COUNT 200
#define CRASH_THRESHOLD 4  // in g

MPU6050 accel;  // Create an MPU6050 object

float offset_ax = 0, offset_ay = 0, offset_az = 0;

void setup() {
  Serial.begin(115200);   // Use 115200 baud rate for ESP32 serial communication
  Wire.begin(21, 22);     // I2C SDA on GPIO21 and SCL on GPIO22
  pinMode(BUZZER_PIN, OUTPUT);
  
  accel.initialize();     // Initialize the accelerometer

  // Calibration: Take sample readings to calculate offsets
  Serial.println("Calibrating accelerometer... Please keep the device at rest.");
  
  long sum_ax = 0, sum_ay = 0, sum_az = 0;
  int16_t ax, ay, az;

  for (int i = 0; i < SAMPLE_COUNT; i++) {
    accel.getAcceleration(&ax, &ay, &az);  // Get raw accelerometer readings
    sum_ax += ax;
    sum_ay += ay;
    sum_az += az;
    delay(100);  // Delay for 100ms to collect 10 samples/sec
  }

  // Compute the average offset
  offset_ax = sum_ax / (float)SAMPLE_COUNT;
  offset_ay = sum_ay / (float)SAMPLE_COUNT;
  offset_az = sum_az / (float)SAMPLE_COUNT;

  Serial.println("Calibration complete.");
  Serial.print("Offset ax: "); Serial.println(offset_ax);
  Serial.print("Offset ay: "); Serial.println(offset_ay);
  Serial.print("Offset az: "); Serial.println(offset_az);
}

void loop() {
  int16_t ax_raw, ay_raw, az_raw;
  accel.getAcceleration(&ax_raw, &ay_raw, &az_raw);  // Get raw accelerometer data

  // Convert raw values to 'g' (1g = 9.8m/s^2)
  float ax = (ax_raw - offset_ax) / 16384.0;
  float ay = (ay_raw - offset_ay) / 16384.0;
  float az = (az_raw - offset_az) / 16384.0;

  // Calculate the total acceleration in 'g' (combined x, y, z axes)
  float g_total = sqrt(ax * ax + ay * ay + az * az);

  // Print the total acceleration
  Serial.print("g_total: "); Serial.println(g_total);

  // If the total acceleration exceeds the crash threshold, trigger the buzzer
  if (g_total > CRASH_THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH);  // Turn buzzer ON
    delay(1000);                    // Keep buzzer ON for 10 seconds
    digitalWrite(BUZZER_PIN, LOW);   // Turn buzzer OFF
  }

  delay(100);  // Sample every 100ms (10 Hz)
}
