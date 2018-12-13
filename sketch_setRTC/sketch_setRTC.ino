#include <DS3231.h>
#include <Wire.h>

DS3231 Clock;
bool Century = false;
bool h12Hr;
bool PM;
byte ADay, AHour, AMinute, ASecond, ABits, prevSecond;
bool ADy, A12h, Apm;
int buff;
char buff2;
byte year, month, date, DoW, hour, minute, second;
int cnt;

void setup ()
{
  Serial.begin(9600);
  Wire.begin();
  prevSecond = 0;
  // Enable Alarms and Interupt
//  Clock.enableOscillator(true, true, 0);
}

void loop ()
{
  cnt = ++cnt;
  Clock.getTime(year, month, date, DoW, hour, minute, second);
  if (prevSecond != second) {
    Serial.print("Date and Time is ");
    Serial.print(date, DEC);
    Serial.print("/");
    Serial.print(month, DEC);
    Serial.print("/");
    Serial.print(year, DEC);
    Serial.print(" ");
    Serial.print(DoW, DEC);
    Serial.print(" ");
    digitalClockDisplay();
    Serial.print('\n');
    prevSecond = second;
    //Test if Alarm Fired
  }

  if (Serial.available() > 12 ) {
    buff = (Serial.read() - 48) * 10 + Serial.read() - 48;
    Clock.setYear(buff);
    buff = (Serial.read() - 48) * 10 + Serial.read() - 48;
    Clock.setMonth(buff);
    buff = (Serial.read() - 48) * 10 + Serial.read() - 48;
    Clock.setDate(buff);
    buff = Serial.read() - 48;
    Clock.setDoW(buff);
    buff = (Serial.read() - 48) * 10 + Serial.read() - 48;
    Clock.setHour(buff);
    buff = (Serial.read() - 48) * 10 + Serial.read() - 48;
    Clock.setMinute(buff);
    buff = (Serial.read() - 48) * 10 + Serial.read() - 48;
    Clock.setSecond(buff);

    Serial.println("timeset");
  }
  if (cnt > 500) {
    cnt = 0;
    while (Serial.available() != 0) {
      Serial.read();
    }
  }
}
void digitalClockDisplay() {
  // digital clock display of the time
  printDigits(hour);
  Serial.print(":");
  printDigits(minute);
  Serial.print(":");
  printDigits(second);
}

void printDigits(int digits) {
  // utility function for digital clock display: prints leading 0
  if (digits > 59)
    Serial.print("--");
  else {
    if (digits < 10)
      Serial.print('0');
    Serial.print(digits);
  }
}

