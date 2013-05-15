//initialze variables - these are the ones that will be random and sent to affi
long w;
long x;
long y;
long z;
long bi;
long place = 0;

long a;
long b;
long c;
long d;

// this is the string that's sent above water
String com1;
char* test;

//this is the name of the saved incoming string
String incomingString;

void setup(){
  Serial.begin(9600);           // initializing Serial monitor
}

void loop(){
// generate random values to send above water  
  w = 100;//random(100);
  x = random(100);
  y = random(100);
  z = random(100);
  bi = random(0,2);
// these are the random decimal endings of the above numbers  
  a = 0;//random(0,100);
  b = random(0,100);
  c = random(0,100);
  d = random(0,100);
 /* 
// save incoming data from affi if it's available
  while (Serial.available() > 0) {
          // read the incoming string:
          incomingString = String(Serial.read());

          // print what you got:
     //     Serial.println(incomingByte, DEC);
  }
 */ 
  
  // write the string to send above water - space delimited
  com1 += String(w);
  com1 += ".";
  com1 += String(a);
  com1 += ",";
  com1 += String(x);
  com1 += ".";
  com1 += String(b);
  com1 += ",";
  com1 += String(y);
  com1 += ".";
  com1 += String(c);
  com1 += ",";
  com1 += String(z);
  com1 += ".";
  com1 += String(d);
  com1 += ",";
  com1 += String(place);
  //com1 += "\n";
  // this part is the last part of the string and is just sending back
  // the data just received from affi -  code needs to be written to parse
  // the byte and send it back as subcomponents, not just one un-parsed string
  //com1 += incomingString;
  
  test="teststring.";
  
  Serial.println(com1);

  // erase com1 string so it only contains the latest values next loop
  com1 = "";

  delay(250);
}
