//#define UIP_CONF_UDP             1
#include <UIPEthernet.h>
#include "PubSubClient.h"
#include "DHT.h"
#define CLIENT_ID       "UNO"
#define PUBLISH_DELAY   20000
#define DHTPIN        3
#define DHTTYPE     DHT11
bool statusD4 = HIGH;
bool statusD5 = HIGH;
bool statusD6 = HIGH;
bool statusD7 = HIGH;
bool statusD8 =HIGH;
bool statusD9 = HIGH;
bool statusD10 =LOW;
bool statusD11=LOW;
bool statusD12=LOW;
int valueA0;
int valueA1;
float valueA2; //made a float to store a  battery value
int valueA3;
int  upTime = 0;
byte i;
String ip = "192.168";
String MAC = "";
uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};


EthernetClient ethClient;
PubSubClient mqttClient;
DHT dht(DHTPIN, DHTTYPE);

long previousMillis;

void setup() {
  pinMode(4, INPUT_PULLUP);//
  pinMode(5, INPUT_PULLUP);//
  pinMode(6, INPUT_PULLUP);//
  pinMode(7, INPUT_PULLUP);//
  pinMode(8, INPUT_PULLUP);//
  pinMode(9, INPUT_PULLUP);//
  pinMode(10,INPUT_PULLUP);// 
  pinMode(11,INPUT_PULLUP);//
  pinMode(12,INPUT_PULLUP);//

  // setup serial communication
  //Serial.begin(9600);
  /*
    while(!Serial) {};
    Serial.println(F("MQTT Arduino Demo"));
    Serial.println();
  */
  // setup ethernet communication using DHCP
  if (Ethernet.begin(mac) == 0) {
    //Serial.println(F("Unable to configure Ethernet using DHCP"));
    for (;;);
  }

  // ip="192.168";
  for (i = 2; i < 4; i++) {
    ip = ip + ".";
    ip = ip + String (Ethernet.localIP()[i]);
  }
  
  //---mac
  for (i = 0; i < 6; i++) {
    if ((mac[i]) <= 0x0F) {
      MAC = MAC + "0"; //zet er zonodig een '0' voor
    }
    MAC = MAC + String ((mac[i]), HEX);
    MAC = MAC + ":";
  }
  MAC[(MAC.length()) - 1] = '\0'; //verwijder laatst toegevoegde ":"

  /*
    Serial.println(F("Ethernet configured via DHCP"));
    Serial.print("IP address: ");
    Serial.println(Ethernet.localIP());
    Serial.println();
  */
  // setup mqtt client
  mqttClient.setClient(ethClient);
  //  mqttClient.setServer( "raspberrypi.local",1883);

  mqttClient.setServer("192.168.1.103", 1883);
  //Serial.println(F("MQTT client configured"));

  // setup DHT sensor
  dht.begin();
  //Serial.println(F("DHT sensor initialized"));

  //Serial.println();
  // Serial.println(F("Ready to send data"));
  previousMillis = millis();
}

void loop() {
  statusD4 = digitalRead(4);//
  statusD5 = digitalRead(5);// 
  statusD6 = digitalRead(6);// 
  statusD7 = digitalRead(7);
  statusD8 = digitalRead(8);
  statusD9 = digitalRead(9);
  statusD10=digitalRead(10);
  statusD11=digitalRead(11);
  statusD12=digitalRead(12);
  upTime = millis() / 60000;
  valueA0 = analogRead(A0);//
  valueA1 = analogRead(A1); 
  valueA2 = analogRead(A2);
  valueA2 = (valueA2 * 5) / 1023.0;
   valueA3 = analogRead(A3); //garage
  // it's time to send new data?
  if (millis() - previousMillis > PUBLISH_DELAY) {
    sendData();
    previousMillis = millis();
  }

  mqttClient.loop();
}

void sendData() {

  char msgBuffer[7];//was 20
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (mqttClient.connect(CLIENT_ID)) {
    mqttClient.publish("home/temp", deblank(dtostrf(t, 4, 1, msgBuffer)));//
    mqttClient.publish("home/humid", deblank(dtostrf(h, 3, 0, msgBuffer)));
    mqttClient.publish("home/d4", (statusD4 == HIGH) ? "OPEN" : "CLOSED");//
    mqttClient.publish("home/d5", (statusD5 == HIGH) ? "OPEN" : "CLOSED");//
    mqttClient.publish("home/d6", (statusD6 == HIGH) ? "OPEN" : "CLOSED");//
    mqttClient.publish("home/d7", (statusD7 == HIGH) ? "OPEN" : "CLOSED");//
    mqttClient.publish("home/d8", (statusD8 == HIGH) ? "OPEN" : "CLOSED"); //
    mqttClient.publish("home/d9", (statusD9 == HIGH) ? "OPEN" : "CLOSED"); //
    mqttClient.publish("home/d10",(statusD10 == HIGH) ? "OPEN" : "CLOSED"); //
    mqttClient.publish("home/d11",(statusD11== HIGH) ? "OPEN" : "CLOSED"); //
    mqttClient.publish("home/d12",(statusD12== HIGH) ? "OPEN" : "CLOSED"); //
    mqttClient.publish("home/A0", deblank(dtostrf(valueA0, 4, 0, msgBuffer)));
    mqttClient.publish("home/A1", deblank(dtostrf(valueA1, 4, 0, msgBuffer)));
    mqttClient.publish("home/A2", deblank(dtostrf(valueA2, 4, 2, msgBuffer)));
    mqttClient.publish("home/A3", deblank(dtostrf(valueA1, 4, 0, msgBuffer)));
    mqttClient.publish("home/versie", "enc28j60MQTT_uni");
    mqttClient.publish("home/ip", ip.c_str());
    mqttClient.publish("home/u", deblank(dtostrf(upTime, 6, 0, msgBuffer)));//uptime
    mqttClient.publish("home/mac", MAC.c_str());
   

  }
}

char * deblank(char *str) {
  char *out = str;
  char *put = str;

  for (; *str != '\0'; ++str) {

    if (*str != ' ') {
      *put++ = *str;
    }
  }
  *put = '\0';
  return out;
}
