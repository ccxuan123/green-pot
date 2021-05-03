// guide https://forum.arduino.cc/t/using-millis-for-timing-a-beginners-guide/483573
// led dimming example using millis() for timing

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 10;  //the value is a number of milliseconds between changes of brightness
const byte ledPin = 6;    //using an LED on a PWM pin.
byte brightness = 0;       //initial brightness
byte increment = 1;        //amount to change the brightness at the end of each period.

void setup()
{
  Serial.begin(115200);  //start Serial in case we need to print debugging info
  pinMode(ledPin, OUTPUT);
  startMillis = millis();  //initial start time
}

void loop()
{
  currentMillis = millis();  //get the current time
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    analogWrite(ledPin, brightness);    //set the brightness of the LED
    brightness += increment;    //will wrap round from 255 to 0 because brightness is an unsigned data type
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED brightness
  }
}
