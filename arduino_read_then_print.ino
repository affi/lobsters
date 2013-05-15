/*
  Reads serial data and prints to the serial monitor
 
  Janille Maragh 5/15
 */

char str[310];
int n;

// runs once when the program starts
void setup() {
  // open serial port
  Serial.begin(9600);
  n=0;
}

// runs over and over again
void loop() {
  // check if new data is available
  if (Serial.available() > 0) {
   n = Serial.readBytesUntil('\n', str, 300);
   str[n] = 0;
   Serial.println(str);
    
  }
  // wait for 20ms before checking again for new data
  delay(20);
}
