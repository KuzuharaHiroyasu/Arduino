#include <Wire.h>
#include <DS3231.h>

enum led_pattern {
  off_pattern,
  ready_pattern,
  error_pattern,
  end_pattern
};

DS3231 clock;
RTCDateTime dt;
const int LED = 13;
const int POWER = 22;
const int VIB = 6;
const int VIB2 = 5;

int incomingByte = 0;   // for incoming serial data
int interval = 250;
int i, k = 0;
bool power_on = false;
led_pattern pattern = ready_pattern;

void setup() {
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  pinMode(LED, OUTPUT);
  pinMode(POWER, OUTPUT);
  pinMode(VIB, OUTPUT);
  pinMode(VIB2, OUTPUT);
  digitalWrite(LED, LOW);
  digitalWrite(POWER, HIGH);
  digitalWrite(VIB, LOW);
  digitalWrite(VIB2, LOW);
  clock.begin();
}

void loop() {
  // send data only when you receive data:

  switch (pattern)
  {
    case off_pattern: //消灯
      break;
    case ready_pattern: //点滅
      digitalWrite(LED, HIGH);
      delay(interval);
      digitalWrite(LED, LOW);
      delay(interval);
      i++;
      if (i == 20 && !power_on) //10秒後に電源ON
      {
        digitalWrite(POWER, LOW);
        delay(2000);
        digitalWrite(POWER, HIGH);
        power_on = true;
        interval = 1000;
      }
      break;
    case error_pattern: //点灯
      break;
    case end_pattern: // ウィンドウズ終了
      for (int ii = 0; ii < 3; ii++)
      {
        digitalWrite(LED, HIGH);
        delay(interval);
        digitalWrite(LED, LOW);
        delay(interval);
      }
      delay(1000);
      break;
    default:
      break;
  }

  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    switch (incomingByte)
    {
      case 0: //準備完了
        digitalWrite(LED, LOW);
        pattern = off_pattern;
        break;
      case 1: //エラー
        digitalWrite(LED, HIGH);
        pattern = error_pattern;
        break;
      case 2: //時刻
        dt = clock.getDateTime();
        Serial.print("Y");
        delay(50);
        Serial.print(dt.year);
        delay(50);

        Serial.print("M");
        delay(50);
        Serial.print(dt.month);
        delay(50);

        Serial.print("D");
        delay(50);
        Serial.print(dt.day);
        delay(50);

        Serial.print("h");
        delay(50);
        Serial.print(dt.hour);
        delay(50);

        Serial.print("m");
        delay(50);
        Serial.print(dt.minute);
        delay(50);

        Serial.print("s");
        delay(50);
        Serial.print(dt.second);
        delay(50);

        Serial.print("END");
        break;
      case 3: //バイブ
        digitalWrite(VIB, HIGH);
        digitalWrite(VIB2, HIGH);

        delay(100);
        digitalWrite(VIB, LOW);
        delay(20);
        digitalWrite(VIB2, LOW);

        delay(80);

        digitalWrite(VIB, HIGH);
        digitalWrite(VIB2, HIGH);

        delay(100);
        digitalWrite(VIB, LOW);
        delay(20);
        digitalWrite(VIB2, LOW);

        delay(80);

        digitalWrite(VIB, HIGH);
        digitalWrite(VIB2, HIGH);

        delay(100);
        digitalWrite(VIB, LOW);
        delay(20);
        digitalWrite(VIB2, LOW);

        break;
      default:
        break;
    }
  }
  delay(10);
}

