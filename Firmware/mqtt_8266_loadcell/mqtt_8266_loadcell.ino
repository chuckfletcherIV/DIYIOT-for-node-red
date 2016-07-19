/*
 HX711 raw reading
by http://freedom2000.free.fr
 */


// the setup function runs once when you press reset or power the board
unsigned long Weight = 0;
unsigned long AverageWeight = 0;
   int Clock = 13; //4 on board
   int Dout = 12; //5 on board
   
void setup() 
{
  //intialize HX711
  pinMode(Clock, OUTPUT); // initialize digital pin 4 as an output.(clock)

  digitalWrite(Clock, HIGH);  
  delayMicroseconds(100);   //be sure to go into sleep mode if > 60µs
  digitalWrite(Clock, LOW);     //exit sleep mode*/

  pinMode(Dout, INPUT);  // initialize digital pin 5 as an input.(data Out)
     
  Serial.begin(115200); // initialize serial communication at 9600 bits per second:
  Serial.println("ready");
  delay(100);
}

// the loop function runs over and over again forever
void loop() 
{
  // wait for the chip to become ready
 while (digitalRead(Dout) == HIGH);

 AverageWeight = 0;
  for (char j = 0; j<100; j++) 
  {
     Weight =0; 
    // pulse the clock pin 24 times to read the data
    for (char i = 0; i<24; i++) 
    {
      digitalWrite(Clock, HIGH);
      delayMicroseconds(2);
      Weight = Weight <<1;
      if (digitalRead(Dout)==HIGH) Weight++;
      digitalWrite(Clock, LOW);
    }
    // set the channel and the gain factor (A 128) for the next reading using the clock pin (one pulse)
    digitalWrite(Clock, HIGH);
    Weight = Weight ^ 0x800000;
    digitalWrite(Clock, LOW);
    AverageWeight += Weight;
    delayMicroseconds(60);
  }
  AverageWeight = AverageWeight/100;
  Serial.println(AverageWeight);
  Serial.println("");
  delay(1000);
}

