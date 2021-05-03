// guide https://forum.arduino.cc/t/using-millis-for-timing-a-beginners-guide/483573
// counting button presses with 20ms using millis() for timing
// this code is a method to debounce push button

unsigned long periodStartMillis;
unsigned long currentMillis;
const unsigned long period = 5000;  //period during which button input is valid
const byte buttonPin1 = 13;    //button on pin 13
byte currentButtonState;
byte previousButtonState;
int count = 0;
boolean printFinalMessage = true;
unsigned long debounceStartMillis;
unsigned long debouncePeriod = 20;
boolean debouncing = false;

void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin1, INPUT_PULLUP);
  Serial.println("Press the button as many times a possible in 5 seconds");
  periodStartMillis = millis();
}

void loop()
{
  currentMillis = millis();
  if (currentMillis - periodStartMillis <= period)  //true until the period elapses
  {
    previousButtonState = currentButtonState;    //save the previous button state
    currentButtonState = digitalRead(buttonPin1);  //read the current state of the input
    if (currentButtonState != previousButtonState) //if the button state has changed
    {
      debounceStartMillis = currentMillis;  //save the time that the state change occured
      debouncing = true;  //flag that debouncing in progress
    }    //end state change check
    
    if (currentMillis - debounceStartMillis >= debouncePeriod)  //if the debounce period has elapsed
    {
      if (debouncing == true)    //debouncing taking place
      {
        if (currentButtonState == LOW)  //if the button is currently pressed
        {
          debouncing = false;    //debouncing is finished
          count++;               //increment the count
          Serial.println(count);
        }    //end count increment
      }  //end debouncing in progress check
    }    //end debounce time elapsed check
  }  //end timing period check
  else  //period has ended
  {
    if (printFinalMessage == true)
    {
      Serial.println("Time is up");
      Serial.print("Button pressed count : ");
      Serial.println(count);
      printFinalMessage = false;    //prevent the final message being displayed again
    }    //end printing final message
  }    //end final message check
}
