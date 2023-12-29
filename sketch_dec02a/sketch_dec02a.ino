#include <SoftwareSerial.h>
int minDistance=0;
int maxDistance=2000;
SoftwareSerial mySerial(14,17); // RX, TX
unsigned char data[4]={};
int distance;

void setup()
{
 Serial.begin(115200);
 mySerial.begin(9600); 
}

void loop()
{
  percentage();
 maxDistance=distance;
 Serial.print("MAXDISTANCE");
 Serial.print(maxDistance);
}
void percentage()
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
           distance=distance/10;
           Serial.print(distance);
           Serial.println("cm");
            Serial.println("");
          
      }else Serial.println("ERROR");
     }
     delay(100);
}
