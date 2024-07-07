#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <UrlEncode.h>

#define Sensor D0
#define LED D1
#define Buzzer D2

const char* ssid = "*********";//Enter your SSID
const char* password = "*********";//Enter your password

String phoneNumber = "**********"; //country_code + phone number
String apiKey = "**********";

void setup() {
  Serial.begin(115200);
  pinMode(Buzzer, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(Sensor, INPUT);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // Send Message to WhatsAPP
  sendAlert("Your system is ready!");
}

void loop() {
  bool sensorValue = digitalRead(Sensor);
  if (sensorValue == 1) {
    digitalWrite(Buzzer, HIGH);
    digitalWrite(LED, HIGH);
    sendAlert("Warning!");
    delay(1500);
  } else {
    digitalWrite(Buzzer, LOW);
    digitalWrite(LED, LOW);
  }
}


void sendAlert(String message) {

  // Data to send with HTTP POST
  String url = "http://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&text=" + urlEncode(message) + "&apikey=" + apiKey;
  WiFiClient client;
  HTTPClient http;
  http.begin(client, url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200) {
    Serial.print("Message sent successfully");
  }
  else {
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end(); // Data to send with HTTP POST
}
