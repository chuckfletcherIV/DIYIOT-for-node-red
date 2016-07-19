#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const int numReadings = 5;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;
String lastReading;
// the average

// Update these with values suitable for your network.


const char* ssid     = "DIYIOT";
const char* password = "diyiotdiyiot";
const char* mqtt_server = "10.10.10.3";
const char* topic = "knob/1";

WiFiClient espClient;
PubSubClient client(espClient);
//long lastMsg = 0;
char msg[50];
int value = 0;
String data;

void setup() {
  pinMode(A0, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;
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

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
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
void loop() {
  data = String(analogRead(A0));
  
  //total = total - readings[readIndex];
  //readings[readIndex] = data.toInt();
  //total = total + readings[readIndex];
  //readIndex = readIndex + 1;
  //if (readIndex >= numReadings)
  //  readIndex = 0;
  //average = total / numReadings;
  
  //average = map(average, 0, 1023, 0, 360);
  
  //String myAverage = String(average);
  

  if (!client.connected()) {
    reconnect();
  }

    client.loop();
    //snprintf (msg, 75, data);
    //if (myAverage != lastReading) {
      data.toCharArray(msg,50);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish(topic, msg);
    }
    //lastReading = myAverage;
    //delay(250);
}


