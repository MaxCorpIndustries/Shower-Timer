#include <LiquidCrystal_I2C.h>

int sensorPin = A0; 
int sensorValue = 0;

const int buzzer = 8; 

const int buttonPin = 2;
int buttonState = 0; 

int state =0;
float timer =0.0;  
float divisor = 0.001;
int actualtime =0;
int halfway =0;
int quarterway =0;
int timerstat=0;

int onetime=0;
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() 
{
    Serial.begin(9600);
    pinMode(buzzer, OUTPUT);

    pinMode(buttonPin, INPUT);
    lcd.init();                      // initialize the lcd 
    lcd.init();
    lcd.backlight();
    lcd.begin(16,2);

    lcd.print("===Boot===");

        
    //startup noise
    tone(buzzer, 440);
    delay(1000);
    noTone(buzzer);
    delay(1000);
    tone(buzzer, 365);
    delay(1000);
    noTone(buzzer);
    
}

void loop() 
{ 
  if (state == 0)
  {  
    sensorValue = analogRead(sensorPin);    
    lcd.clear();
    lcd.print("Time to wait:");
    state =1;
    
  }
  if (state == 1)
  {
    lcd.setCursor(0, 1);
    sensorValue = analogRead(sensorPin);
    timer = (sensorValue* divisor)*10;
    timer = constrain(timer,.5,10);
    lcd.print("Minutes:" + String(timer));

    //begin reading button
    buttonState = digitalRead(buttonPin);
    
    if (buttonState == HIGH) 
    {
      state=2;
      lcd.clear();
      lcd.print("Countdown Start");
      halfway = (timer* 60)*.5;
      quarterway = (timer* 60)*.75;
    }
  }
  if (state == 2)
  {
      
      lcd.setCursor(0, 1);
      
      actualtime = actualtime +1;
      
      lcd.print("S: "+ String(timer) + " D:"+String(actualtime));
      Serial.println(timerstat);
      delay(1000);

      if (actualtime >= (timer*60))
      {
          if (timerstat == 2)
          {
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print("TIME ELAPSED");
            for(int i =0; i < 20; i++)
            {
              tone(buzzer, 440);
              delay(250);
              noTone(buzzer);
              delay(250);
              tone(buzzer, 365);
              delay(250);
              noTone(buzzer);
              if (buttonState == HIGH) 
              {
                state=0;
                lcd.clear();
                actualtime=0;
                break;
              }              
            }          
            state=3;
            lcd.clear();
            timerstat =3;
          }
      }
      
      if  (actualtime >= halfway)
      {
        if (timerstat ==0)
        {
          tone(buzzer, 490);
          delay(250);
          noTone(buzzer);
          delay(250);
          tone(buzzer, 380);
          delay(250);
          noTone(buzzer);
          timerstat =1;
        }
      }
                    
      if  (actualtime >= quarterway)
      {
        if (timerstat ==1)
        {
          tone(buzzer, 320);
          delay(250);
          noTone(buzzer);
          delay(250);
          tone(buzzer, 400);
          delay(250);
          noTone(buzzer);
          timerstat =2;
        }
      }          
      
   }
   if (state == 3)
   {
      if (onetime ==0)
      {
        lcd.setCursor(0, 1);
        lcd.print("Press Start");
        lcd.setCursor(0, 2);
        lcd.print("To Reset.");
        onetime=1;
      }
      if (buttonState == HIGH) 
      {
        lcd.clear();
        state=0;
        actualtime=0;
      }
   }
}
