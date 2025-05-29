#ifndef DHT_UTILS_H
#define DHT_UTILS_H  // Prevent multiple inclusions of this header

// Include the DHT sensor library
#include <DHT.h>

// Define the GPIO pin connected to the DHT sensor
#define DHTPIN 27     // You can change this pin as needed

// Define the type of DHT sensor being used (DHT11, DHT22, etc.)
#define DHTTYPE DHT11 // Change to DHT22 if using that sensor

// Create a DHT sensor object
DHT dht(DHTPIN, DHTTYPE);

// Initialize the DHT sensor
void initDHT() {
  dht.begin();  // Start communication with the DHT sensor
}

// Function to read temperature from the DHT sensor
float readTemperature() {
  return dht.readTemperature(); // Returns temperature in Celsius
}

// Function to read humidity from the DHT sensor
float readHumidity() {
  return dht.readHumidity(); // Returns relative humidity in %
}

#endif  // End of include guard
