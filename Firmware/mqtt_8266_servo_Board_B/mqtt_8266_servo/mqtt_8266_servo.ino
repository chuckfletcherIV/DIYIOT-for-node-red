  #include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h> 
Servo myservo;

const char* ssid     = "DIYIOT";
const char* password = "diyiotdiyiot";
const char* mqtt_server = "10.10.10.3";
const char* topic = "servo/1";
char msg[50];
char message_buffer[100];
String myMsg;

WiFiClient espClient;
PubSubClient client(espClient);
int servoId = 0;
int potVal = 0; // Potentiometer value
int ledPosition; // Position of active LED
int lastpot = 0;  // last potentiometer reading to compare with

void setup()
{

  Serial.begin(115200);  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  myservo.attach(13);
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
 int i = 0;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  payload[length] = '\0';
  String potValString = String((char*)payload);
  potVal = potValString.toInt();
  Serial.println("Received: " + String(potVal));
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

void loop()
{ 
  
    if (!client.connected()) {
    reconnect();
  }

        client.loop();        
        int myPos = map(potVal, 0, 360, -180, 180);
        myservo.write(myPos);   
}


