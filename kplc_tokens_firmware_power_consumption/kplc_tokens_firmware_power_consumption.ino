/////Measuring Current Using ACS712/////
const int current_analogchannel = 0; //Connect current sensor with A0 of Arduino
int sensitivity = 100; // use 100 for 20A Module and 66 for 30A Module
float current_adc_value= 0;
int offset_voltage = 2500; 
double voltage = 0; //voltage measuring
double measured_current = 0;// Current measuring

void setup() {
  Serial.begin(9600);
  
}

void loop() {
  sample_current();
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
  Serial.print("Raw Value = ");
  Serial.print(current_adc_value);

  Serial.print("\t mV = "); // shows the voltage measured 
  Serial.print(voltage,3);

  Serial.print("\t measured_current = ");
  Serial.println(measured_current, 3);

  delay(1500);
}
