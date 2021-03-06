#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <Servo.h>

/* Define pins for each components */
#define ldrDigitalPin 18 // analog-in at pin A4
#define moistDigitalPin 17 // analog-in at pin A3
#define lightPin  10
#define buzzerPin 2
#define ledPin 3
#define ultraPot A1
#define servoPin 11
const int echoPin = 12; 
const int trigPin = 13;
Servo sprayer; // attach to pin 11


/* Some variables */
long distance;
int distanceLimit;
bool objFlag1 = false; //flag for ultrasonic sensor detected object at 0s
bool objFlag2 = false; //flag for ultrasonic sensor detected object at N s
bool outputOff = false; 

/* Parameters for the system */
int MAXDISTLIMIT = 50;
int MINDISTLIMIT = 2;
int SPRAYTIME = 2000; //in milliseconds
int DELAYTIME = 3000;

/* Function for using ultrasonic sensor */
long readUltrasonicDistance(int triggerPin, int echoPin);

/* Pre-defined tasks function */
static void TaskReadDist1(void *pvParameters);
static void TaskReadDist2(void *pvParameters);
static void TaskOutput(void *pvParameters);
static void TaskCheckLight(void *pvParameters);
static void TaskCheckMoist(void *pvParameters);

void setup() {
  Serial.begin(9600);
  Serial.print("In Step Function\n");

  /* Create tasks */
  xTaskCreate(TaskReadDist1, "ReadDistance1", 80, NULL, 3, NULL);
  xTaskCreate(TaskReadDist2, "ReadDistance2", 80, NULL, 3, NULL);
  xTaskCreate(TaskOutput, "Output", 80, NULL, 3, NULL);
  xTaskCreate(TaskCheckLight, "Check Light", 80, NULL, 1, NULL);
  xTaskCreate(TaskCheckMoist, "Check Moist", 80, NULL, 1, NULL);
}

void loop() {
  //empty
}

/* ReadDist1 Task with priority of 3 */
static void TaskReadDist1(void *pvParameters)
{
  pinMode(ultraPot, INPUT);
  while(1){
    // update distace between detected object and sensor
    distance = readUltrasonicDistance(trigPin, echoPin);

    // update pot value that define current limit
    distanceLimit = map(analogRead(ultraPot), 0, 1023, MINDISTLIMIT, MAXDISTLIMIT);
    
    // print out the current limit about every 1s */
    if(millis()%1000>=900){
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.print("\tLimit: ");
      Serial.println(distanceLimit);
    }

    objFlag1 = (distance <= distanceLimit) ? true : false;
    vTaskDelay(15);
  }
}

/* ReadDist2 Task with priority of 3 */
static void TaskReadDist2(void *pvParameters)
{
  while(1){
    if(objFlag1){
      Serial.print("Object detected. Waiting for output...\n");

      // wait for sometime to check again
      vTaskDelay(DELAYTIME / portTICK_PERIOD_MS);
      distance = readUltrasonicDistance(trigPin, echoPin);

      // update objFlag2
      objFlag2 = (distance <= distanceLimit) ? true : false;
    }
    vTaskDelay(15);
  }
}

/* Output Task with priority of 3 */
static void TaskOutput(void *pvParameters)
{
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  while(1){
    //Check if object is remain detected for 3s
    if(objFlag1 && objFlag2){
      Serial.print("Output activate\n");

      // switch on LED and buzzer
      digitalWrite(ledPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      sprayer.write(180); //spray water

      // set flag to switch off outputs to true
      outputOff = true;
    } else if (!objFlag1 && outputOff){
      vTaskDelay(1000/portTICK_PERIOD_MS);
      Serial.print("Output deactivate\n");

      // switch off LED and buzzer
      digitalWrite(ledPin, LOW);
      digitalWrite(buzzerPin, LOW);
      sprayer.write(0);

      // set flag to switch off outputs to false
      outputOff = false;
    }
    vTaskDelay(15);  
  }
}

/* Check light Task with priority of 1 */
static void TaskCheckLight (void *pvParameters)
{
  pinMode(ldrDigitalPin, INPUT);
  pinMode(lightPin, OUTPUT);
  while(1){
    // check if the surroundings is too dark
    if(digitalRead(ldrDigitalPin)){
      Serial.print("Dark light on\n");
      //Switch on the extra light
      digitalWrite(lightPin, HIGH);
    } else {
      //Switch off the extra light
      Serial.print("Bright light off\n");
      digitalWrite(lightPin, LOW);
    }
    vTaskDelay (2000/portTICK_PERIOD_MS);
  }
}

/* Check Moisture task with priority of 1 */
static void TaskCheckMoist (void *pvParameters)
{
  sprayer.attach(servoPin);
  pinMode(moistDigitalPin, INPUT);
  while(1){
    //Check if the pot is too dry
    if(digitalRead(moistDigitalPin)){
      Serial.print("Too dry, spray water for 2s\n");

      //activate sprayer for 2 seconds
      sprayer.write(180);
      vTaskDelay( SPRAYTIME /portTICK_PERIOD_MS);
      sprayer.write(0);
    } else {
      Serial.print("soil is damp\n");
    }
    vTaskDelay(5000/portTICK_PERIOD_MS);
  }
}

/* Functions for using ultrasonic sensor */
long readUltrasonicDistance(int triggerPin, int echoPin){
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return 0.01723 * pulseIn(echoPin, HIGH);
}