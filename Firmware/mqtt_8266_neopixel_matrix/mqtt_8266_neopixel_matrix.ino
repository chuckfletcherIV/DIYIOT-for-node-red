
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid     = "4D";
const char* password = "5Y8N0IwFfI";
const char* mqtt_server = "192.168.1.125";
const char* topic = "neopixel_matrix/1";

WiFiClient espClient;
PubSubClient client(espClient);


#include <Adafruit_NeoPixel.h>
#define PIN            12
#define NUMPIXELS     256
int r;
int g;
int b;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

int delayval = 0; // delay for half a second

void setup() {
  Serial.begin(115200);  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pixels.begin(); // This initializes the NeoPixel library.
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
  
//   char message_buff[200];
//
//   int i = 0;
//
//  for(i=0; i<length; i++) {
//    message_buff[i] = payload[i];
//  }
//  message_buff[i] = '\0';
//  
//  String myString = String(message_buff);

  payload[length] = '\0';
  String myString = String((char*)payload);

  Serial.println("incoming string:" + myString);
 
 
int commaIndex = myString.indexOf(',');
int secondCommaIndex = myString.indexOf(',', commaIndex+1);

String firstValue = myString.substring(0, commaIndex);
String secondValue = myString.substring(commaIndex+1, secondCommaIndex);
String thirdValue = myString.substring(secondCommaIndex+1); //To the end of the string  

int r = firstValue.toInt();
int g = secondValue.toInt();
int b = thirdValue.toInt();

Serial.println("before function: " + String(r) + "," + String(g) + "," + String(b));
updatecolors(r,g,b);

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
}

void updatecolors(int r, int g, int b) {
  
  Serial.println("sending colors to led display");
  
  Serial.println(String(r) + "," + String(g) + "," + String(b));

   for(int i=0;i<NUMPIXELS;i++){
     
    pixels.setPixelColor(i, pixels.Color(r,g,b));

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
     pixels.show(); // This sends the updated pixel color to the hardware.
  
}




