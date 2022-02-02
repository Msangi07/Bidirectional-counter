#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

SoftwareSerial gsm(10, 11);
LiquidCrystal lcd(22, 23, 24, 25, 26, 27);

String number1 = "+254779780971"; // +254 is the country code
String number2 = "+2547"; // +254 is the country code
String SMS1 = "A public transport matatu in Nakuru-Ravine road with plate number KDF 3012 has carried more than requred number of passengers(14)";
String SMS2 = "Stop a public transport matatu with plate number KDF 3012 for breaking traffic act 58 that prohibits Overloading";

#define sensor1 A0
#define sensor2 A1

int count = 0;
int countState = 0;
int State = 0;
int smsState = 0;

int sensorState1 = 0;
int sensorState2 = 0;

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  gsm.begin(9600);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
}

void loop() {
  lcdDisplay();
  if (smsState == 1)
  {
    sendMessage(number1, SMS1);
    delay(3000);
    sendMessage(number2, SMS2);
    smsState = 0;
  }
  else
    sensorState1 = digitalRead(sensor1);
  sensorState2 = digitalRead(sensor2);
  if (State == 0 && sensorState1 == 0)
  {
    State = 1;
  }
  else if (State == 1 && sensorState2 == 0)
  {
    count = count + 1;
    delay(500);
    State = 0;
    if (count >= 14)
    {
      count = 14;
      smsState = 1;
    }
  }

  else if (State == 0 && sensorState2 == 0)
  {
    State = 2;
  }
  else if (State == 2 && sensorState1 == 0)
  {
    count = count - 1;
    delay(500);
    State = 0;
    if (count < 0) count = 0;
  }

}

void lcdDisplay()
{
  lcd.setCursor(0, 0);
  lcd.print("Passengers in the car are");
  lcd.setCursor(0, 1);
  lcd.print(count);
  lcd.print("  ");
}



void sendMessage(String number, String SMS)
{
  gsm.println("AT+CMGF=1");
  delay(1000);
  gsm.println("AT+CMGS=\"" + number + "\"\r");
  delay(1000);
  gsm.println(SMS);
  delay(100);
  gsm.println((char)26);
  delay(1000);
}
