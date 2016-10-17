#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <AccelStepper.h>
#define HALFSTEP 8

// Motor pin definitions
#define motorPin1  13     // IN1 on the ULN2003 driver 1
#define motorPin2  12     // IN2 on the ULN2003 driver 1
#define motorPin3  14     // IN3 on the ULN2003 driver 1
#define motorPin4  16     // IN4 on the ULN2003 driver 1

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

// Update these with values suitable for your network.

const char* ssid     = "DIYIOT";
const char* password = "diyiotdiyiot";
const char* mqtt_server = "10.10.10.3";
const char* topic = "mini_stepper/1";

WiFiClient espClient;
PubSubClient client(espClient);

char msg[50];
String data;

//////////////////////////////////////////////////////////////////////////////
void setup() {
  stepper.setMaxSpeed(1000.0);
  stepper.setAcceleration(500.0);
  stepper.setSpeed(500);
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
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    payload[length] = '\0';
     String message = String ((char*)payload);
    int degrees = message.toInt();   
    int steps = (int) (degrees * 5.6); 
    stepper.moveTo(steps);
    }
     
  }

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    
      Serial.print("Attempting MQTT connection...");
    // Attempt to connect
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

//////////////////////////////////////////////////////////////////////////////
void loop(){
    
  client.loop();    

  if (!client.connected()) {
    reconnect();
  } 
  yield();
 
  stepper.run();
  
}


