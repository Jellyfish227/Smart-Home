#include <ir_Lego_PF_BitStreamEncoder.h>
#include <IRremote.h>

#include <DFRobot_RGBLCD.h> //LCD

#include <Adafruit_Sensor.h>


#include <SPI.h>

#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>

#include <DHT.h>
#include <DHT_U.h>

bool fanon = false; //false=off
bool airon = false; //true=on
bool lion = false;
bool dooron = false; //false = door close 
bool athome = true; //true = in house   in house and haven't turn on anything
int air_con = 0;
int fan_con = 0;
int light_con = 0;
int air_re = 3000;
int fan_re = 3000;
int light_re = 3000;
int  airtemp = 25;
int colorR = 255; //LCD
int colorG = 0; //LCD
int colorB = 0; //LCD
int screen_sta = 1;
int screen_re = 500;
float temp;
float hum;
char input;
IRsend irsend;
#define dht_dpin 50
#define DHTTYPE DHT22
DHT dht(dht_dpin, DHTTYPE);
DFRobot_RGBLCD lcd(16,2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(30,INPUT);  //light sensor
  pinMode(40,OUTPUT); //light
  pinMode(41,OUTPUT); //fan
  pinMode(53,INPUT);  //magnet
  dht.begin();
  lcd.init();

  colorR = 0;
  colorG = 0;
  colorB = 255;

  lcd.setRGB(colorR, colorG, colorB);
}

