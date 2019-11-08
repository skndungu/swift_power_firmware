#include <Wire.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "THE DEVELOPER";
const char* password = "MANAGEMENT2";

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // Set the LCD I2C address


////////////  Variables //////////

////////// Energy Variables /////////

const int current_analogchannel = 0; //Connect current sensor with A0 of Arduino
unsigned int sensitivity = 100; // use 100 for 20A Module and 66 for 30A Module
float current_adc_value= 0;
int offset_voltage = 2500; 
double voltage = 0; //voltage measuring
double measured_current = 0;// Current measuring

float ac_supply_voltage = 220.0; // ac reading from the mains
float power = 0.0;
float watt_hr = 0;
float kWh = 0;
float average_watt_hr =0.0;

unsigned long last_time = 0;
unsigned long current_time = 0;
unsigned long interval = 100;
unsigned int calibration = 100;
float pF = 0.85;
float to_hours = 3600000.0;


double measured_current_offset = 0.1201;

/////////////// EEPROM variables /////////////

const int address = 20;
float stored_value;

const int eeprom_update_interval = 7000; // intervals between http get requests
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

const int flicker_interval = 5000; // intervals between load units fliker
unsigned long flicker_previousMillis = 0;
unsigned long flicker_currentMillis = 0;

float consumed_units = 1.34;


///////////// HTTP & JSON PARSER Variables ///////////
String received_json;

StaticJsonDocument<2000> doc;


void setup()
{
  EEPROM.begin(3055);
  Serial.begin(9600);
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight
  lcd.setCursor(3, 0);
  lcd.print("KPLC");
  lcd.setCursor(3, 1);
  lcd.print("Units");
  lcd.setCursor(3, 2);
  lcd.print("40.94kwh");
  lcd.setCursor(3, 3);
  lcd.print("****");
  retrieve_from_eeprom(); // functions retrieves data from the eeprom when the system initiates


  ////   Parsing JSON  //////

    Serial.begin(115200);
  while (!Serial) continue;

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(3000);
    Serial.print("Connecting..");
  }

  

  
}

void loop() {
//  load_units();
  connect_n_parse_json(); // function establishes a connection with the Internet
  store_to_eeprom();  // Functions stores data to the EEPROM
  update_eeprom_cycles();
}

///////////////// Updating to EEPROM ////////////////
void store_to_eeprom() {
  EEPROM.begin(3055);
  EEPROM.put(address, kWh);

}

void retrieve_from_eeprom() {
  EEPROM.begin(3055);
  stored_value = EEPROM.get(address, stored_value);
  kWh = stored_value;
  Serial.print("Retrieved Val: ");
  Serial.println(stored_value);
  update_on_lcd();
}

void update_eeprom_cycles() {
  unsigned long currentMillis = millis();
  if ((unsigned long )(currentMillis - previousMillis) >= eeprom_update_interval) {
    previousMillis = currentMillis;

    kWh = kWh -= consumed_units;
    store_to_eeprom();
    Serial.print("Updated eeprom val: ");
    Serial.println(kWh);
  }
}

void update_on_lcd() {
  lcd.setCursor(3, 2);
  lcd.print(kWh);
  lcd.print("kWh");
}

void load_units() {
  lcd.init();   // initializing the LCD
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("KPLC");
  lcd.setCursor(3, 1);
  lcd.print("Units");
  lcd.setCursor(3, 2);
  lcd.print("0 kwh");
  lcd.setCursor(3, 3);
  lcd.print("Load Units!!");
}


///////////////// Energy Consumption ////////////////
void sample_current() {
  unsigned int sampled_sensor_val = 0;
  float max_point = 0;
  for(int i=0; i<500; i++) {
    if(sampled_sensor_val = analogRead(current_analogchannel), sampled_sensor_val > max_point)
    max_point = sampled_sensor_val;
  }

  current_adc_value = max_point;
  voltage = (current_adc_value / 1023.0) * 5000; // Gets the voltage in mV
  measured_current = ((voltage - offset_voltage) / sensitivity);
  measured_current = (measured_current) / (sqrt(2) );
  measured_current = measured_current - measured_current_offset;
  Serial.print("Raw Value = ");
  Serial.print(current_adc_value);

  Serial.print("\t mV = "); // shows the voltage measured 
  Serial.print(voltage,3);

  Serial.print("\t measured_current = ");
  Serial.println(measured_current, 3);

  delay(1500);
}

void getPower() {
  power = (ac_supply_voltage * measured_current) * pF;
  Serial.print("Power");
  Serial.println(power);
  last_time = current_time;
  current_time = millis();
  watt_hr = watt_hr + power * ((current_time - last_time) / to_hours );
  kWh = watt_hr/1000;
  if(kWh < 0.00) {
    kWh = 0;
  }
  else {
      kWh = watt_hr/1000;
  }
  Serial.print(watt_hr);
  Serial.println("Wh");
  lcd.setCursor(3,2);
  lcd.print(kWh);
  lcd.print(" ");
  lcd.print("kWh");
}


///////////// Parsing JSON ////////////

void connect_n_parse_json() {
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
