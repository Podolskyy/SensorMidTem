// Prevent multiple inclusion of this header file
#ifndef WIFI_UTILS_H
#define WIFI_UTILS_H

// Include WiFi library for ESP32
#include <WiFi.h>

// Use external credential variables from EEPROM utility
extern String ssid, pass;

// Track whether AP mode is active
bool apModeActive = false;

// Forward declaration for launching the web server (defined elsewhere)
void launchWebServer();

// Function to connect ESP32 to a WiFi network using stored credentials
bool connectToWiFi() {
  WiFi.softAPdisconnect(); // Ensure AP is off
  WiFi.disconnect();       // Disconnect any previous WiFi
  WiFi.mode(WIFI_STA);     // Set WiFi to Station mode
  WiFi.begin(ssid.c_str(), pass.c_str());  // Begin WiFi connection

  Serial.println("Connecting to WiFi...");

  // Wait up to ~25 seconds for connection
  for (int i = 0; i < 50; i++) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi connected!");
      Serial.println(WiFi.localIP());      // Print local IP address
      WiFi.setAutoReconnect(true);         // Reconnect if disconnected
      WiFi.persistent(true);               // Save credentials in flash
      return true;
    }
    Serial.print(".");
    delay(500);
  }

  // If unable to connect after 50 attempts, return false
  Serial.println("\nWiFi connection failed.");
  return false;
}

// Function to start Access Point (AP) mode for WiFi configuration
void startAccessPointMode() {
  const char* ssidap = "IkmalESP32";  // SSID name of the AP
  const char* passap = "";            // No password (open AP)
  WiFi.mode(WIFI_AP);                 // Set ESP32 to AP mode
  WiFi.softAP(ssidap, passap);        // Start the AP

  // Log AP mode info
  Serial.println("\nAP Mode active. Connect to: " + WiFi.softAPIP().toString());
  apModeActive = true;

  // Start the web server so user can enter WiFi credentials
  launchWebServer();
}

// Returns whether the device is currently in AP mode
bool isInAPMode() {
  return apModeActive;
}

#endif  // End of header guard
