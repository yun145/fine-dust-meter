# fine-dust-meter
<h5>제 미세먼지 측정기의 핵심은 측정기가 설치 되지 않은 곳에서도 측정하고 대비할 수 있도록 하기 위함 입니다. ╰(*°▽°*)╯</h5>
<h5>코코아펩의 미세먼지 측정기 키트를 사용해 제작하였습니다. </h5>
<h8>https://www.kocoafab.cc/make/view/729</h8>
<hr>
 
 
 
 * 미세먼지 센서
   * 미세먼지 측정하기
   ```python
    #include "LiquidCrystal_I2C.h"     //라이브러리
    #include "DHT.h"
    #include "emotion.h"
    #include "PMsensor.h"

    #define sensitivity  0.1
    #define DHTPIN 6
    #define DHTTYPE DHT11

    const int sensorPin = A0;
    const int sensorLED = 3; 

    PMsensor PM;                        //미세먼지 센서 설정
    LiquidCrystal_I2C lcd(0x27, 16, 2); //LCD 설정

    DHT dht(DHTPIN, DHTTYPE);

    void setEmoticon(float data) {
      if (data > 100) {                 //미세먼지 값 > 100 일 때
        lcd.createChar(4, topAngry1);   //화난 이모티콘 출력
        lcd.createChar(5, topAngry2);
        lcd.createChar(6, bottomAngry1);
        lcd.createChar(7, bottomAngry2);
        digitalWrite(9, LOW);           //적색 LED 출력
        digitalWrite(10, LOW);
        digitalWrite(11, HIGH);
      }
      else if (data > 50) {             //미세먼지 값 > 50 일 때
        lcd.createChar(4, topSoSo1);    //정색 이모티콘 출력
        lcd.createChar(5, topSoSo2);
        lcd.createChar(6, bottomSoSo1);
        lcd.createChar(7, bottomSoSo2);
        digitalWrite(9, HIGH);          //청색 LED 출력
        digitalWrite(10, HIGH);
        digitalWrite(11, LOW);
      }

      else {                            //미세먼지 값 <= 50 일 때
        lcd.createChar(4, topSmile1);   //웃는 이모티콘 출력
        lcd.createChar(5, topSmile2);
        lcd.createChar(6, bottomSmile1);
        lcd.createChar(7, bottomSmile2);
        digitalWrite(9, LOW);           //녹색 LED 출력
        digitalWrite(10, HIGH);
        digitalWrite(11, LOW);
      }
    }

    void setup() {
      lcd.init();                       //LCD 준비
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
      lcd.setCursor(0, 0);                //LCD 출력
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
   ```
     
     
     
     
     
     
     
