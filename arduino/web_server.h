// Header guard to prevent multiple inclusion
#ifndef WEB_SERVER_H
#define WEB_SERVER_H

// Include WebServer and EEPROM utilities
#include <WebServer.h>
#include "eeprom_utils.h"

// Create a web server instance on port 80
WebServer server(80);

// Variable to hold dynamically generated HTML content
String content = "";

// Function declarations
void setupWebHandlers();   // Set up all web request routes
void handleWebRequests();  // Process client requests

// Launches the web server and initializes handlers
void launchWebServer() {
  setupWebHandlers();   // Define routes and behavior
  server.begin();       // Start listening for HTTP requests
  Serial.println("Web server started");
}

// Handles incoming HTTP requests
void handleWebRequests() {
  server.handleClient();  // Responds to client if any request is pending
}

// Defines the web routes and corresponding behavior
void setupWebHandlers() {
  // Route: "/"
  server.on("/", []() {
    // Build the main HTML page with current WiFi settings and input form
    content = "<html><head><style>"
              ".button {background-color: #3CBC8D; color: white; padding: 5px 10px; text-align: center; text-decoration: none; display: inline-block; font-size: 14px; margin: 4px 2px; cursor: pointer;}"
              "input[type=text],[type=password] {width: 100%; padding: 5px 10px; margin: 5px 0; box-sizing: border-box; border: none; background-color: #3CBC8D; color: white;}"
              "</style></head><body>";
    content += "<h1>WIFI MANAGER</h1><br><h3>Current Settings</h3>";
    content += "<table><tr><td><label> WiFi SSID</label></td><td><label>" + ssid + "</label></td></tr>";
    content += "<tr><td><label> WiFi Password</label></td><td><label>" + pass + "</label></td></tr>";
    content += "<tr><td><label> Dev ID</label></td><td><label>" + devid + "</label></td></tr></table><br><br>";

    // Form to enter new credentials
    content += "<form method='get' action='setting'>";
    content += "<h3>New WiFi Settings</h3><table>";
    content += "<tr><td><label>WiFi SSID</label></td><td><input type='text' name='ssid' length=32></td></tr>";
    content += "<tr><td><label>WiFi Password</label></td><td><input type='password' name='password' length=32></td></tr>";
    content += "<tr><td><label>Device ID</label></td><td><input type='text' name='devid' length=32></td></tr>";
    content += "<tr><td></td><td><input class='button' type='submit'></td></tr></table></form>";
    content += "</body></html>";

    // Send the page to the client
    server.send(200, "text/html", content);
  });

  // Route: "/setting" — Save new credentials and reboot
  server.on("/setting", []() {
    // Retrieve input values from query parameters
    String ssidw = server.arg("ssid");
    String passw = server.arg("password");
    String devidw = server.arg("devid");

    // Save the credentials to EEPROM
    writeCredentialsToEEPROM(ssidw, passw, devidw);

    // Confirm success and reboot
    content = "Success..please reboot";
    server.send(200, "text/html", content);
    delay(2000);  // Give user time to read the message
    ESP.restart(); // Restart ESP32 to apply new WiFi settings
  });

  // Route: "/clear" — Clear EEPROM and reboot
  server.on("/clear", []() {
    clearEEPROM();  // Remove all saved credentials
    server.send(200, "text/html", "EEPROM Cleared. Please reboot.");
    delay(2000);
    ESP.restart();
  });
}

#endif  // End of header guard
