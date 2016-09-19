#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid     = "DIYIOT";
const char* password = "diyiotdiyiot";
const char* mqtt_server = "10.10.10.3";
const char* topic = "keypad/1";
char msg[50];
char message_buffer[100];

#include "Keypad.h"

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {13, 12, 14, 16}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2, 4, 5}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

WiFiClient espClient;
PubSubClient client(espClient);

void setup(){
  Serial.begin(115200); 
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
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
// int i = 0;
//  Serial.print("Message arrived [");
//  Serial.print(topic);
//  Serial.print("] ");
//  payload[length] = '\0';
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    
      Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(topic)) {
      Serial.println("Connected");
      // Once connected, publish an announcement...
      client.publish(topic, "connected");
      client.publish("channels", topic);
      // ... and resubscribe
      //client.subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
} 

void loop(){
  
      if (!client.connected()) {
      reconnect();
      }

      client.loop();   
  
  String myKey =  (String) keypad.getKey();
  myKey.toCharArray(msg,50);
  yield();
  if (myKey != ""){
    Serial.println(msg);
    client.publish(topic, msg);
  }
}
