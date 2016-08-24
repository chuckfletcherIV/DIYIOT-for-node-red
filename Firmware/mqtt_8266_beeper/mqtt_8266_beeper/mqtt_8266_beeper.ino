void setup()  { 
  // declare pin 9 to be an output:
  pinMode(13, OUTPUT);
  beep(50);
  beep(50);
  beep(50);
  delay(1000);
} 

void loop()  { 
  beep(200); 
}

void beep(unsigned char delayms){
  analogWrite(13, 20);      // Almost any value can be used except 0 and 255
                           // experiment to get the best tone
  delay(delayms);          // wait for a delayms ms
  analogWrite(13, 0);       // 0 turns it off
  delay(delayms);          // wait for a delayms ms   
}  
