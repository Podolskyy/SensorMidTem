#ifndef RELAY_UTILS_H
#define RELAY_UTILS_H  // Prevent multiple inclusion of this header file

// Define GPIO pins used for controlling relays
#define RELAY1_PIN 26
#define RELAY2_PIN 25

// Initializes the relay pins
void initRelays() {
  pinMode(RELAY1_PIN, OUTPUT);  // Set relay 1 pin as output
  pinMode(RELAY2_PIN, OUTPUT);  // Set relay 2 pin as output

  // Ensure both relays are turned off at start
  digitalWrite(RELAY1_PIN, LOW);
  digitalWrite(RELAY2_PIN, LOW);
}

// Functions to turn Relay 1 ON and OFF
void relay1On()  { digitalWrite(RELAY1_PIN, HIGH); }  // Activate relay 1
void relay1Off() { digitalWrite(RELAY1_PIN, LOW);  }  // Deactivate relay 1

// Functions to turn Relay 2 ON and OFF
void relay2On()  { digitalWrite(RELAY2_PIN, HIGH); }  // Activate relay 2
void relay2Off() { digitalWrite(RELAY2_PIN, LOW);  }  // Deactivate relay 2

#endif  // End of header guard
