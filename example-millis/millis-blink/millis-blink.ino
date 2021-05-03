// guide https://forum.arduino.cc/t/using-millis-for-timing-a-beginners-guide/483573
// blink example using millis() for timing

const int ledPin = 11;
const int buttonPin = 13;

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period =1000;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  startMillis = millis();   //initial start time
}

void loop() {
  currentMillis = millis();   //get the current "time"
  if(currentMillis - startMillis >= period){
    digitalWrite(ledPin, !digitalRead(ledPin));
    startMillis = currentMillis;
  }
  
}
