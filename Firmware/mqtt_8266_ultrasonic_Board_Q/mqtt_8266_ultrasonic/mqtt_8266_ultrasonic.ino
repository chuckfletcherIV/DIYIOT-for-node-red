#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define trigPin 12
#define echoPin 13

// Update these with values suitable for your network.

const char* ssid     = "DIYIOT";
const char* password = "diyiotdiyiot";
const char* mqtt_server = "10.10.10.3";
const char* topic = "ultrasonic/1";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
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
 
    long distance, duration;
  
    yield();

    digitalWrite(trigPin, LOW);  // Added this line

    delayMicroseconds(2); // Added this line

    digitalWrite(trigPin, HIGH);

    delayMicroseconds(10); // Added this line

    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);

    distance = (duration/2) / 29.1;
  
  if (!client.connected()) {
   reconnect();
  }
  client.loop();
    
    if (distance < 1000) {
    String data = String(distance);
    char msg[50];
    
    data.toCharArray(msg,50);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(topic, msg);
    delay(250);
    }
}
