#include <WiFi.h>
#include <HTTPClient.h>// library
#include <ArduinoJson.h>
#include <EEPROM.h> // inbuild memory
#include <PubSubClient.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(14,17); // RX, TX
unsigned char data[4]={};
int distance;
#include"function.h" /// function
#define FLOAT_HIGH 33
#define FLOAT_LOW 25 
#define SUMP 26
 int data1;
  int data2;
int percentage=0;
#define TRIGGER_PIN 14
#define ECHO_PIN 27
#define MAX_PERCENTAGE 100
int maxDistance = 0; // To store the maximum distance
int minDistance = 0; 
const int addr1 =100;
const int addr2 =200;

unsigned long previousMillis = 0;  // Variable to store the last time the LED was updated
const long interval = 5000;


int count = 0;
int count1 = 1;
int count2 = 0;
WiFiServer server(8182);
bool parinMode = true;

String result;
String store;
String action;

char network[200];
char ssid[32];
char password[64];
char hardwarests[200];
char softreset[200];
char readings[200];

char distances[200];


const char* mqttServer = "mqtt.onwords.in";// mqttt
const int mqttPort = 1883;
const char* mqttUsername = "Nikhil";
const char* mqttPassword = "Nikhil8182";



String hotspot = "onwords-" + PId; //hotsopt name with pid
const char* hotspot_cstr = hotspot.c_str();

String mqttClientId1 = PId;
const char* mqttClientId = mqttClientId1.c_str();  // host id.

String response;

void setup()
{
  
 Serial.begin(115200);
 mySerial.begin(9600); 
  pinMode(buttonPin, INPUT);
  pinMode(R1, OUTPUT); //PINMODE DECLARATION
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(led, OUTPUT);//led
  pinMode(FLOAT_HIGH, INPUT_PULLUP);
  pinMode(FLOAT_LOW, INPUT_PULLUP);
  pinMode(SUMP, INPUT_PULLUP);
 
  pinMode(onbutton, INPUT_PULLUP);
 pinMode(offbutton, INPUT_PULLUP);
  digitalWrite(R1, LOW);
  digitalWrite(led, LOW);
  digitalWrite(R2, LOW);
  digitalWrite(R3, LOW);
  EEPROM.begin(EEPROM_SIZE);
  Serial.println("read from eeprom");
  String ssid = handlereadssid();
  String password = handlereadpassword() ;
  Serial.println("ssid = " + ssid);
  Serial.println("password = " + password);

  if (ssid == "")
  {
    Serial.println("PARING MODE ON");
    delay(2000);
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(1000);
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(1000);
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(1000);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(hotspot_cstr, "");
    server.begin();
    Serial.println(WiFi.softAPIP());
    delay(2000);
  }
  else
  {
   
  EEPROM.get(addr1 ,data2);
  EEPROM.get(addr2 ,data1);
  minDistance=data1;
  maxDistance=data2;
    Serial.println("connecting to ssid id =" + ssid + " password =" + password);
    delay(20);
    WiFi.begin(ssid.c_str(), password.c_str());
    handle_reconnecting_wifi();
    Serial.println("connected to wifi");
    digitalWrite(led, HIGH);
    parinMode = false;
    delay(200);
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);

  }

}
void handle_reconnecting_wifi()
{
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    digitalWrite(led, HIGH);
    delay(200);
    digitalWrite(led, LOW);
    delay(200);
     if (digitalRead(FLOAT_LOW) == LOW&&digitalRead(FLOAT_HIGH) == LOW) {
    digitalWrite(R1, LOW);
     handlecurrentstatus(0);
   }  
 
    if (digitalRead(SUMP) == HIGH) {
    digitalWrite(R1, LOW);
   handlecurrentstatus(0);
   }
    if (digitalRead(FLOAT_HIGH) == LOW) {
    digitalWrite(R1, LOW);
     handlecurrentstatus(0);
   }
  
  
  if (digitalRead(FLOAT_LOW) == HIGH&&digitalRead(SUMP) == HIGH) {
      digitalWrite(R1, LOW);
       handlecurrentstatus(0);
  }
    if (digitalRead(FLOAT_LOW) == HIGH&&digitalRead(SUMP) == LOW)
     {
      digitalWrite(R1, HIGH);
       handlecurrentstatus(1);
      }
      manual();
    int buttonState = digitalRead(buttonPin);
    Serial.println(buttonState);
    if (buttonState == LOW) {
      if (!buttonPressed) {
        buttonPressStart = millis();
        buttonPressed = true;
      }
      if (millis() - buttonPressStart >= 5000) {

        for (int i = 0 ; i < 512  ; i++) {
          EEPROM.write(i, 0);
        }

        EEPROM.end();
        Serial.println("EEPROM Clear Done!");
        ESP.restart();
        delay(100);

      }
    }
  }
}

