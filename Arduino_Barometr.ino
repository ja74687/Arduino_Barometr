#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <LiquidCrystal_I2C.h>

Adafruit_BMP280 bme; // I2C
LiquidCrystal_I2C lcd(0x3F, 16, 2);
int i = 0;
int avg = 0;
int onOffScreen = 1;
int pressureAVG[10];
unsigned long timer;

byte stopnie[8] = {
  0b00100,
  0b01010,
  0b10001,
  0b01010,
  0b00100,
  0b00000,
  0b00000,
  0b00000
};
byte downArrow[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b10101,
  0b01110,
  0b00100
};
byte upArrow[8] = {
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100
};



void setup() {
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  pinMode(4, INPUT);
  lcd.init();
  lcd.init();
  lcd.backlight();
  if (!bme.begin()) {
    lcd.setCursor(0, 0);
    lcd.print("BMP280 Error");
    while (1);
  }
  lcd.createChar(0, stopnie);
  lcd.createChar(1, downArrow);
  lcd.createChar(2, upArrow);

}

void loop() {

  if (millis() > timer) {
    timer = millis() + 60000;
    displayLCD((int)bme.readTemperature(), (int)(bme.readPressure() / 100), (int)bme.readAltitude(1013.25));


  }

  if (digitalRead(4) == HIGH)
  {
    if (onOffScreen == 0)
    {
      lcd.noBacklight();
      onOffScreen = 1;
    } else {
      lcd.backlight();
      onOffScreen = 0;
    }
    delay(250);
  }
}

void displayLCD(int temp, int pressure, int altitude)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp);
  lcd.print((char)0);
  lcd.print("C");
  lcd.setCursor(8, 0);
  lcd.print("Wys:");
  lcd.print(altitude);
  lcd.print("m");
  lcd.setCursor(1, 1);
  lcd.print("Cis:");
  lcd.print(pressure);
  lcd.print(" hPa");

  //----------------------------- Przewidywana tendencja -------------------
  if (i < 10)
  {
    pressureAVG[i] = pressure;
    if (i == 9)
    {
      double x = 0;

      for (int j = 0; j < 10; j++)
      {
        x += pressureAVG[j];
      }
      avg = x / 10;

    }
  } else {
    i = 0;
  }
  i++;
  if (avg == pressure)
  {
    lcd.setCursor(14, 1);
    lcd.print("-");
  } else if (avg < bme.readAltitude(1013.25)) {
    lcd.setCursor(14, 1);
    lcd.print((char)2);
  } else if (avg > bme.readAltitude(1013.25)) {
    lcd.setCursor(14, 1);
    lcd.print((char)1);
  }
  //-------------------------------------------------------------------

}
