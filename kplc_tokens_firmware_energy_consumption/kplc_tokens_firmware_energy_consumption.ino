/////Measuring Energy ACS712/////
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20,4); 

const int lcd_vcc = 9;


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

void setup() {
  Serial.begin(9600);
  pinMode(lcd_vcc, OUTPUT);
  digitalWrite(lcd_vcc,HIGH);
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight
  lcd.setCursor(3,0);
  lcd.print("KPLC");
  lcd.setCursor(3,1);
  lcd.print("Units");
  lcd.setCursor(0,3);
  lcd.print("Prototype Welcome...");
//  lcd.setCursor(3,3);
//  lcd.print("****");
}

void loop() {
  sample_current();
  getPower();
}

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
