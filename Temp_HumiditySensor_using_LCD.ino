#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// ----------------------
// Pin Definitions
// ----------------------
#define DHTPIN 2
#define DHTTYPE DHT11

// ----------------------
// LCD Address
// Usually 0x27 or 0x3F
// ----------------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  lcd.init();
  lcd.backlight();

  dht.begin();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Weather");
  lcd.setCursor(0, 1);
  lcd.print(" Monitor");

  delay(2000);
}

void loop()
{
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if reading failed
  if (isnan(humidity) || isnan(temperature))
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!");
    delay(2000);
    return;
  }

  lcd.clear();

  // First Line
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temperature, 1);
  lcd.write(byte(223)); // Degree symbol
  lcd.print("C");

  // Second Line
  lcd.setCursor(0, 1);
  lcd.print("Hum :");
  lcd.print(humidity, 1);
  lcd.print("%");

  delay(3000);
}