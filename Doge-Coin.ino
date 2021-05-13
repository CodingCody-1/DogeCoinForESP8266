#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>

String price;
const char* serverPath = "https://api.cryptonator.com/api/ticker/doge-usd";
const char* fingerprint = "10 76 19 6B E9 E5 87 5A 26 12 15 DE 9F 7D 3B 92 9A 7F 30 13";         //Change this sometime before it expires
int Delay = 0;

ESP8266WiFiMulti wifiMulti;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  wifiMulti.addAP("SSID-1", "PASSWORD-1");   // add Wi-Fi networks you want to connect to
  wifiMulti.addAP("SSID-2", "PASSWORD-2");
  wifiMulti.addAP("SSID-3", "PASSWORD-3");

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(1000);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer
}

void loop() {
  if (Delay == 20) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
  
      digitalWrite(LED_BUILTIN, LOW);
  
      http.begin(serverPath, fingerprint);
      int httpResponseCode = http.GET();
  
      if (httpResponseCode > 0) {
        Serial.print('\n');
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
  
        price = payload.substring(49, 59);
        Serial.println(price);

        digitalWrite(LED_BUILTIN, HIGH);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }

      http.end();
      delay(1000);
      Delay = 0;
    }
  }
    if (Delay < 20) {
      Serial.print(" ");
      Serial.print(Delay);
      delay(1000);
      Delay = Delay + 1;
    }
    if (Delay > 20) {
      Delay = 0;
    }
}