void loop() {
  // put your main code here, to run repeatedly:
  //sys main trigger (door)
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  int magstat = digitalRead(53);
  if ((magstat == LOW) && (!dooron)) //haven't turn on door
   {
    athome = !athome;
    dooron = !dooron;
    delay(15);
   }

  if ((magstat == HIGH) && (dooron)) //have turn on door
   {
    dooron = !dooron;
    delay(15);
   }


 
  


  //main
  if (athome)
   {
    Serial.print("okkkkk");
    if (air_con == 0) //sys_dominant (air con)
     {
      if ((temp<=25)&&(hum<=85)&&(airon))  //have turn on aircon
       {
        irsend.sendNEC(0xFF629D,32); //turn off air con
        airon = !(airon);
        delay(15);
       }


      if ((hum>=90)&&(temp<=25)&&(!airon)) //haven't turn on fan
       {
        irsend.sendNEC(0xFF629D,32); //turn on air con
        airon = !(airon);
        delay(15);
       }


      if ((temp>=27)&&(hum>=90)&&(!airon))
       {
        irsend.sendNEC(0xFF629D,32); //turn on air con
        airon = !(airon);
        delay(15);
       }
     

      if ((temp>=27)&&(hum<=85)&&(!airon))
       {
        irsend.sendNEC(0xFF629D,32); //turn on air con
        airon = !(airon);
        delay(15);
       }
     }








    if (fan_con == 0) //sys dominant (fan)
     {
      if ((temp>=27)&&(!fanon)) 
       {
        digitalWrite(41,HIGH); //fan on
        fanon = !(fanon);
        delay(150);
       }


      if ((round(temp) == 26)&&(airon)&&(fanon))  //have turn on aircon,temp down
       {
        digitalWrite(41,LOW);  //fan off
        fanon = !(fanon);
        delay(15);
       }
     }




   
    



    if (light_con == 0)
     {
      int lidata = digitalRead(30);
      if ((lidata == HIGH)&&(!lion)) //at nigth,haven't turn on the light forget what high mean
       {
        digitalWrite(40,HIGH); //idfk high is what
        lion = !(lion);        //then you don't fking care  --Jellyfish
        delay(15);
       }

      if ((lidata == LOW)&&(lion)) //daytime,have turn on the light forget what high mean
       {
        digitalWrite(40,LOW); //idfk high is what
        lion = !(lion);       //what you would like to said should be low right?  --Jellyfish
        delay(15);
       }
     }



//part of user dominant control
//status = 1 == user dominant
//status = 0 == sys dominant


    if (Serial.available()>0)
     {
      input = Serial.read();
      Serial.println(input);

      if (input == 'a')
       {
        Serial.println("air"); //on air
        if (!airon)
        {
         irsend.sendNEC(0xFF629D,32);
         airon = !(airon);
         if (air_con == 0)
         {
          air_con = 1;
         }
         else
         {
          air_con = 0;
         }
        }
       }


      if (input == 'x')
       {
        Serial.println("nair"); //off air
        if (airon)
        {
         irsend.sendNEC(0xFF629D,32);
         airon = !(airon);
         if (air_con == 0)
         {
          air_con = 1;
         }
         else
         {
          air_con = 0;
         }
        }
       }



      if (input == 'f')
       {
        Serial.println("fan"); //on fan
        if (!fanon)
        {
         digitalWrite(41,HIGH);
         fanon = !(fanon);
         if (fan_con == 0)
         {
          fan_con = 1;
         }
         else
         {
          fan_con = 0;
         }
        }
       }



      if (input == 'y')
       {
        Serial.println("nfan"); //off fan
        if (fanon)
        {
         digitalWrite(41,LOW);
         fanon = !(fanon);
         if (fan_con == 0)
         {
          fan_con = 1;
         }
         else
         {
          fan_con = 0;
         }
        }
       }


       
      if (input == 'l')
       {
        Serial.println("light"); //on light
        if (!lion)
        {
         digitalWrite(40,HIGH);
         lion = !(lion);
         if (light_con == 0)
         {
          light_con = 1;
         }
         else
         {
          light_con = 0;
         }
        }
       } 


       if (input == 'z')
       {
        Serial.println("nlight"); //off light
        if (lion)
        {
         digitalWrite(40,LOW);
         lion = !(lion);
         if (light_con == 0)
         {
          light_con = 1;
         }
         else
         {
          light_con = 0;
         }
        }
       }

       if (input == 'p')
       {
        air_con = 0;
        fan_con = 0;
        light_con = 0;
       }
     }



    check(air_con,air_re);
    check(fan_con,fan_re);
    check(light_con,light_re);

    lcd.clear();
    if (screen_sta == 1)
    {
     lcd.setCursor(0,0);
     lcd.print("Sys. Dominant:");
     if (air_con == 0)
     {
      lcd.setCursor(0,1);
      lcd.print("AirCon");
     }

     if (fan_con == 0)
     {
      lcd.setCursor(7,1);
      lcd.print("Fan");
     }

     if (light_con == 0)
     {
      lcd.setCursor(11,1);
      lcd.print("Light");
     }
    }

    if (screen_sta == 2)
    {
     lcd.setCursor(0,0);
     lcd.print("User Dominant:");
     if (air_con == 1)
     {
      lcd.setCursor(0,1);
      lcd.print("AirCon");
     }

     if (fan_con == 1)
     {
      lcd.setCursor(7,1);
      lcd.print("Fan");
     }

     if (light_con == 1)
     {
      lcd.setCursor(11,1);
      lcd.print("Light");
     }
    }

    if (screen_sta == 3)
    {
     lcd.setCursor(0,0);
     lcd.print("Temp:");
     lcd.setCursor(5,0);
     lcd.print(temp);
     lcd.setCursor(10,0);
     lcd.print((char)223);
     lcd.setCursor(11,0);
     lcd.print("C");
     lcd.setCursor(0,1);
     lcd.print("Hum:");
     lcd.setCursor(4,1);
     lcd.print(hum);
     lcd.setCursor(9,1);
     lcd.print("%");
    }

    screen_re -= 50 ;
    if (screen_re == 0)
    {
     screen_sta = (screen_sta % 3) + 1 ;
     screen_re = 500;
    }

    //RGBround(colorR, colorG, colorB);
    if ((colorG == 0)&&(colorG != 255))
  {
    colorR += 5; 
    colorB -= 5;
  } 
  if ((colorB == 0)&&(colorG != 255))
  {
    colorG += 5;
    colorR -= 5;
    }
  if ((colorR == 0)&&(colorB != 255))
  {
    colorB += 5;
    colorG -= 5;    
    }
    lcd.setRGB(colorR, colorG, colorB);
    
    delay(250);
   }
  
  
  
  
  else
   {
    Serial.println("not at home");
    if (airon)
     {
      irsend.sendNEC(0xFF629D,32);
      airon = !(airon);
      delay(15);
     }

    if (fanon)
     {
      digitalWrite(41,LOW); //of fan
      fanon = !(fanon);
      delay(15);
     }

    if (lion)
     {
      digitalWrite(40,LOW); //idfk high is what
      lion = !(lion);       //fk you this is off light!!!  --Jellyfish
      delay(15);
     }

    air_con = 0;
    fan_con = 0;
    light_con = 0;
    air_re = 3000;
    fan_re = 3000;
    light_re = 3000;
    lcd.clear();
    lcd.setRGB(0,0,0);
    delay(250);
   }
 
  
  
  
  Serial.println(colorR);
  Serial.println(colorG);
  Serial.println(colorB);
  
  
  delay(250); 
}

void check(int x,int y)
{
 if (x == 1)
  {
    y -= 30; 
  }
 if (y == 0)
  {
    x = 0;
  }
 if (x == 0)
  {
    y = 3000;
  }
}

void RGBround(int r, int g, int b) //inport f(x) variable
{
  if ((g == 0)&&(r != 255))
  {
    r += 1; 
    b -= 1;
  } 
  if ((b == 0)&&(g != 255))
  {
    g += 1;
    r -= 1;
    }
  if ((r == 0)&&(b != 255))
  {
    b += 1;
    g -= 1;    
    }
}
