#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "THE DEVELOPER";
const char* password = "MANAGEMENT2";

String received_json;

StaticJsonDocument<2000> doc;

void setup () {
  Serial.begin(115200);
  while (!Serial) continue;

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(3000);
    Serial.print("Connecting..");
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin("http://jsonplaceholder.typicode.com/users/7");  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);

      // Deserialize the JSON document
      DeserializationError error = deserializeJson(doc, payload);

      // Test if parsing succeeds.
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }

      int id = doc["id"];
      Serial.print("My JsonID is: ");
      Serial.println(String(id));

    }
    http.end();   //Close connection
  }
  delay(30000);    //Send a request every 30 seconds
}



void print_client_data() {
  Serial.print("Id = ");
}
