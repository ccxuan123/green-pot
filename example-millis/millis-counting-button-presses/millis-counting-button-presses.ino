// guide https://forum.arduino.cc/t/using-millis-for-timing-a-beginners-guide/483573
// counting button presses using millis() for timing

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 5000;  //period during which button input  is valid
const byte buttonPin1 = 13;    //button on pin 13
byte currentButtonState;
byte previousButtonState;
int count = 0;
boolean counting;  //EDIT - corrected mistake

void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin1, INPUT_PULLUP);
  Serial.println("Press the button as many times as possible in 5 seconds");
  counting = true;    //turn on the counting indicator
  startMillis = millis();  //save the start time
}

void loop()
{
  currentMillis = millis();
  previousButtonState = currentButtonState;
  if (currentMillis - startMillis <= period)  //true until the period elapses.  Note that this is the reverse of BWOD
  {
    currentButtonState = digitalRead(buttonPin1);
    if (currentButtonState == LOW and previousButtonState == HIGH)  //the button has become pressed
    {
      count++;    //increment count
      Serial.println(count);
    }
  }
  else  //period has ended
  {
    if (counting == true)  //if we were counting
    {
      Serial.print("Time is up");
      counting = false;    //prevent the message being displayed again
    }
  }
}
