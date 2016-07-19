#include <SPI.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid     = "4D";
const char* password = "5Y8N0IwFfI";
const char* mqtt_server = "192.168.1.125";
const char* topic = "neopixel_ring/1";
char msg[50];
char message_buffer[100];
String myMsg;

WiFiClient espClient;
PubSubClient client(espClient);

#include <Adafruit_NeoPixel.h>
#define PIN 13
#define LED_COUNT 256
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

long colour[] = { 0x191F61, 0x231D61, 0x2F1D61, 0x3D1D62, 0x4B1F62, 0x592062, 0x682462, 0x6F2562, 0x762862, 0x7E2A62,
                  0x802B50, 0x832E3E, 0x86312E, 0x893620, 0x8C3B15, 0x904019, 0x96441C, 0x9B491E, 0xA04D21, 0xA55124,
                  0xA85424, 0xAA5726, 0xAE5927, 0xB15C29, 0xB45F2A, 0xBD6C2E, 0xC77932, 0xD08636, 0xDA933B, 0xE5A13F, 
                  0xE7AA3E, 0xE9B23E, 0xEBBC3E, 0xEDC43D, 0xF0CD3D, 0xF1D33E, 0xF3D93E, 0xF5DF3F, 0xF7E540, 0xF9EB41,
                  0xFAEE47, 0xFBF150, 0xFBF359, 0xFCF665, 0xFDF970, 0xF7F77C, 0xF0F487, 0xF0F487, 0xEAF294, 0xE4EEA1,
                  0xDEECAE, 0xD8EABB, 0xD2E8C9, 0xCCE6D7, 0xC6E3E6, 0xC1E1F3, 0xCCE7F6, 0xD9EDF8, 0xE5F3FA, 0xF2F9FD,
                  0xF2F9FD, 0xE5F3FA, 0xD9EDF8, 0xCCE7F6, 0xC1E1F3, 0xC6E3E6, 0xCCE6D7, 0xD2E8C9, 0xD8EABB, 0xDEECAE,
                  0xE4EEA1, 0xEAF294, 0xF0F487, 0xF0F487, 0xF7F77C, 0xFDF970, 0xFCF665, 0xFBF359, 0xFBF150, 0xFAEE47,
                  0xF9EB41, 0xF7E540, 0xF5DF3F, 0xF3D93E, 0xF1D33E, 0xF0CD3D, 0xEDC43D, 0xEBBC3E, 0xE9B23E, 0xE7AA3E,
                  0xE5A13F, 0xDA933B, 0xD08636, 0xC77932, 0xBD6C2E, 0xB45F2A, 0xB15C29, 0xAE5927, 0xAA5726, 0xA85424,
                  0xA55124, 0xA04D21, 0x9B491E, 0x96441C, 0x904019, 0x8C3B15, 0x893620, 0x86312E, 0x832E3E, 0x802B50,
                  0x7E2A62, 0x762862, 0x6F2562, 0x682462, 0x592062, 0x4B1F62, 0x3D1D62, 0x2F1D61, 0x231D61, 0x191F61 };

int const potPin = A0; // Potentiometer input
int potVal = 0; // Potentiometer value
int ledPosition; // Position of active LED
int lastpot = 0;  // last potentiometer reading to compair with

void setup()
{
  Serial.begin(115200);  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  leds.begin();  // Call this to start up the LED strip.
  clearLEDs();   // This function, defined below, turns all LEDs off...
  leds.setBrightness(32); // Set global brightness for the whole strip 0-255  
  leds.show();   // ...but the LEDs don't actually update until you call this.
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
    
  //potVal = analogRead(potPin); // potentiometer  
    if( abs (potVal - lastpot) > 6 ) { // lower values than 6 result in flickering back and forth between the active pixels when pot position is in between values
       lastpot = potVal;     // set value before next comapare (pot != lastpot)
       ledPosition = map(potVal, 0, 100, 16, 0); // map the led position on a 120 led strip
       clearLEDs();   // This function, defined below, turns all LEDs off...
       leds.setPixelColor(ledPosition, colour[ledPosition]); // Pointing the active pixel to the colours defined in the array.

       // 5 negative + active LED + 5 positive LEDs retaining their original colour (but dimming on the outside)
       //The intention was there to dim the outer pixels but I've yet to find a way to make it work.
      // leds.setPixelColor(ledPosition-5, colour[ledPosition-5]);
       //leds.setPixelColor(ledPosition-4, colour[ledPosition-4]);
       leds.setPixelColor(ledPosition-3, colour[ledPosition-3]);
       leds.setPixelColor(ledPosition-2, colour[ledPosition-2]);
       leds.setPixelColor(ledPosition-1, colour[ledPosition-1]);
       leds.setPixelColor(ledPosition+1, colour[ledPosition+1]);
       leds.setPixelColor(ledPosition+2, colour[ledPosition+2]);
       leds.setPixelColor(ledPosition+3, colour[ledPosition+3]);
       //leds.setPixelColor(ledPosition+4, colour[ledPosition+4]);
       //leds.setPixelColor(ledPosition+5, colour[ledPosition+5]);       
 
       leds.show();
    }
    
        client.loop();
}

// Sets all LEDs to off, but DOES NOT update the display;
// call leds.show() to actually turn them off after this.
void clearLEDs()
{
  for (int i=0; i<LED_COUNT; i++)
  {
    leds.setPixelColor(i, 0);
  }
}


