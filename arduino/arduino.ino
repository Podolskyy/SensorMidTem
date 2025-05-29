#include "wifi_utils.h"
#include "eeprom_utils.h"
#include "web_server.h"
#include "dht_utils.h"
#include "relay_utils.h"
#include "display_utils.h"
#include <HTTPClient.h>

const char* serverEndpoint = "http://192.168.1.3:8080/backend/insert_data.php";  // Replace with your real PHP endpoint

void setup() {
  Serial.begin(115200);

  initDisplay();
  initDHT();
  initRelays();
  readCredentialsFromEEPROM();

  if (!connectToWiFi()) {
    launchWebServer();  // fallback to AP config mode
    return;
  }

  showStartupScreen();
}

void loop() {
  float temp = readTemperature();
  float hum = readHumidity();
  temp=temp+15;
  hum=hum-50;
  bool tempAbnormal = (temp < 22.0 || temp > 36.0);
  bool humAbnormal = (hum < 50.0 || hum > 95.0);
  

  if (tempAbnormal || humAbnormal) {
    relay1On();
    relay2On();
  }

  // Determine status message
  String status;
  if (tempAbnormal && humAbnormal)
    status = "Temp & Hum Abnormal";
  else if (tempAbnormal)
    status = "Temp Abnormal";
  else if (humAbnormal)
    status = "Humid Abnormal";
  else
    status = "Normal";

  // Send to server via HTTP POST
  HTTPClient http;
  http.begin(serverEndpoint);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String postData = "temperature=" + String(temp) + "&humidity=" + String(hum);
  int responseCode = http.POST(postData);

  if (responseCode > 0) {
    Serial.println("Data sent successfully: " + http.getString());
  } else {
    Serial.println("Failed to send data. HTTP Error: " + String(responseCode));
  }
  http.end();
  tempAbnormal = true;
  // Activate relay if abnormal

  // Countdown on OLED
  for (int i = 10; i > 0; i--) {
    updateDisplay("Data in " + String(i) + " sec",
                  "Current: " + status,
                  String(temp) + " C, " + String(hum) + " %");
    delay(1000);
  }

  if (tempAbnormal || humAbnormal) {
    relay1Off();
    relay2Off();
  }
}
