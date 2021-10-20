/*
 * https://github.com/ochin-space/miscellaneous
 * Designer: perniciousflier
 * date: 19/10/2021
 * Note: this fw is meant to be used on Arduino Nano.
 * This project is designed to precisely measure the glass to glass latency of a video tx rx system.
 * To measure the latency you have to mesure the offset first (the latency of this circuit). 
 * To do it you have to place the photocell in front of the led diode and then press the button for more then 3seconds. 
 * Once the offset has been measured you can measure the latency of your system placing the led diode in front of the camera
 * and the fotocell in front of the screen (before check where is the led on your screen). To make the measure just brifely press 
 * the button and see on the serial terminal the latency value measured in microseconds.
*/

char latencyStr[20];
const byte buttonPin = 3;
const byte ledPin = 4;
const byte interruptPin = 2;
unsigned long StartTime, latency, offset=0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(interruptPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(interruptPin), interrupt_offset, FALLING );
  StartTime = micros();
  digitalWrite(ledPin, HIGH);
}

void loop() {
  unsigned long cnt=0;
  while(digitalRead(buttonPin)){delay(10);}
  while(!digitalRead(buttonPin)){delay(10);cnt++;}
  if(cnt>300) //pressed for >=3sec
  {
    attachInterrupt(digitalPinToInterrupt(interruptPin), interrupt_offset, FALLING );
  }
  else
  {
    attachInterrupt(digitalPinToInterrupt(interruptPin), interrupt_handler, FALLING );
  }
  StartTime = micros();
  digitalWrite(ledPin, HIGH);    
}

void interrupt_handler() {
  latency = micros()-StartTime;
  if(latency>offset) latency = latency-offset;
  else latency = 0; 
  detachInterrupt(digitalPinToInterrupt(interruptPin));
  digitalWrite(ledPin, LOW);
  String latencyStr = String(latency, DEC);
  Serial.print("Latency: ");
  Serial.print(latencyStr);  
  Serial.println(" us");
}

void interrupt_offset() {
  offset = micros()-StartTime;
  detachInterrupt(digitalPinToInterrupt(interruptPin));
  digitalWrite(ledPin, LOW);
  String offsetStr = String(offset, DEC);
  Serial.print("Offset: ");
  Serial.print(offsetStr);  
  Serial.println(" us");
}
