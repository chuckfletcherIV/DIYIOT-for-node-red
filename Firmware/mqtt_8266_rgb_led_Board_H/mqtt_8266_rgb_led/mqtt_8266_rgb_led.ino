#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define COMMON_ANODE

const char* ssid     = "DIYIOT";
const char* password = "diyiotdiyiot";
const char* mqtt_server = "10.10.10.3";
const char* topic = "rgbled/1";
int redPin = 13;
int greenPin = 12;
int bluePin = 14;
int redValue;
int greenValue;
int blueValue;

char msg[50];
char message_buffer[100];

// Board Pin # to Chip Pin #
// Board 7 = Chip 5
// Board 6 = Chip 4
// Board 5 = Chip 2
// Board 4 = Chip 13
// Board 3 = Chip 12
// Board 2 = Chip 14
// Board 1 = Chip 16

WiFiClient espClient;
PubSubClient client(espClient);
 
//uncomment this line if using a Common Anode LED
//#define COMMON_ANODE
 

void setup(){
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  
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
    Serial.println("Received: " + String(message));
    int redValue = getValue(message, ',', 0).toInt();
    int greenValue = getValue(message, ',', 1).toInt();
    int blueValue = getValue(message, ',', 2).toInt();
    Serial.println("Red: " + String(redValue) + "Blue: " + String(blueValue) + "Green: " + String(greenValue));
    setColor(redValue, blueValue, greenValue);
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

void loop(){
  
      if (!client.connected()) {
      reconnect();
      }
  
      client.loop(); 
  }
  
  
void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

String getValue(String data, char separator, int index)
{
 int found = 0;
  int strIndex[] = {
0, -1  };
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
  if(data.charAt(i)==separator || i==maxIndex){
  found++;
  strIndex[0] = strIndex[1]+1;
  strIndex[1] = (i == maxIndex) ? i+1 : i;
  }
 }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
