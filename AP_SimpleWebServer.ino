#include <WiFiNINA.h>

char ssid[] = "ArduinoAP";
char pass[] = "password123";
WiFiServer server(80);

const int ledPin = 7;

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
  // Replace this with actual code to read pH sensor value
  return 7.0;
}

// Placeholder function for reading conductivity value
float readConductivitySensor() {
  // Replace this with actual code to read conductivity sensor value
  return 500;
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
}

