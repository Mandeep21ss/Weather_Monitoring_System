# Arduino Weather Monitoring System

A portable embedded systems project that measures real-time temperature and humidity using a DHT11 sensor and displays the readings on a 16×2 LCD via I2C communication.

---

## How It Works

The DHT11 sensor reads temperature and humidity every 3 seconds. The Arduino processes the values and updates the LCD display. If the sensor fails to respond, the display shows a `Sensor Error!` message instead of invalid data.

**Startup:** Shows a `Weather / Monitor` splash screen for 2 seconds on power-on.

---

## Display Format

```
Temp:28.4°C
Hum :61.0%
```

| State | Row 0 | Row 1 |
|-------|-------|-------|
| Startup | ` Weather` | ` Monitor` |
| Normal | `Temp:28.4°C` | `Hum :61.0%` |
| Error | `Sensor Error!` | *(blank)* |

---

## Hardware

### Components

| Component | Quantity |
|-----------|----------|
| Arduino Uno | 1 |
| DHT11 Temperature & Humidity Sensor | 1 |
| 16×2 LCD Display | 1 |
| I2C LCD Module (PCF8574) | 1 |
| Breadboard | 1 |
| Jumper Wires | Several |
| 9V Battery + Connector | 1 |

### Pin Connections

**DHT11 Sensor**

| DHT11 | Arduino |
|-------|---------|
| VCC | 5V |
| GND | GND |
| DATA | D2 |

**I2C LCD Module**

| LCD | Arduino |
|-----|---------|
| VCC | 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

### Wiring Diagram

```
DHT11 DATA ──────────────────── D2
DHT11 VCC  ──────────────────── 5V
DHT11 GND  ──────────────────── GND

LCD SDA ──────────────────────── A4
LCD SCL ──────────────────────── A5
LCD VCC ──────────────────────── 5V
LCD GND ──────────────────────── GND
```

---

## Libraries Required

| Library | Install |
|---------|---------|
| `Wire.h` | Built into Arduino IDE |
| `LiquidCrystal_I2C` | Library Manager → search `LiquidCrystal I2C` |
| `DHT.h` | Library Manager → search `DHT sensor library` (Adafruit) |
| `Adafruit Unified Sensor` | Library Manager → search `Adafruit Unified Sensor` |

---

## Code

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN  2
#define DHTTYPE DHT11

// Change to 0x3F if display stays blank
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
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

void loop() {
  float humidity    = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!");
    delay(2000);
    return;
  }

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temperature, 1);
  lcd.write(byte(223)); // Degree symbol
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Hum :");
  lcd.print(humidity, 1);
  lcd.print("%");

  delay(3000);
}
```

---

## Setup

1. Wire components as shown above
2. Install all required libraries via Arduino IDE Library Manager
3. Upload the sketch to Arduino Uno
4. Power via USB or 9V battery
5. LCD shows splash screen then live readings

---

## I2C Address Troubleshooting

If the LCD stays blank, your module may use address `0x3F` instead of `0x27`. Run this I2C scanner to find the correct address:

```cpp
#include <Wire.h>
void setup() {
  Wire.begin();
  Serial.begin(9600);
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found: 0x");
      Serial.println(addr, HEX);
    }
  }
}
void loop() {}
```

Open Serial Monitor at **9600 baud** and note the address. Then update:

```cpp
LiquidCrystal_I2C lcd(0x3F, 16, 2); // use your address here
```

---

## Limitations

- DHT11 accuracy: ±2°C temperature, ±5% humidity
- Indoor use only — not suitable for outdoor environments
- No data logging between power cycles
- No wireless or remote monitoring

## Future Improvements

- Replace DHT11 with DHT22 for higher accuracy
- Add ESP32 for Wi-Fi and cloud data upload
- Add SD card for data logging
- Add BMP280 for atmospheric pressure readings
- Add RTC (DS3231) to timestamp readings

---

## License

MIT License — free to use, modify, and distribute.
