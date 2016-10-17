#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define waterSensorPin A0

const char* ssid     = "DIYIOT";
const char* password = "diyiotdiyiot";
const char* mqtt_server = "10.10.10.3";
const char* topic = "water_sensor/1";

WiFiClient espClient;
PubSubClient client(espClient);

char msg[50];
String data;
String message;

void setup() {
  pinMode(waterSensorPin, INPUT);
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
  
  if (!client.connected()) {
    reconnect();
  }

    client.loop();
    
    int sensorValue=analogRead(waterSensorPin);  // this returns the distance to the object you're measuring
    
    if (sensorValue > 100) {
    message = "water detected";
    } else if (sensorValue < 100) {
    message = "no water detected"; 
    }

    message.toCharArray(msg,50);
    
    Serial.print("Water Sensor Reading: ");  // returns it to the serial monitor
    Serial.println(msg);
    client.publish(topic, msg);

}
