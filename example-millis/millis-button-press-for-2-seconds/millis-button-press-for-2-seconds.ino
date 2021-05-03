//output only trigger when the button is pressed for more than 2 seconds
//the output task is led light up for 2 seconds. 
//if the button is released after pressed for more than 2 seconds, 
//led will swich off after 2 seconds


unsigned long currentMillis;
unsigned long buttonStartMillis;
unsigned long outputStartMillis;
const unsigned long buttonPeriod = 2000;
const unsigned long outputPeriod = 2000;

const byte ledPin = 6;
const byte buttonPin = 13;

boolean waitingState = false; //true if the button is pressed for more than buttonPeriod
boolean outputState = false;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  //buttonStartMillis = millis();
}

void loop() {
  currentMillis = millis();
  if(!digitalRead(buttonPin) && !waitingState){
    waitingState = true;
    //Serial.println("start");
    buttonStartMillis = currentMillis;
  } else if (digitalRead(buttonPin)){
    waitingState = false;
  }
  
  if(waitingState){
    if(currentMillis - buttonStartMillis >= buttonPeriod){
      Serial.println("button pressed for 5 s");
      outputState = true;
      //flag1 = false;
      //buttonStartMillis = currentMillis;
      outputStartMillis = currentMillis;
    }
  }

  if (outputState){
    digitalWrite(ledPin,HIGH);
    if(currentMillis - outputStartMillis >= outputPeriod){
      digitalWrite(ledPin, LOW);
      outputState = false;
    }
  }

}
