#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<Wire.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

const int numReadings = 100;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

// Update these with values suitable for your network.

const char* ssid     = "DIYIOT";
const char* password = "diyiotdiyiot";
const char* mqtt_server = "10.10.10.3";

  String message;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Wire.pins(2,14); // SDA, SCL
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  
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
    if (client.connect("accel/1")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("accel/1", "connected");
      // ... and resubscribe
      client.subscribe("test/topic");
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
  
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  float AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  float AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  float AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  float Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  float GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  float GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  float GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  
  GyX = GyX/131;
  GyY = GyY/131;
  GyZ = GyZ/131;
  AcX = AcX/131;
  AcY = AcY/131;
  AcZ = AcZ/131;
  
  Serial.print("AcX = "); Serial.print(AcX);
  Serial.print(" | AcY = "); Serial.print(AcY);
  Serial.print(" | AcZ = "); Serial.print(AcZ);
  Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
  Serial.print(" | GyX = "); Serial.print(GyX);
  Serial.print(" | GyY = "); Serial.print(GyY);
  Serial.print(" | GyZ = "); Serial.println(GyZ);
//  root["x_accel"] = AcX;
//  root["y_accel"] = AcY;
//  root["z_accel"] = AcZ; 
//  root["x_rot"] = GyX;
//  root["y_rot"] = GyY;
//  root["z_rot"] = GyZ;
  
  
// total = total - readings[readIndex];
//  readings[readIndex] = GyX;
//  total = total + readings[readIndex];
//  readIndex = readIndex + 1;
//  if (readIndex >= numReadings)
//    readIndex = 0;
//  average = total / numReadings;
  
  //average = map(average, 0, 1023, -180, 180);
  
  String myAverage = String(GyX) + ", " +  String(GyY) +  ", " +  String(GyZ) + ", " + String(AcX) + ", " +  String(AcY) +  ", " +  String(AcZ);
 
 // delay(333);

  if (!client.connected()) {
    reconnect();
  }

   client.loop();
    
//    char buffer[256];
//    root.printTo(message);
//    root.printTo(buffer, sizeof(buffer));
   
   // Serial.print("my data string: " + message);
    char msg[75];
    
     myAverage.toCharArray(msg,75);
    //Serial.print("Publish message: ");
    //Serial.println(msg);
    //client.publish("accel/2", buffer);
    client.publish("accel/1", msg);
}
