// Include custom utility libraries
#include "wifi_utils.h"       // WiFi connection and AP mode utilities
#include "eeprom_utils.h"     // Reading WiFi credentials from EEPROM
#include "web_server.h"       // Web server for AP fallback configuration
#include "dht_utils.h"        // Reading from DHT sensor
#include "relay_utils.h"      // Relay control functions
#include "display_utils.h"    // OLED display handling
#include <HTTPClient.h>       // For sending HTTP requests

// Backend endpoint to send sensor data
const char* serverEndpoint = "http://192.168.1.3:8080/backend/insert_data.php";

void setup() {
  Serial.begin(115200);  // Start serial communication for debugging

  initDisplay();         // Initialize OLED display
  initDHT();             // Initialize DHT sensor
  initRelays();          // Set relay pins as output
  readCredentialsFromEEPROM(); // Load saved WiFi credentials

  // Attempt to connect to WiFi
  if (!connectToWiFi()) {
    launchWebServer();   // Launch AP mode if WiFi fails
    return;
  }

  showStartupScreen();   // Show a startup screen on OLED
}

void loop() {
  // Read temperature and humidity from DHT sensor
  float temp = readTemperature();
  float hum = readHumidity();

  // Check if readings are outside the acceptable range
  bool tempAbnormal = (temp < 22.0 || temp > 36.0);
  bool humAbnormal = (hum < 50.0 || hum > 95.0);

  // Turn on relays if abnormal values are detected
  if (tempAbnormal || humAbnormal) {
    relay1On();
    relay2On();
  }

  // Prepare status message based on abnormal readings
  String status;
  if (tempAbnormal && humAbnormal)
    status = "Temp & Hum Abnormal";
  else if (tempAbnormal)
    status = "Temp Abnormal";
  else if (humAbnormal)
    status = "Humid Abnormal";
  else
    status = "Normal";

  // Prepare and send HTTP POST request to backend
  HTTPClient http;
  http.begin(serverEndpoint);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Format the data for sending
  String postData = "temperature=" + String(temp) + "&humidity=" + String(hum);
  int responseCode = http.POST(postData); // Send POST request

  // Log the result
  if (responseCode > 0) {
    Serial.println("Data sent successfully: " + http.getString());
  } else {
    Serial.println("Failed to send data. HTTP Error: " + String(responseCode));
  }
  http.end(); // Close the connection

  // Countdown with OLED updates every second
  for (int i = 10; i > 0; i--) {
    updateDisplay("Data in " + String(i) + " sec",
                  "Current: " + status,
                  String(temp) + " C, " + String(hum) + " %");
    delay(1000); // Wait 1 second
  }

  // Turn off relays after delay (if they were turned on)
  if (tempAbnormal || humAbnormal) {
    relay1Off();
    relay2Off();
  }
}
