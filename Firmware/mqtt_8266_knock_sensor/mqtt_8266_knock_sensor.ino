#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid     = "DIYIOT";
const char* password = "diyiotdiyiot";
const char* mqtt_server = "10.10.10.3";
const char* topic = "knock/1";
unsigned long lastKnockTime; // Record the time that we measured a sound

int knockPin = 13; // Use Pin 10 as our Input
int knockVal = HIGH; // This is where we record our shock measurement
boolean bAlarm = false;

int knockAlarmTime = 500; // Number of milli seconds to keep the knock alarm high

WiFiClient espClient;
PubSubClient client(espClient);

/////////////////////////////
//SETUP
void setup(){
  Serial.begin(115200);
  pinMode (knockPin, INPUT) ; // input from the Sound Detection Module
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
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    
      Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(topic)) {
      Serial.println("Connected");
      // Once connected, publish an announcement...
      client.publish("channels", topic);
      client.publish(topic, "connected");
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

////////////////////////////
//LOOP
void loop(){
  
      if (!client.connected()) {
      reconnect();
      }

      client.loop();   

    
       knockVal = digitalRead (knockPin) ; // read the sound alarm time
  
      if (knockVal == LOW) // If we hear a sound
      {
      
        lastKnockTime = millis(); // record the time of the sound alarm
        // The following is so you don't scroll on the output screen
        if (!bAlarm){
          //Serial.println("Knock Detected");
          client.publish(topic, "knock");
          bAlarm = true;
        }
      }
      else
      {
        if( (millis()-lastKnockTime) >knockAlarmTime  &&  bAlarm){
          //Serial.println("No knock Detected");
          client.publish(topic, "no knock");
          bAlarm = false;
        }
      }
  }
