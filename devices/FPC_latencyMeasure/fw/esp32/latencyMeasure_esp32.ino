
char latencyStr[20];

const byte buttonPin = 0;
const byte ledPin = 12; //D6
const byte interruptPin = 14; //D5
unsigned long StartTime, latency, offset=0;

void ICACHE_RAM_ATTR interrupt_handler();
void ICACHE_RAM_ATTR interrupt_offset();

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(interruptPin), interrupt_offset, FALLING );
  StartTime = micros();
  digitalWrite(ledPin, HIGH);
}

void loop() {
  while(digitalRead(buttonPin)){delay(10);}
  while(!digitalRead(buttonPin)){delay(10);}
  attachInterrupt(digitalPinToInterrupt(interruptPin), interrupt_handler, FALLING );
  StartTime = micros();
  digitalWrite(ledPin, HIGH);
}

void interrupt_handler() {
  latency = micros()-StartTime;
  if(latency>offset) latency = latency-offset;
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
