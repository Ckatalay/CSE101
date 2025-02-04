#include <WiFi.h>
#include <ESP32Servo.h>
#include <SPIFFS.h>

Servo myservo;
const char *ssid = "qazwsx";        // WiFi network name
const char *password = "qazwsxedc";   // WiFi network password
const int servoPin = 2;
int pos = 0;           // Variable to store servo position
int delayTime = 1000;  // Default delay time in milliseconds
WiFiServer server(80);
unsigned long onStartTime = 0; // Variable to store start time of opening


// Read delay time from file
void readDelayTime() {
  if (SPIFFS.begin(true)) {
    if (SPIFFS.exists("/time.txt")) {
      File file = SPIFFS.open("/time.txt", "r");
      if (file) {
        String time = file.readString();
        delayTime = time.toInt();
        file.close();
      }
    }
    SPIFFS.end();
  }
}

void setup() {
  Serial.begin(115200);  // Initialize Serial communication
  delay(10);

  // Servo setup
  myservo.attach(servoPin);
  myservo.write(0);

  // Connect to Wi-Fi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  readDelayTime();

  // Handle WiFi client requests
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.print("Click <a href=\"/H\">here</a> to feed the cat.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn the servo motor slowly.<br>");
            
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        // Handle Wi-Fi commands
        if (currentLine.endsWith("GET /H")) {
          onStartTime = millis();

          for (pos = 0; pos <= 55; pos++) {
            myservo.write(pos);
            delay(1);
          }
          delay(delayTime);
          for (pos = 55; pos >= 0; pos--) {
            myservo.write(pos);
            delay(1);
          }
        }
        if (currentLine.endsWith("GET /L")) {
          for (pos = 0; pos <= 70; pos++) {
            myservo.write(pos);
            delay(15);
          }
          for (pos = 70; pos >= 0; pos--) {
            myservo.write(pos);
            delay(15);
          }
        }
        unsigned long duration = (millis() - onStartTime) / 1000;

          Serial.print("Duration:[");
          Serial.print(duration);
          Serial.println("]");
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
