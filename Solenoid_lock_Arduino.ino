#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial mySerial(2, 3);  // RX, TX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust I2C address if needed

#define RELAY_PIN 4
#define BUZZER_PIN 5
#define LED_PIN 6

enum SystemMode { NORMAL_MODE, ENROLL_MODE, DELETE_MODE };
SystemMode currentMode = NORMAL_MODE;
int targetID = 0;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  
  // Initialize fingerprint sensor
  finger.begin(57600);
  if (finger.verifyPassword()) {
    lcdDisplay("System Ready", "Scan Finger");
  } else {
    lcdDisplay("Sensor Error!", "Check Wiring");
    while(1);
  }

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  handleSerialCommands();
  
  switch(currentMode) {
    case NORMAL_MODE:
      normalModeOperation();
      break;
    case ENROLL_MODE:
      enrollModeOperation();
      break;
    case DELETE_MODE:
      deleteModeOperation();
      break;
  }
}

void handleSerialCommands() {
  if (Serial.available()) {
    char command = Serial.read();
    
    switch(command) {
      case 'e': // Start enrollment
        currentMode = ENROLL_MODE;
        Serial.println("Enter ID (1-127):");
        while(!Serial.available());
        targetID = Serial.parseInt();
        if(targetID < 1 || targetID > 127) {
          Serial.println("Invalid ID!");
          currentMode = NORMAL_MODE;
        }
        break;
        
      case 'd': // Start deletion
        currentMode = DELETE_MODE;
        Serial.println("Enter ID to delete:");
        while(!Serial.available());
        targetID = Serial.parseInt();
        if(targetID < 1 || targetID > 127) {
          Serial.println("Invalid ID!");
          currentMode = NORMAL_MODE;
        }
        break;
        
      case 'r': // Reset to normal
        currentMode = NORMAL_MODE;
        lcdDisplay("System Ready", "Scan Finger");
        break;
    }
  }
}

void normalModeOperation() {
  int fingerID = getFingerprintID();
  
  if(fingerID >= 0) {
    accessGranted(fingerID);
  }
  else if(fingerID == -3) {
    accessDenied();
  }
}

void enrollModeOperation() {
  lcdDisplay("Enrolling ID:", String(targetID));
  Serial.print("Enrolling ID: ");
  Serial.println(targetID);
  
  if(enrollFinger(targetID)) {
    lcdDisplay("Enrollment Success", "ID: " + String(targetID));
    successFeedback();
  } else {
    lcdDisplay("Enrollment Failed", "Try Again");
    errorFeedback();
  }
  currentMode = NORMAL_MODE;
  lcdDisplay("System Ready", "Scan Finger");
}

void deleteModeOperation() {
  lcdDisplay("Deleting ID:", String(targetID));
  Serial.print("Deleting ID: ");
  Serial.println(targetID);
  
  if(deleteFinger(targetID)) {
    lcdDisplay("Deletion Success", "ID: " + String(targetID));
    successFeedback();
  } else {
    lcdDisplay("Deletion Failed", "Invalid ID");
    errorFeedback();
  }
  currentMode = NORMAL_MODE;
  lcdDisplay("System Ready", "Scan Finger");
}

int getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -2;

  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_NOTFOUND) return -3;
  if (p != FINGERPRINT_OK) return -4;

  return finger.fingerID;
}

bool enrollFinger(int id) {
  // First scan
  lcdDisplay("Place Finger", "Step 1/2");
  while(finger.getImage() != FINGERPRINT_OK);
  if(finger.image2Tz(1) != FINGERPRINT_OK) return false;

  lcdDisplay("Remove Finger", "");
  delay(2000);

  // Second scan
  lcdDisplay("Place Finger", "Step 2/2");
  while(finger.getImage() != FINGERPRINT_OK);
  if(finger.image2Tz(2) != FINGERPRINT_OK) return false;

  // Create model
  if(finger.createModel() != FINGERPRINT_OK) return false;
  
  // Store model
  return finger.storeModel(id) == FINGERPRINT_OK;
}

bool deleteFinger(int id) {
  return finger.deleteModel(id) == FINGERPRINT_OK;
}

void accessGranted(int id) {
  digitalWrite(RELAY_PIN, LOW);
  lcdDisplay("Access Granted", "ID: " + String(id));
  successFeedback();
  delay(5000);
  digitalWrite(RELAY_PIN, HIGH);
  lcdDisplay("System Ready", "Scan Finger");
}

void accessDenied() {
  lcdDisplay("Access Denied", "Invalid Finger");
  errorFeedback();
  delay(3000);
  lcdDisplay("System Ready", "Scan Finger");
}

void lcdDisplay(String line1, String line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

void successFeedback() {
  tone(BUZZER_PIN, 1500, 500);
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
}

void errorFeedback() {
  for(int i=0; i<3; i++) {
    tone(BUZZER_PIN, 1000, 200);
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}