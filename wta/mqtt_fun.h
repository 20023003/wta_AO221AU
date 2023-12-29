
char softreset_status[200];
char calibration_us[200];
char ssid[32];
char password[64];
char reading_us[200];


const char* mqttServer = "mqtt.onwords.in";// mqttt
const int mqttPort = 1883;
const char* mqttUsername = "Nikhil";
const char* mqttPassword = "Nikhil8182";


char hardware_current_Status[300];
char hardware_productid[100];

WiFiServer server(8182);
WiFiClient espClient;
PubSubClient client(espClient);

void paringMode()
{
  WiFi.mode(WIFI_AP);
  String hotspot = "onwords-" + PId; //hotsopt name with pid
  const char* hotspot_name = hotspot.c_str();// hotspot creation
  WiFi.softAP(hotspot_name, "");  // assigning name to hotspot
  server.begin();                 //starting http server
  WiFi.softAPIP();
  Serial.println(WiFi.softAPIP());//print webserver local ip by default 192.168.4.1
  delay(2000);                    //this is mandatory delay
}
void currentstatus(int relay1)
{
  StaticJsonDocument<300> doc;
  doc["device1"] = relay1;
  serializeJson(doc, hardware_current_Status);
  client.publish(topic3, hardware_current_Status, false);
}
void callback(char* topic, byte * payload, unsigned int length) {
  Serial.print(topic);
  String aa = "";
  for (int i = 0; i < length; i++) {
    aa += (char)payload[i];
  }
  Serial.println(aa);
  DynamicJsonDocument doc(6000);
  DeserializationError error = deserializeJson(doc, aa); // Deserialize the message into the JSON document
  if (error) {
    return;
  }
  int firmware_sts = doc["firmware_sts"];
  if (firmware_sts == 1)
  {
    String firmwareUrl = doc["firmwareUrl"];
    frimware_update(firmwareUrl);
  }
  if (doc.containsKey("device1")) {
    device1 = doc["device1"];
    digitalWrite(R1, device1);
    currentstatus(device1);
  }
  currentstatus(device1);
  String response = doc["request"];
  if (response == "getCurrentStatus")
  {
    DynamicJsonDocument docc(500);
    docc["device1"] = device1;
    serializeJson(docc, hardware_current_Status);
    client.publish(topic3, hardware_current_Status, false);
  }
  int soft_resets = doc["reset"];
  if (soft_resets == 1)
  {
    for (int i = 0 ; i < 60  ; i++)
    {
      EEPROM.write(i, 0);
    }
    EEPROM.end();
    delay(100);
    StaticJsonDocument <500>soft;
    soft["status"] = true;
    serializeJson(soft, softreset_status);
    client.publish(topic5, softreset_status, publishQoS);
    delay(500);
    ESP.restart();
  }
}
