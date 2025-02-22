#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Airon_";
const char* password = "12345678";

WebServer server(80);

const int ledPins[] = {33, 27, 14, 12, 13};
bool allOn = false;

void setupWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void setupLEDs() {
  for (int pin : ledPins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>ESP32 LED Control</title>";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  html += "<h1>ESP32 LED Control</h1>";
  html += "<button onclick=\"location.href='/LED=ALL'\">Toggle All LEDs</button>";
  html += "<button onclick=\"location.href='/LED=SEQ1'\">Sequence 1</button>";
  html += "<button onclick=\"location.href='/LED=SEQ2'\">Sequence 2</button>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleLED() {
  String request = server.uri();
  if (request == "/LED=ALL") {
    toggleAllLEDs();
  } else if (request == "/LED=SEQ1") {
    playSequence1();
  } else if (request == "/LED=SEQ2") {
    playSequence2();
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void toggleAllLEDs() {
  allOn = !allOn;
  for (int pin : ledPins) {
    digitalWrite(pin, allOn ? HIGH : LOW);
  }
}

void playSequence1() {
  for (int pin : ledPins) {
    digitalWrite(pin, HIGH);
    delay(300);
    digitalWrite(pin, LOW);
  }
}

void playSequence2() {
  sequenceOne(); sequenceTwo(); sequenceThree(); sequenceTwo(); sequenceOne(); clearLEDs();
}

void setup() {
  Serial.begin(115200);
  setupWiFi();
  setupLEDs();
  server.on("/", handleRoot);
  server.on("/LED=ALL", handleLED);
  server.on("/LED=SEQ1", handleLED);
  server.on("/LED=SEQ2", handleLED);
  server.begin();
}

void loop() {
  server.handleClient();
}

void clearLEDs() {
  for (int pin : ledPins) {
    digitalWrite(pin, LOW);
  }
  delay(100);
}

void sequenceOne() { clearLEDs(); digitalWrite(14, HIGH); delay(300); }
void sequenceTwo() { clearLEDs(); digitalWrite(27, HIGH); digitalWrite(12, HIGH); delay(300); }
void sequenceThree() { clearLEDs(); digitalWrite(33, HIGH); digitalWrite(13, HIGH); delay(300); }