void loop() {

  if (parinMode)
  {

    WiFiClient clients;
    clients = server.available();
    if (!clients) { // WIFIMANAGER
      return;

    }
    count1=0;
    Serial.println("New client");
    while (!clients.available()) {

      delay(1);

    }
    delay(2000);
    String jsonPayload = clients.readString();
    Serial.println(String(jsonPayload));
    int bodyStart = jsonPayload.indexOf("\r\n\r\n") + 4;
    String body = jsonPayload.substring(bodyStart);
    Serial.println("Body: " + body);
    clients.flush();
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, body);
    const char* ssid = doc["ssid"].as<const char*>();
    const char* password = doc["password"].as<const char*>();
    Serial.println("SSID: " + String(ssid));
    Serial.println("Password: " + String(password));
    Serial.println("HTTP/1.1 200 OK");
    String x = "connecting to " + String(ssid);
    clients.println("Content-Type: text/plain");
    clients.println("");
    clients.println("ok");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
      digitalWrite(led, HIGH);
      delay(400);
      digitalWrite(led, LOW);
      delay(400);
    }
    result = ssid + String(':') + password;
    handlewritestring(result);
    digitalWrite(led, HIGH);
    Serial.println("Connected to WiFi");
    delay(1);
    Serial.println("Client disconnected");
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
    parinMode = false;
  }
  else
  {
    handlecleareeprom();
  }
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

 
   if (count1 == 0) {
    if (digitalRead(FLOAT_LOW) == LOW&&count2==0) {
      ULTRASONIC();
      maxDistance = distance; // Store maximum distanc
       EEPROM.put(addr1, maxDistance);
        EEPROM.commit(); 
      Serial.println("Maximum distance detected");
       Serial.println(maxDistance);
      count2=1;
         }
     if (digitalRead(FLOAT_HIGH) == LOW) {
      ULTRASONIC();
      minDistance =distance; // Store minimum distance
      EEPROM.put(addr2 , minDistance);
      EEPROM.commit(); 
         digitalWrite(R1, LOW);
         handlecurrentstatus(0);
      Serial.println("Minimum distance detected");
      Serial.println(minDistance);
      count1 = 1;
    }
    client.publish(topic7, "{\"readings\":\"calibrating\"}");
client.publish(topic9, "{\"readings\":\"calibrating\"}");

  }

  ULTRASONIC();
    unsigned int distance = distance;

    // Calculate the percentage of the distance between the minimum and maximum distances
     percentage = map(distance, minDistance, maxDistance, 100, 0);
     Serial.println(percentage);
    if(percentage>=100)
    {
      percentage=100;
    }
    
     if(percentage<=0)
    {
      percentage=0;
    }
  if (digitalRead(FLOAT_LOW) == LOW&&digitalRead(FLOAT_HIGH) == LOW) {
    digitalWrite(R1, LOW);
     handlecurrentstatus(0);
   }  
 
    if (digitalRead(SUMP) == HIGH) {
    digitalWrite(R1, LOW);
   handlecurrentstatus(0);
   }
    if (digitalRead(FLOAT_HIGH) == LOW) {
    digitalWrite(R1, LOW);
     handlecurrentstatus(0);
   }
  
  
  if (digitalRead(FLOAT_LOW) == HIGH&&digitalRead(SUMP) == HIGH) {
      digitalWrite(R1, LOW);
       handlecurrentstatus(0);
  }
    if (digitalRead(FLOAT_LOW) == HIGH&&digitalRead(SUMP) == LOW)
     {
      digitalWrite(R1, HIGH);
       handlecurrentstatus(1);
      }
 manual();
 
  if (count == 50) {
   
    StaticJsonDocument <2000>docsss;
  
    docsss["readings"] = percentage;
    serializeJson(docsss, readings);
    Serial.println(readings);
    client.publish(topic7, readings, true);
    count = 0;
   
  }
  count++;
