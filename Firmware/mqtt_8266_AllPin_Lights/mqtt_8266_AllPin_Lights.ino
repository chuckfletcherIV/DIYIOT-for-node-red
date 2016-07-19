#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid     = "DIYIOT";
const char* password = "diyiotdiyiot";
const char* mqtt_server = "10.10.10.3";
const char* topic = "lights/1";
char msg[50];
char message_buffer[100];


WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
Serial.begin(115200); 
setup_wifi();
pinMode(12, OUTPUT);
pinMode(13, OUTPUT);
pinMode(14, OUTPUT);
pinMode(16, OUTPUT);
pinMode(04, OUTPUT);
pinMode(05, OUTPUT); 
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
 int i = 0;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  payload[length] = '\0';
  String message = String((char*)payload);
  Serial.println("Received: " + String(message));
  
  int commaIndex = message.indexOf(',');
  int secondCommaIndex = message.indexOf(',', commaIndex+1);
  int pin = message.substring(0, commaIndex).toInt();
  String command = message.substring(commaIndex+1, secondCommaIndex);
  
  if (command == "on") {
    digitalWrite(pin, HIGH);
  } else if (command == "off") {
    digitalWrite(pin, LOW);
  }  
  
    if (command == "allon") {
    digitalWrite(12, HIGH);
        digitalWrite(13, HIGH);
            digitalWrite(14, HIGH);
                digitalWrite(16, HIGH);
                    digitalWrite(04, HIGH);
                        digitalWrite(05, HIGH);
  } else if (command == "alloff") {
        digitalWrite(12, LOW);
        digitalWrite(13, LOW);
            digitalWrite(14, LOW);
                digitalWrite(16, LOW);
                    digitalWrite(04, LOW);
                        digitalWrite(05, LOW);
    digitalWrite(pin, LOW);
  }  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    
      Serial.print("Attempting MQTT connection...");
      Serial.println(topic);
    // Attempt to connect
    if (client.connect(topic)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(topic, "connected");
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
}

   

