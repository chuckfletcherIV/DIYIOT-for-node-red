#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SharpIR.h>

#define ir A0
#define model 1080//20150
SharpIR SharpIR(ir, model);

void analogReference(uint8_t external) {}

// Update these with values suitable for your network.

const char* ssid     = "DIYIOT";
const char* password = "diyiotdiyiot";
const char* mqtt_server = "10.10.10.3";
const char* topic = "sharp_distance/1";

WiFiClient espClient;
PubSubClient client(espClient);

char msg[50];
String data;

void setup() {
  pinMode(A0, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
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
  }
  Serial.println();

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
void loop() {
  delay(500);
  
  
  if (!client.connected()) {
    reconnect();
  }

    client.loop();
    
    int dis=SharpIR.distance();  // this returns the distance to the object you're measuring
   
    data = String(dis);

    data.toCharArray(msg,50);
    
    Serial.print("Mean distance: ");  // returns it to the serial monitor
    Serial.println(msg);
    client.publish(topic, msg);
}
