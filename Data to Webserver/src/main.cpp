#include <WiFiNINA.h>

char ssid[] = "ArduinoAP";  // Set the name for your access point
char pass[] = "password123";  // Set the password for your access point
WiFiServer server(80);  // create a server at port 80

int ledPin = 7;  // LED connected to digital pin 7

void setup() {
  // initialize serial communication:
  Serial.begin(9600);

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  // attempt to start the WiFi module:
  if (WiFi.beginAP(ssid, pass) != WL_AP_LISTENING) {
    Serial.println("Starting access point failed!");
    // don't continue
    while (true);
  }

  // you're connected now, so print out the status:
  // printWiFiStatus();

  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);

  // start the server:
  server.begin();
}

void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          // HTML webpage
          client.println("<!DOCTYPE html><html><body>");
          client.println("<h1>Arduino LED Control</h1>");
          client.println("<form action=\"/on\" method=\"get\"><input type=\"submit\" value=\"Turn On\"></form>");
          client.println("<form action=\"/off\" method=\"get\"><input type=\"submit\" value=\"Turn Off\"></form>");
          client.println("</body></html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("Client disconnected");
  }
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.println(rssi);
}
