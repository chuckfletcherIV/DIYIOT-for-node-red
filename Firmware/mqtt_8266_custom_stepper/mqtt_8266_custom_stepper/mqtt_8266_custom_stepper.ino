#include <CustomStepper.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

CustomStepper stepper(12, 13, 14, 16, (byte[]){8, B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001}, 4075.7728395, 12, CW);
boolean rotate1 = false;
boolean rotatedeg = false;
boolean crotate = false;

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
  //sets the RPM
  stepper.setRPM(12);
  //sets the Steps Per Rotation, in this case it is 64 * the 283712/4455 annoying ger ratio
  //for my motor (it works with float to be able to deal with these non-integer gear ratios)
  stepper.setSPR(4075.7728395);
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
    
    if (degrees > 360) {
        stepper.setDirection(CW);
        stepper.rotate();
      } else if (degrees > - 360) {
        stepper.setDirection(CCW);
        stepper.rotate();
    } else {
      stepper.rotateDegrees(degrees);
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
  yield();
 
  stepper.run();
  
}


