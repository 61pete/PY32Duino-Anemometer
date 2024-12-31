//  Original code and more Information at: https://www.aeq-web.com/
// Version 2.0 | 11-NOV-2020

/** Modified for PY32F030EK28 mcu V1.0 | 29-DEC-2024
* using PY32 Arduino V 0.1.3 https://github.com/py32duino/Arduino-PY32
* and additional board manager url below.
* https://github.com/PY32Duino/Arduino-pack-json-ci/releases/download/Nightly/package_py32_index.json
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>      // Adafruit GFX Library V1.1.12
#include <Adafruit_SSD1306.h>  // V2.5.13

const int RecordTime = 3;   // Define Measuring Time (Seconds)
const int SensorPin = PA7;  // Define Interrupt PA7 ie pin13 on the ic.

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(128, 64, &Wire, -1);



int InterruptCounter;
int WindSpeed;

void setup() {
  Serial.begin(115200);
  // initialize with the I2C addr 0x3C
  Wire.setSDA(PA10);
  Wire.setSCL(PA9);
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setRotation(0);

  // Clear the buffer.
  display.clearDisplay();
}

void loop() {
  measure();

  display.display();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5, 4);
  display.println("Wind speed");
  display.setTextSize(4);
  display.setCursor(10, 30);
  display.println(WindSpeed);
  display.setTextSize(2);
  display.setCursor(85, 40);
  display.println("kmh");
  display.display();
  delay(3000);
  display.clearDisplay();
  Serial.print("Wind Speed:");
  Serial.print(WindSpeed);  // Speed in km/h
  Serial.print("km/h -");
}

void measure() {
  InterruptCounter = 0;
  attachInterrupt(digitalPinToInterrupt(SensorPin), countup, RISING);
  delay(1000 * RecordTime);
  detachInterrupt(digitalPinToInterrupt(SensorPin));
  WindSpeed = (float)InterruptCounter / (float)RecordTime * 2.396;  // 2.396 is the constant for a radius of 106mm of the anemometer.
}

void countup() {
  InterruptCounter++;
}
