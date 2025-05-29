#ifndef RELAY_UTILS_H
#define RELAY_UTILS_H

#define RELAY1_PIN 26
#define RELAY2_PIN 25

void initRelays() {
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  digitalWrite(RELAY1_PIN, LOW);
  digitalWrite(RELAY2_PIN, LOW);
}

void relay1On() { digitalWrite(RELAY1_PIN, HIGH); }
void relay1Off() { digitalWrite(RELAY1_PIN, LOW); }

void relay2On() { digitalWrite(RELAY2_PIN, HIGH); }
void relay2Off() { digitalWrite(RELAY2_PIN, LOW); }

#endif
