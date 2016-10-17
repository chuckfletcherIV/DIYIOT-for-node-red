#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>

IRsend irsend(13);

const char* ssid     = "DIYIOT";
const char* password = "diyiotdiyiot";
const char* mqtt_server = "10.10.10.3";
const char* topic = "ir_send/1";

unsigned int code;

unsigned int walle_light[255] = {6300,-2150,600,-1600,1600,-700,1500,-700,600,-1650,600,-1650,600,-1650,1550,-700,600,-1650,1550,-700,600,-1650,1550,-700,600,-1650,600,-1650,1550,-700,550,-1650,650,-1600,1550,-700,600,-1650,1550,-700,600,-1650,600,-1650,1550,-700,550,-1650,650,-1600,2150};

WiFiClient espClient;
PubSubClient client(espClient);
char message_buffer[100];
String make;
String thisCode;

void setup()  { 
  irsend.begin();
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
  int i = 0;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  payload[length] = '\0';
  String message = String((char*)payload);
  Serial.println("Received: " + String(message));
  
//    String make = getValue(message, ',', 0);
//    String thisCode = String(getValue(message, ',', 1));
//    code = getValue(message, ',', 1).toInt();
//    
//    Serial.println("Found make: " + make);
//    
//   if (make == "nec") {
//    Serial.println("Found Nec Code: " + thisCode);
//   irsend.sendNEC(code, 36);
//   delay(2000);
//  } else if (make == "sony") {
//    Serial.println("Found Sony Code: " + thisCode);
//  irsend.sendSony(code, 12);
//   delay(2000);
//  }

if (message == "walle_light") {
     irsend.sendRaw(walle_light, 255, 32);
}

//		SEND_RC5
//		SEND_RC6
//		SEND_NEC
//		SEND_SONY
//		SEND_PANASONIC
//		SEND_JVC
//		SEND_SAMSUNG
//		SEND_WHYNTER
//		SEND_AIWA_RC_T501
//		SEND_LG
//		SEND_SANYO
//		SEND_MISUBISHI
//		SEND_DISH
//		SEND_SHARP
//		SEND_DENON
//		SEND_PRONTO
//		SEND_LEGO_PF
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    
      Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(topic)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("channels", topic);
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


void loop()  { 

    if (!client.connected()) {
    reconnect();
  }
  client.loop();
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
