#include <Servo.h>
#include <SoftwareSerial.h>

// ✅ Pin Definitions
#define RFID_RX_PIN 2
#define RFID_TX_PIN 3
#define IR_SENSOR_PIN A1
#define SERVO_PIN 9

// ✅ Constants
#define SERVO_OPEN_ANGLE 90
#define SERVO_CLOSE_ANGLE 0
#define FEEDER_OPEN_TIME 4000         // ⏳ 4-second delay after cat leaves
#define SERVO_MOVE_DELAY 30           // 🐢 Base per-step delay

// ✅ Authorized Tags: Tiny, Panther, Testing Tag
const String authorizedTags[] = {
  "796C27A1F2843011",  // Tiny
  "4B6C27A1F2843011",  // Panther
  "676C27A1F2843011"   // Testing Tag
};
const String tagNames[] = {
  "Tiny",
  "Panther",
  "Testing Tag"
};
const int numAuthorizedTags = sizeof(authorizedTags) / sizeof(authorizedTags[0]);

// ✅ Global Variables
Servo feederServo;
SoftwareSerial rfidSerial(RFID_RX_PIN, RFID_TX_PIN);
bool feederOpen = false;
bool tagScanned = false;
bool servoMoving = false;
unsigned long lastScanTime = 0;
const unsigned long scanCooldown = 3000;
unsigned long rfidIgnoreUntil = 0;

void setup() {
  Serial.begin(115200);
  rfidSerial.begin(9600);
  feederServo.attach(SERVO_PIN);
  feederServo.write(SERVO_CLOSE_ANGLE);
  delay(1000);
  feederServo.detach();

  Serial.println("🐱 Tiny & Panther's Feeder Ready (Improved IR & 4s Timeout)");
}

void loop() {
  unsigned long currentTime = millis();

  // ✅ Read RFID tag (only if safe)
  if (!servoMoving && millis() > rfidIgnoreUntil) {
    readRFIDTag();
  }

  // ✅ Reset duplicate scan flag
  if (tagScanned && currentTime - lastScanTime > scanCooldown) {
    tagScanned = false;
  }

  // ✅ Keep feeder open while cat is present
  if (feederOpen) {
    if (isCatPresent()) {
      Serial.println("🐾 Cat detected. Keeping feeder open...");
      lastScanTime = currentTime;
    } else if (millis() - lastScanTime > FEEDER_OPEN_TIME && !servoMoving) {
      Serial.println("⏳ Cat gone. Closing feeder gently...");
      smoothServoMove(SERVO_OPEN_ANGLE, SERVO_CLOSE_ANGLE, SERVO_MOVE_DELAY * 3);
      feederOpen = false;
    }
  }
}

// ✅ Handle RFID tag reading
void readRFIDTag() {
  if (!rfidSerial.available()) return;

  String rawData = "";
  while (rfidSerial.available()) {
    char c = rfidSerial.read();
    rawData += c;
    delay(5);
  }

  Serial.print("🔍 Raw RFID Data: ");
  Serial.println(rawData);

  if (rawData.length() >= 16) {
    String tagID = rawData.substring(1, 17);

    Serial.print("✅ RFID Tag Detected: ");
    Serial.println(tagID);

    int tagIndex = getAuthorizedIndex(tagID);

    if (tagIndex >= 0 && !feederOpen && !tagScanned) {
      Serial.print("🎉 Authorized Tag: ");
      Serial.println(tagNames[tagIndex]);
      delay(200);
      smoothServoMove(SERVO_CLOSE_ANGLE, SERVO_OPEN_ANGLE, SERVO_MOVE_DELAY);
      feederOpen = true;
      lastScanTime = millis();
      tagScanned = true;
    } else {
      Serial.println("❌ Unauthorized or duplicate scan.");
    }
  }
}

// ✅ Return index if tag is authorized, -1 if not
int getAuthorizedIndex(String scannedTag) {
  for (int i = 0; i < numAuthorizedTags; i++) {
    if (scannedTag == authorizedTags[i]) {
      return i;
    }
  }
  return -1;
}

// ✅ IR Sensor Distance Detection (improved logic)
bool isCatPresent() {
  int detectionCount = 0;
  const int sampleCount = 15;
  const int detectionThreshold = 250;

  for (int i = 0; i < sampleCount; i++) {
    int reading = analogRead(IR_SENSOR_PIN);
    Serial.print("📏 Sharp Sensor Reading: ");
    Serial.println(reading);

    if (reading > detectionThreshold) {
      detectionCount++;
    }
    delay(60);
  }

  return detectionCount >= 8;  // More stable presence check
}

// ✅ Smooth Servo Movement — now with full RFID shutdown
void smoothServoMove(int fromAngle, int toAngle, int stepDelay) {
  feederServo.attach(SERVO_PIN);
  delay(50);
  servoMoving = true;

  // 🚫 Disable RFID serial to eliminate interference
  rfidSerial.end();

  int stepSize = 1;
  if (fromAngle < toAngle) {
    for (int pos = fromAngle; pos <= toAngle; pos += stepSize) {
      feederServo.write(pos);
      delay(stepDelay);
    }
  } else {
    for (int pos = fromAngle; pos >= toAngle; pos -= stepSize) {
      feederServo.write(pos);
      delay(stepDelay);
    }
  }

  delay(100);
  feederServo.detach();
  servoMoving = false;

  // ✅ Re-enable RFID reader AFTER servo finishes moving
  rfidSerial.begin(9600);
  while (rfidSerial.available()) rfidSerial.read();
  rfidIgnoreUntil = millis() + 500;
}