delay(100);
  
}

void reconnect()
{
  while (!client.connected()) {
    String ssid = handlereadssid();
    String password = handlereadpassword() ;
    Serial.println("ssid = " + ssid);
    Serial.println("password = " + password);
    delay(200);
    WiFi.begin(ssid.c_str(), password.c_str());
    handle_reconnecting_wifi();
    Serial.println("Connected to WiFi");
    if (client.connect(mqttClientId, mqttUsername, mqttPassword)) {
      Serial.println("Connected to MQTT broker");
      client.subscribe(topic1, subscribeQoS);
      client.subscribe(topic2, subscribeQoS);
      client.subscribe(topic5, subscribeQoS);
      client.publish(topic3, currentStatus, false);

      client.publish(topic4, productid, publishQoS);
      client.publish(topic6, softreset, publishQoS);
      client.publish(topic7, readings, true);
      client.publish(topic8, distances, publishQoS);
      digitalWrite(led, HIGH);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      digitalWrite(led, LOW);
      delay(500);
       if (digitalRead(FLOAT_LOW) == LOW&&digitalRead(FLOAT_HIGH) == LOW) {
    digitalWrite(R1, LOW);
     handlecurrentstatus(0);
   }  
 
    if (digitalRead(SUMP) == HIGH) {
    digitalWrite(R1, LOW);
   handlecurrentstatus(0);
   }
    if (digitalRead(FLOAT_HIGH) == LOW) {
    digitalWrite(R1, LOW);
     handlecurrentstatus(0);
   }
  
  
  if (digitalRead(FLOAT_LOW) == HIGH&&digitalRead(SUMP) == HIGH) {
      digitalWrite(R1, LOW);
      handlecurrentstatus(0);
  }
    if (digitalRead(FLOAT_LOW) == HIGH&&digitalRead(SUMP) == LOW)
     {
      digitalWrite(R1, HIGH);
       handlecurrentstatus(1);
      }
      manual();
    }
  }
}

void callback(char* topic, byte * payload, unsigned int length) {
  Serial.print("Received message on topic: ");
  Serial.print(topic);
  Serial.print("Message: ");

  String aa = "";
  for (int i = 0; i < length; i++) {
    aa += (char)payload[i];
  }
  Serial.println(aa);
  DynamicJsonDocument doc(6000);
  DeserializationError error = deserializeJson(doc, aa); // Deserialize the message into the JSON document
  if (error) {
    Serial.print("Error deserializing JSON: ");
    Serial.println(error.c_str());
    return;
  }
 

  if (doc.containsKey("device1")) {
    device1 = doc["device1"];
    digitalWrite(R1, device1);
    handlecurrentstatus(device1);
  }
  String response = doc["request"];
  if (response == "getCurrentStatus")
  {
    StaticJsonDocument <2000>docs;
    Serial.println("insideeeee");
    docs["device1"] = device1;
    serializeJson(docs, currentStatus);
    Serial.println(currentStatus);
    client.publish(topic3, currentStatus, false);
  }
  int sftret = doc["reset"];
  if (sftret == 1)
  {
    for (int i = 0 ; i < 512  ; i++) {
      EEPROM.write(i, 0);
    }

    EEPROM.end();
    Serial.println("EEPROM Clear Done!");
    delay(100);
    StaticJsonDocument <2000>strs;
    strs["status"] = true;
    serializeJson(strs, softreset);
    Serial.println(softreset);
    client.publish(topic6, softreset, publishQoS);
    delay(500);
    ESP.restart();
  }

}
void ULTRASONIC()
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
       
         Serial.println("cm");
           distance=distance/10;
         
          
      }
     }
     
}
