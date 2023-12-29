String PId;
extern String PId;
String topicStr1;
const char* topic1;
String topicStr2;
const char* topic2;
String topicStr3;
const char* topic3;
String topicStr4;
const char* topic4;
String topicStr5;
const char* topic5;
String topicStr6;
const char* topic6;
String topicStr7;
const char* topic7;
String topicStr8;
const char* topic8;
String topicStr9;
const char* topic9;
String topicStr10;
const char* topic10;
String topicStr11;
const char* topic11;
String mqttClientId1;
const char* mqttClientId;

void mqtt_topics()
{
  mqttClientId1 = PId;
  mqttClientId = mqttClientId1.c_str();  // host pid
  topicStr1 = "onwords/" + PId + "/status";
  topic1 = topicStr1.c_str();
  topicStr2 = "onwords/" + PId + "/getCurrentStatus";
  topic2 = topicStr2.c_str();
  topicStr3 = "onwords/" + PId + "/currentStatus";
  topic3 = topicStr3.c_str();
  topicStr4 = "onwords/hardReset";
  topic4 = topicStr4.c_str();
  topicStr5 = "onwords/" + PId + "/softReset";
  topic5 = topicStr5.c_str();
  topicStr6 = "onwords/" + PId + "/firmware";
  topic6 = topicStr6.c_str();
  topicStr7 = "onwords/" + PId + "/readings";
  topic7 = topicStr7.c_str();
  topicStr8 = "onwords/" + PId + "/distance";
  topic8 = topicStr8.c_str();
  topicStr9 = "onwords/" + PId + "/calibration";
  topic9 = topicStr9.c_str();
  topicStr10 = "onwords/" + PId + "/errormessage";
  topic10 = topicStr10.c_str();
  topicStr11 = "onwords/" + PId + "/notification";
  topic11 = topicStr11.c_str();
}
