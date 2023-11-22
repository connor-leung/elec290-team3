#include <WiFiNINA.h>
#define SensorPin A0          // the pH meter Analog output is connected with the Arduino’s Analog
unsigned long int avgValue;  //Store the average value of the sensor feedback
//float b;
int buf[10],temp;

#include <EEPROM.h>
#include "GravityTDS.h"
 
#define TdsSensorPin A1
GravityTDS gravityTds;
 
float temperature = 25,tdsValue = 0;

char ssid[] = "ArduinoAP";
char pass[] = "password123";
WiFiServer server(80);

//const int ledPin = 7;

// Placeholder variables for sensor values
float pHValue = 7.0; // Replace with actual pH sensor reading
float conductivityValue = 500; // Replace with actual conductivity sensor reading

void setup() {
  Serial.begin(9600);

  // Set up the Arduino as an access point
  WiFi.beginAP(ssid, pass);

  while (WiFi.status() != WL_AP_LISTENING) {
    delay(1000);
    Serial.println("Creating access point...");
  }

  Serial.println("Access Point created");

  // Set pin 7 as an output
  pinMode(ledPin, OUTPUT);

  // Start the server
  server.begin();

  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization


}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New client connected");

    // Read the first line of the request
    String request = client.readStringUntil('\r');
    Serial.println("Request: " + request);

    // Serve the main page
    serveMainPage(client);

    // Close the connection
    client.stop();
    Serial.println("Client disconnected");
  }
}

// Placeholder function for reading pH value
float readPHSensor() {
  for(int i = 0; i < 10; i++) {
    buf[i] = analogRead(SensorPin);
    delay(10);
  }

  for(int i = 0; i < 9; i++) {
    for(int j = i + 1; j < 10; j++) {
      if(buf[i] > buf[j]) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }

  avgValue = 0;
  for(int i = 2; i < 8; i++) {
    avgValue += buf[i];
  }

  float phValue = (float)avgValue * 5.0 / 1024 / 6;
  phValue = 3.5 * phValue;

  Serial.print("    pH:");
  Serial.print(phValue, 2);
  Serial.println(" ");
  digitalWrite(13, HIGH);
  delay(800);
  digitalWrite(13, LOW);

  return phValue;
}
// Placeholder function for reading conductivity value
float readConductivitySensor() {
    // temperature = readTemperature();  // Add your temperature sensor and read it
  gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
  gravityTds.update();  // sample and calculate
  tdsValue = gravityTds.getTdsValue();  // then get the value
  Serial.print(tdsValue, 0);
  Serial.println("ppm");
  delay(1000);

  return tdsValue;
}

void serveMainPage(WiFiClient client) {
  // Update sensor readings using placeholder functions
  pHValue = readPHSensor();
  conductivityValue = readConductivitySensor();

  // Send the HTTP response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println("<html><body>");
  client.println("<h1>Arduino Web Server</h1>");
  client.println("<p>pH Value: " + String(pHValue, 2) + "</p>");
  client.println("<p>Conductivity Value: " + String(conductivityValue) + "</p>");
  client.println("</body></html>");

  delay(1000);
}