

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <Update.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
int minDistance=0;
int maxDistance=0;
SoftwareSerial mySerial(14,27); // RX, TX
unsigned char data[4]={};
int distance=0;
int distance1=0;
#include"print.h"
#include"mqtt_topic.h"
#include"initial_function.h"
#include"firmware.h"
#include"mqtt_fun.h"
#include"eeprom_storage.h"
#include"offline_reconnect.h"

void setup()
{
 Serial.begin(115200);
 mySerial.begin(9600); 
  EEPROM.begin(EEPROM_SIZE);
  String ssid = readssid();
  String password = readpassword() ;
  p("ssid = " + ssid);
  p("password = " + password);
  PId = readPid();
  p("PId=" + PId);
  delay(1000);
  intializepins();
  if (ssid == "")
  {
    blinkled3time();
    paringMode();
  }
  else
  {
    WiFi.begin(ssid.c_str(), password.c_str());
    reconnecting_wifi();
    digitalWrite(led, HIGH);
    parinMode = false;
    delay(200);
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
    mqtt_topics();
  }
}
void loop()
{
  if (parinMode)
  {
    floatlogic();
    clients = server.available();
    if (!clients)
    {
      return;
    }
    count1 = 0;
    while (!clients.available()) {
      delay(1);
    }
    delay(2000);
    pair();
  }
  else
  {
    listen_for_hardreset();
  }
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  if (count1 == 0) {
    if (digitalRead(FLOAT_LOW) == LOW && count2 == 0) {
      do{
     for(int i=0;i<4;i++)
     {
       data[i]=mySerial.read();
     }
  }while(mySerial.read()==0xff);

  mySerial.flush();

  if(data[0]==0xff)
    {
      int sum;
      sum=(data[0]+data[1]+data[2])&0x00FF;
      if(sum==data[3])
      {
        distance=(data[1]<<8)+data[2];
       
           Serial.print("distance=");
           maxDistance=distance/10;
          
          
      }
     }
      EEPROM.put(addr1_max, maxDistance);
      EEPROM.commit();
      Serial.println("Maximum distance detected");
      Serial.println(maxDistance);
      count2 = 1;
    }
    if (digitalRead(FLOAT_HIGH) == LOW) {
     do{
     for(int i=0;i<4;i++)
     {
       data[i]=mySerial.read();
     }
  }while(mySerial.read()==0xff);

  mySerial.flush();

  if(data[0]==0xff)
    {
      int sum;
      sum=(data[0]+data[1]+data[2])&0x00FF;
      if(sum==data[3])
      {
        distance=(data[1]<<8)+data[2];
       
           Serial.print("distance=");
           minDistance=distance/10;
          
          
      }
     }
    
      EEPROM.put(addr2_min , minDistance);
      EEPROM.commit();
      digitalWrite(R1, LOW);
      currentstatus(0);
      Serial.println("Minimum distance detected");
      Serial.println(minDistance);
      count1 = 1;
    }
    StaticJsonDocument <2000>cals;
    cals["readings"] = "calibrating";
    serializeJson(cals, calibration_us);
    client.publish(topic7, calibration_us);
    client.publish(topic9, calibration_us);
  }


  do{
     for(int i=0;i<4;i++)
     {
       data[i]=mySerial.read();
     }
  }while(mySerial.read()==0xff);

  mySerial.flush();

  if(data[0]==0xff)
    {
      int sum;
      sum=(data[0]+data[1]+data[2])&0x00FF;
      if(sum==data[3])
      {
        distance=(data[1]<<8)+data[2];
       
           Serial.print("distance=");
           distance=distance/10;
          
          
      }
     }
  percentage = map(distance, minDistance, maxDistance, 100, 0);
  Serial.println(percentage);
  if (percentage >= 100)
  {
    percentage = 100;
  }
  if (percentage <= 0)
  {
    percentage = 0;
  }
  floatlogic();
  if (count == 5)
  {
    StaticJsonDocument <2000>value;
    value["readings"] = percentage;
    serializeJson(value, reading_us);
    client.publish(topic7, reading_us, true);
    count = 0;
  }
  count++;
  delay(100);
}
void ultrasonic()
{
   do{
     for(int i=0;i<4;i++)
     {
       data[i]=mySerial.read();
     }
  }while(mySerial.read()==0xff);

  mySerial.flush();

  if(data[0]==0xff)
    {
      int sum;
      sum=(data[0]+data[1]+data[2])&0x00FF;
      if(sum==data[3])
      {
        distance=(data[1]<<8)+data[2];
       
           Serial.print("distance=");
           distance1=distance/10;
          
          
      }
     }
     delay(100);
}
