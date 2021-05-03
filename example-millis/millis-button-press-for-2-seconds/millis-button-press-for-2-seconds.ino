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
}

void loop() {
  currentMillis = millis(); // start counting the ms passed since program is started

  //!waitingState is added to if condition to prevent stuck in 
  //this if block when button is pressed continously
  if(!digitalRead(buttonPin) && !waitingState){   //check whether button is pressed
    waitingState = true;             //flag that the 2 seconds waiting for button is started
    //Serial.println("start");
    buttonStartMillis = currentMillis;  //save the time when button is pressed
  } else if (digitalRead(buttonPin)){
    waitingState = false;            //flag off when button is released
  }
  
  if(waitingState){                  //waiting for button started
    if(currentMillis - buttonStartMillis >= buttonPeriod){   //if the continous input exceed the specify limit
      Serial.println("button pressed for 5 s");
      outputState = true;            //flag the output is ready to take place
      //flag1 = false;
      //buttonStartMillis = currentMillis;
      outputStartMillis = currentMillis;    //save the starting time for output
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
