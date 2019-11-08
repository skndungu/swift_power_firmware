#include <EEPROM.h>

const int sensor_pin = A0;
float sensor_value;
int address = 2;
float stored_value;

const int interval = 7000;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
float consumed_units = 1;

void setup() {
  Serial.begin(9600);
  EEPROM.begin(3055);
  Serial.println("Retrieving data...");
  delay(2000);
//  retrieve_from_eeprom();
//  Serial.println(stored_value);

  
}

void loop() {
//   takeReading();
//   store_to_eeprom();
   retrieve_from_eeprom();
//   update_eeprom_cycles();
}

void takeReading() {
  sensor_value = analogRead(sensor_pin);
  delay(10);
  store_to_eeprom();
  Serial.print("Sensor Val: ");
  Serial.println(sensor_value);
  delay(100);
}

void store_to_eeprom() {
    EEPROM.begin(3055);
    address = address + sizeof(float);
    EEPROM.put(address, sensor_value);
    EEPROM.commit();  
    Serial.println(sensor_value);
    EEPROM.end();
}

void retrieve_from_eeprom() {
  EEPROM.begin(3055);
  address = address + sizeof(float);
  stored_value = EEPROM.get(address,stored_value);
  Serial.print("Retrieved Val: ");
  Serial.println(stored_value);
  EEPROM.end();
}

void update_eeprom_cycles(){
    unsigned long currentMillis = millis();
  if ((unsigned long )(currentMillis - previousMillis) >= interval) {
    previousMillis = currentMillis;
    retrieve_from_eeprom();

    stored_value = stored_value -= consumed_units;
    sensor_value = stored_value;
    store_to_eeprom() ;
    Serial.print("Updated eeprom val: ");
    Serial.println(sensor_value);
  }
}
