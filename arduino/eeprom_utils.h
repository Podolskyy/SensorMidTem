// Prevent multiple inclusion of this header file
#ifndef EEPROM_UTILS_H
#define EEPROM_UTILS_H

#include <EEPROM.h>  // Include library for EEPROM handling

// Variables to hold WiFi and device credentials
String ssid = "";
String pass = "";
String devid = "";

// Forward declaration (ensures compiler knows this function exists before it's defined)
void clearEEPROM();

// Reads WiFi SSID, password, and device ID from EEPROM
void readCredentialsFromEEPROM() {
  EEPROM.begin(512);  // Initialize EEPROM with 512 bytes of space
  Serial.println("Reading EEPROM...");

  ssid = "";
  pass = "";
  devid = "";

  // Read SSID from address 0 to 19
  for (int i = 0; i < 20; i++) {
    char c = char(EEPROM.read(i));
    if (c != 0) ssid += c;
  }

  // Read password from address 20 to 39
  for (int i = 20; i < 40; i++) {
    char c = char(EEPROM.read(i));
    if (c != 0) pass += c;
  }

  // Read device ID from address 40 to 59
  for (int i = 40; i < 60; i++) {
    char c = char(EEPROM.read(i));
    if (c != 0) devid += c;
  }

  // Print the credentials to Serial monitor
  Serial.println("SSID: " + ssid);
  Serial.println("PASS: " + pass);
  Serial.println("DEVID: " + devid);

  EEPROM.end();  // End EEPROM session
}

// Writes new credentials (SSID, Password, Device ID) to EEPROM
void writeCredentialsToEEPROM(String a, String b, String c) {
  clearEEPROM();        // Clear existing data before writing
  EEPROM.begin(512);    // Initialize EEPROM

  Serial.println("Writing to EEPROM...");

  // Write SSID starting from address 0
  for (int i = 0; i < a.length(); i++) EEPROM.write(i, a[i]);
  EEPROM.write(a.length(), 0);  // Null-terminator

  // Write Password starting from address 20
  for (int i = 0; i < b.length(); i++) EEPROM.write(20 + i, b[i]);
  EEPROM.write(20 + b.length(), 0);  // Null-terminator

  // Write Device ID starting from address 40
  for (int i = 0; i < c.length(); i++) EEPROM.write(40 + i, c[i]);
  EEPROM.write(40 + c.length(), 0);  // Null-terminator

  EEPROM.commit();  // Save changes to EEPROM
  EEPROM.end();     // End EEPROM session

  Serial.println("Write successful");
}

// Clears the entire EEPROM (sets all 512 bytes to 0)
void clearEEPROM() {
  EEPROM.begin(512);  // Initialize EEPROM
  for (int i = 0; i < 512; i++) EEPROM.write(i, 0);  // Overwrite all bytes
  EEPROM.commit();    // Save changes
  EEPROM.end();       // End session
  Serial.println("EEPROM cleared");
}

#endif  // End of include guard
