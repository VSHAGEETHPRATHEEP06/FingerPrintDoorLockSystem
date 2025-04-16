# Fingerprint Door Lock Security System

An Arduino-based biometric security solution that uses fingerprint authentication to control door access.

## Project Overview

This project implements a secure access control system using fingerprint biometrics. When an authorized fingerprint is detected, the system grants access by activating a relay that can control an electronic door lock. The system features visual feedback through an LCD display and LED indicators, along with audible alerts via a buzzer.

## Features

- **Biometric Authentication**: Secure access control using fingerprint recognition
- **User Management**: Enroll new users or delete existing fingerprints via serial commands
- **Visual Feedback**: LCD display shows system status and operation instructions
- **Audible Alerts**: Buzzer provides sound notifications for successful and failed operations
- **LED Indicators**: Visual confirmation of system status
- **Electronic Lock Control**: Relay output to connect to various electronic lock mechanisms

## Components Required

- Arduino (UNO or compatible)
- Adafruit Fingerprint Sensor
- 16x2 I2C LCD Display
- Relay Module (for door lock control)
- Buzzer
- LED
- Power Supply
- Jumper Wires
- Electronic Door Lock (solenoid lock, magnetic lock, or electric strike)

## Software Requirements

- Arduino IDE
- Libraries:
  - Adafruit_Fingerprint
  - SoftwareSerial
  - Wire
  - LiquidCrystal_I2C

## Circuit Diagram

The connection diagram for the system can be found in the file: `FingerPrintDoorLock_CircuitDiagram.png`

## Installation and Setup

1. Connect all components according to the circuit diagram
2. Install all required libraries through the Arduino Library Manager
3. Upload the `FingerPrint_Door_Lock_System.ino` sketch to your Arduino
4. Open the Serial Monitor at 9600 baud to interact with the system

## Usage Instructions

### Normal Operation
- The LCD will display "System Ready" and "Scan Finger"
- Place an enrolled finger on the sensor to gain access
- Successful scans will trigger the relay, activating the door lock

### Enrolling New Fingerprints
1. Send 'e' command through Serial Monitor
2. Enter the ID number (1-127) when prompted
3. Follow LCD instructions to place finger twice
4. System will confirm successful enrollment

### Deleting Fingerprints
1. Send 'd' command through Serial Monitor
2. Enter the ID number to delete
3. System will confirm successful deletion

### Reset to Normal Mode
- Send 'r' command to return to normal operation mode

## Photos

See the `Final Product` folder for images of the completed project.

## Documentation

For detailed project documentation, refer to the included PDF: `Group33-FingerprintDoor LockSecurity SystemUsing Arduino.pdf`

## Project Team

Group 33

