//For WiFi & AP related processes
#ifndef WIFI_UTILS_H
#define WIFI_UTILS_H

#include <WiFi.h>

extern String ssid, pass;
bool apModeActive = false;
void launchWebServer();  // Forward declaration


bool connectToWiFi() {
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), pass.c_str());

  Serial.println("Connecting to WiFi...");
  for (int i = 0; i < 50; i++) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi connected!");
      Serial.println(WiFi.localIP());
      WiFi.setAutoReconnect(true);
      WiFi.persistent(true);
      return true;
    }
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi connection failed.");
  return false;
}

void startAccessPointMode() {
  const char* ssidap = "IkmalESP32";
  const char* passap = "";
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssidap, passap);
  Serial.println("\nAP Mode active. Connect to: " + WiFi.softAPIP().toString());
  apModeActive = true;
  launchWebServer();
}

bool isInAPMode() {
  return apModeActive;
}

#endif
