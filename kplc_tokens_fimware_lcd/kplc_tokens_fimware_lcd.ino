#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20,4);  
// Set the LCD I2C address

void setup()  
{
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight
  lcd.setCursor(3,0);
  lcd.print("KPLC");
  lcd.setCursor(3,1);
  lcd.print("Units");
  lcd.setCursor(3,2);
  lcd.print("40.94kwh");
  lcd.setCursor(3,3);
  lcd.print("****");
}

void loop() {
  
}
