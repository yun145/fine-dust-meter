#include "LiquidCrystal_I2C.h"
#include "DHT.h"
#include "emotion.h"
#include "PMsensor.h"

#define sensitivity  0.1
#define DHTPIN 6
#define DHTTYPE DHT11

const int sensorPin = A0;
const int sensorLED = 3;

PMsensor PM;
LiquidCrystal_I2C lcd(0x27, 16, 2);

DHT dht(DHTPIN, DHTTYPE);

void setEmoticon(float data) {
  if (data > 100) {
    lcd.createChar(4, topAngry1);
    lcd.createChar(5, topAngry2);
    lcd.createChar(6, bottomAngry1);
    lcd.createChar(7, bottomAngry2);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, HIGH);
  }
  else if (data > 50) {
    lcd.createChar(4, topSoSo1);
    lcd.createChar(5, topSoSo2);
    lcd.createChar(6, bottomSoSo1);
    lcd.createChar(7, bottomSoSo2);
    digitalWrite(9, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(11, LOW);
  }

  else {
    lcd.createChar(4, topSmile1);
    lcd.createChar(5, topSmile2);
    lcd.createChar(6, bottomSmile1);
    lcd.createChar(7, bottomSmile2);
    digitalWrite(9, LOW);
    digitalWrite(10, HIGH);
    digitalWrite(11, LOW);
  }
}

void setup() {
  lcd.init();
  lcd.backlight();

  lcd.createChar((byte)0, temp);
  lcd.createChar(1, humi);
  lcd.createChar(2, dust);
  lcd.createChar(3, C);

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  Serial.begin(9600);

  dht.begin();
  PM.init(sensorLED, sensorPin);
}

void loop() {
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();

  if (isnan(humi) || isnan(temp) ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  float data = 0;
  int err = PMsensorErrSuccess;

  if ((err = PM.read(&data, true, sensitivity)) != PMsensorErrSuccess) {
    Serial.print("data Error = ");
    Serial.println(err);
    delay(1500);
    return;
  }
  Serial.println(data);
  delay(1500);
  setEmoticon(data);
  lcd.setCursor(0, 0);
  lcd.write(2);
  lcd.print(" ");
  lcd.print(data);

  if (data > 100)
    lcd.print("ug");
  else
    lcd.print(" ug");
  if ((err = PM.read(&data, true, sensitivity)) != PMsensorErrSuccess) {
    Serial.print("data Error = ");
    Serial.println(err);
    delay(1500);
    return;
  }
  Serial.println(data);
  delay(1500);
  setEmoticon(data);
  lcd.setCursor(0, 0);
  lcd.write(2);
  lcd.print(" ");
  lcd.print(data);

  if (data > 100)
    lcd.print("ug");
  else
    lcd.print(" ug");

  lcd.setCursor(0, 1);
  lcd.write((byte)0);
  lcd.print(" ");
  lcd.print((int)temp);
  lcd.write(3);

  lcd.print(" ");
  lcd.write(1);
  lcd.print(" ");
  lcd.print((int)humi);
  lcd.print("%");

  lcd.setCursor(13, 0);
  lcd.write(4);
  lcd.write(5);

  lcd.setCursor(13, 1);
  lcd.write(6);
  lcd.write(7);
}
