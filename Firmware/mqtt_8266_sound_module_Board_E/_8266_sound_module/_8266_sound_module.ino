#include <ESP8266WiFi.h>
#include <PubSubClient.h>


const char* ssid     = "DIYIOT";
const char* password = "diyiotdiyiot";
const char* mqtt_server = "10.10.10.3";
const char* topic = "sound/2";
int soundDetectedPin = 13; // Use Pin 10 as our Input
int soundDetectedVal = HIGH; // This is where we record our Sound Measurement
boolean bAlarm = false;
unsigned long lastSoundDetectTime; // Record the time that we measured a sound
int soundAlarmTime = 500; // Number of milli seconds to keep the sound alarm high


WiFiClient espClient;
PubSubClient client(espClient);

/////////////////////////////
//SETUP
void setup(){
  Serial.begin(115200);
  pinMode (soundDetectedPin, INPUT) ; // input from the Sound Detection Module
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

    
       soundDetectedVal = digitalRead (soundDetectedPin) ; // read the sound alarm time
  
      if (soundDetectedVal == LOW) // If we hear a sound
      {
      
        lastSoundDetectTime = millis(); // record the time of the sound alarm
        // The following is so you don't scroll on the output screen
        if (!bAlarm){
          //Serial.println("Sound Detected");
          client.publish(topic, "sound");
          bAlarm = true;
        }
      }
      else
      {
        if( (millis()-lastSoundDetectTime) > soundAlarmTime  &&  bAlarm){
          //Serial.println("No Sound Detected");
          client.publish(topic, "no sound");
          bAlarm = false;
        }
      }
  }
