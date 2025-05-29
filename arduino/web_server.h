//Web Server for AP mode for WiFi Failure/Recovery
#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <WebServer.h>
#include "eeprom_utils.h"

WebServer server(80);
String content = "";

// Forward declarations
void setupWebHandlers();
void handleWebRequests();


void launchWebServer() {
  setupWebHandlers();
  server.begin();
  Serial.println("Web server started");
}

void handleWebRequests() {
  server.handleClient();
}

void setupWebHandlers() {
  server.on("/", []() {
    content = "<html><head><style>.button {background-color: #3CBC8D;color: white;padding: 5px 10px;text-align: center;text-decoration: none;display: inline-block;font-size: 14px;margin: 4px 2px;cursor: pointer;}input[type=text],[type=password]{width: 100%;padding: 5px 10px;margin: 5px 0;box-sizing: border-box;border: none;background-color: #3CBC8D;color: white;}</style></head><body>";
    content += "<h1>WIFI MANAGER</h1><br><h3>Current Settings</h3>";
    content += "<table><tr><td><label> WiFi SSID</label></td><td><label>" + ssid + "</label></td></tr>";
    content += "<tr><td><label> WiFi Password</label></td><td><label>" + pass + "</label></td></tr>";
    content += "<tr><td><label> Dev ID</label></td><td><label>" + devid + "</label></td></tr></table><br><br>";
    content += "<form method='get' action='setting'>";
    content += "<h3>New WiFi Settings</h3><table><tr><td><label>WiFi SSID</label></td><td><input type='text' name='ssid' length=32></td></tr>";
    content += "<tr><td><label>WiFi Password</label></td><td><input type='password' name='password' length=32></td></tr>";
    content += "<tr><td><label>Device ID</label></td><td><input type='text' name='devid' length=32></td></tr>";
    content += "<tr><td></td><td><input class='button' type='submit'></td></tr></table></form>";
    content += "</body></html>";
    server.send(200, "text/html", content);
  });

  server.on("/setting", []() {
    String ssidw = server.arg("ssid");
    String passw = server.arg("password");
    String devidw = server.arg("devid");
    writeCredentialsToEEPROM(ssidw, passw, devidw);
    content = "Success..please reboot";
    server.send(200, "text/html", content);
    delay(2000);
    ESP.restart();
  });

  server.on("/clear", []() {
    clearEEPROM();
    server.send(200, "text/html", "EEPROM Cleared. Please reboot.");
    delay(2000);
    ESP.restart();
  });
}

#endif
