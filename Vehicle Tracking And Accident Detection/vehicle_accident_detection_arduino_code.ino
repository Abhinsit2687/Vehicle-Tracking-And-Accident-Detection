#include<LiquidCrystal.h>
#include<TinyGPS.h>
LiquidCrystal lcd(4, 5, 6, 7, 8, 9);

const int relay_Pin = 2;
const int buzzer_Pin = 3;
const int ir_Sensor = 10;
const int alcohol_Sensor = 11;
const int vibration_Sensor = 12;

TinyGPS gps;
long lat,lon;
bool ir_status = LOW;
bool alcohol_Status = LOW;
bool vibration_Status = LOW;


void setup() {
  // put your setup code here, to run once:

  pinMode(relay_Pin, OUTPUT);
  pinMode(buzzer_Pin, OUTPUT);
  pinMode(ir_Sensor, INPUT);
  pinMode(alcohol_Sensor, INPUT);
  pinMode(vibration_Sensor, INPUT);
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.print("ACCIDENT DETECTION");
  lcd.setCursor(3,2);
  lcd.print("SYSTEM");

}

void loop() {
  // put your main code here, to run repeatedly:
  ir_status = digitalRead(ir_Sensor);
  delay(100);
  if(ir_status == HIGH)
  {
    digitalWrite(buzzer_Pin, LOW);
    delay(200);
    lcd.clear();
    lcd.print("Seat Belt");
    lcd.setCursor(3,2);
    lcd.print("Detected");
    delay(500);

    while(1)
    {
      alcohol_Status = digitalRead(alcohol_Sensor);
      delay(100);
      if(alcohol_Status == LOW)
      {
        digitalWrite(buzzer_Pin, LOW);
        delay(200);
        lcd.clear();
        lcd.print("Alcohol not");
        lcd.setCursor(3,2);
        lcd.print("Deteted");
        delay(500);
        digitalWrite(relay_Pin, HIGH);
        delay(200);
        while(1)
        {
          lcd.clear();
          lcd.print("Vehicle Started");
          delay(500);
          while(1)
          {
            vibration_Status = digitalRead(vibration_Sensor);
            delay(100);
            if(vibration_Status == HIGH)
            {
              lcd.clear();
              lcd.print("Accident Detected");
              lcd.setCursor(3,2);
              lcd.print("Sending Mag");
              delay(500);
              Serial.println("AT+CMGF=1");
              delay(100);
              Serial.println("AT+CMGS=\"+91 8178452806\"\r");
              delay(100);
              Serial.println("Accident Detected");
              Serial.println("please check location");
              while(1)
              {
                gps_read();
              }
            }
            else
            {
              // do nothing
            }
          }
        }
      }
      else
      {
        lcd.clear();
        lcd.print("Alcohol");
        lcd.setCursor(3,2);
        lcd.print("Detected");
        delay(500);
        digitalWrite(relay_Pin, LOW);
        delay(200);
        digitalWrite(buzzer_Pin, HIGH);
        delay(200);
      }
    }
  }

  else
  {
    lcd.clear();
    lcd.print("Seat Belt");
    lcd.setCursor(3,2);
    lcd.print("not connected");
    digitalWrite(relay_Pin, LOW);
    delay(200);
    digitalWrite(buzzer_Pin, HIGH);
    delay(200);
  }

}

void gps_read()
{
  byte a;

  if(Serial.available())
  {
    a=Serial.read();

    while(gps.encode(a))
    {
      gps.get_position(&lat,&lon);

      Serial.println("Position: ");
      Serial.print("lat:");
      Serial.println((lat*0.000001),8);
      Serial.print("log:");
      Serial.println((lon*0.000001),8);
    }
  }
}










