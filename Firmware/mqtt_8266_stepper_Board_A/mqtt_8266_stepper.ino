#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//declare variables for the motor pins
int motorPin1 = 12;    // Blue   - 28BYJ48 pin 1
int motorPin2 = 13;    // Pink   - 28BYJ48 pin 2
int motorPin3 = 14;    // Yellow - 28BYJ48 pin 3
int motorPin4 = 16;    // Orange - 28BYJ48 pin 4
                        // Red    - 28BYJ48 pin 5 (VCC)

int motorSpeed = 1200;  //variable to set stepper speed
int count = 0;          // count of steps made
int countsperrev = 512; // number of steps per full revolution
int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};


// Update these with values suitable for your network.

const char* ssid     = "DIYIOT";
const char* password = "diyiotdiyiot";
const char* mqtt_server = "10.10.10.3";
const char* topic = "stepper/1";

WiFiClient espClient;
PubSubClient client(espClient);

char msg[50];
String data;

//////////////////////////////////////////////////////////////////////////////
void setup() {
  //declare the motor pins as outputs
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
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
    // turn payload into message and react to it. If turnClockwise or if turnAnticlockwise or number of steps ( ie clockwise,#steps) or (anticlockwise,#steps), or stop
    payload[length] = '\0';
    String messageIn = String((char*)payload);
    if (messageIn == "turnClockwise") {
          turnClockwise();
    } else if (messageIn == "turnAnticlockwise")
    {
      turnAnticlockwise();
    } else if (messageIn == "stop") {
      turnStop();
    }
    
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

//////////////////////////////////////////////////////////////////////////////
void loop(){
  
  
  client.loop();    

  if (!client.connected()) {
    reconnect();
  }  
  
}

//////////////////////////////////////////////////////////////////////////////
//set pins to ULN2003 high in sequence from 1 to 4
//delay "motorSpeed" between each pin setting (to determine speed)
void turnAntiClockwise()
{
  for(int i = 0; i < 8; i++)
  {
    setOutput(i);
    delayMicroseconds(motorSpeed);
  }
}

void turnClockwise()
{
  for(int i = 7; i >= 0; i--)
  {
    setOutput(i);
    delayMicroseconds(motorSpeed);
  }
}

void turnStop() 
{
  //figure out how to stop......
}

void setOutput(int out)
{
  digitalWrite(motorPin1, bitRead(lookup[out], 0));
  digitalWrite(motorPin2, bitRead(lookup[out], 1));
  digitalWrite(motorPin3, bitRead(lookup[out], 2));
  digitalWrite(motorPin4, bitRead(lookup[out], 3));
}
