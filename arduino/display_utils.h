#ifndef DISPLAY_UTILS_H
#define DISPLAY_UTILS_H  // Prevent multiple inclusion of this header file

// Include required libraries for OLED display
#include <Wire.h>                  // I2C communication
#include <Adafruit_GFX.h>         // Core graphics library
#include <Adafruit_SSD1306.h>     // OLED driver library

// Define OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// OLED reset pin (not used in this configuration)
#define OLED_RESET -1

// Define I2C pins for ESP32
#define I2C_SDA 4
#define I2C_SCL 15

// Create an instance of the OLED display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Function to initialize the OLED display
void initDisplay() {
  Wire.begin(I2C_SDA, I2C_SCL);                          // Start I2C with custom SDA/SCL pins
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);             // Initialize display with I2C address 0x3C
  display.clearDisplay();                                // Clear any previous display data
  display.setTextColor(SSD1306_WHITE);                   // Set text color to white
  display.setTextSize(1);                                // Set text size
  display.setCursor(0, 0);                               // Set cursor to top-left
  display.print("OLED Ready");                           // Show initial message
  display.display();                                     // Render to the screen
  delay(1000);                                           // Hold the message for a second
}

// Function to update display with 3 lines of text
void updateDisplay(String line1, String line2, String line3) {
  display.clearDisplay();                // Clear the display before showing new data
  display.setCursor(0, 0);               // First line position
  display.println(line1);
  display.setCursor(0, 10);              // Second line position
  display.println(line2);
  display.setCursor(0, 20);              // Third line position
  display.println(line3);
  display.display();                     // Render all lines on the screen
}

// Function to show a fixed startup screen message
void showStartupScreen() {
  updateDisplay("Module Check", "DHT + Relay", "Running...");
}

#endif  // End of header guard
