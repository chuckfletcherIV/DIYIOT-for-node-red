#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid     = "DIYIOT";
const char* password = "diyiotdiyiot";
const char* mqtt_server = "10.10.10.3";
const char* topic = "button/1";

int buttonpin = 13;

WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];

void setup() {
  //start serial connection
  Serial.begin(115200);

  pinMode(buttonpin, INPUT);
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
      client.publish("channels", topic);
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

void loop() {
    delay(250);

  if (!client.connected()) {
    reconnect();
  }

    client.loop();
    String myMessage = "button pressed";
    myMessage.toCharArray(msg,50);
    
    
  int sensorVal = digitalRead(buttonpin);
  //print out the value of the pushbutton
  //Serial.println(sensorVal);

  // Keep in mind the pullup means the pushbutton's
  // logic is inverted. It goes HIGH when it's open,
  // and LOW when it's pressed. Turn on pin 13 when the
  // button's pressed, and off when it's not:
  if (sensorVal == LOW) {
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(topic, msg);
  } else {
  }
}



