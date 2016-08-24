#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h> 
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  0 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  4096 // this is the 'maximum' pulse length count (out of 4096)

uint8_t servonum = 0;

const char* ssid     = "DIYIOT";
const char* password = "diyiotdiyiot";
const char* mqtt_server = "10.10.10.3";
const char* topic = "servo/1";
char msg[50];
char message_buffer[100];
String myMsg;

WiFiClient espClient;
PubSubClient client(espClient);

int potVal = 0; // Potentiometer value
int ledPosition; // Position of active LED
int lastpot = 0;  // last potentiometer reading to compare with

void setup()
{
  Serial.begin(115200);  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  yield();
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  payload[length] = '\0';
  String message = String((char*)payload);
  int commaIndex = message.indexOf(',');
  int secondCommaIndex = message.indexOf(',', commaIndex+1);
  int servonum = message.substring(0, commaIndex).toInt();
  int = potValString.substring(commaIndex+1, secondCommaIndex);
  potVal = potValString.toInt();
  Serial.print("Received: " + String(potVal));
    Serial.println(": " + servonum);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    
    Serial.print("Attempting MQTT connection...");
    if (client.connect(topic)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(topic, "connected");
      // ... and resubscribe
      client.subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop()
{ 
    delay(250);
    if (!client.connected()) {
    reconnect();
    }
        client.loop(); 
        pulselen = map(potVal, 0, 360, SERVOMIN, SERVOMAX);
        pwm.setPWM(servonum, 0, pulselen);
}


