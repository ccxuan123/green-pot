#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
//#pragma GCC ("03")

const int echoPin = 8; 
const int trigPin = 9; 
const int LED = 11;

long distance;

bool outputState = false;
bool outputOff = false;
bool flag1 = false;
bool flag2 = false;

/* function for using ultrasonic sensor */
void displayDistance();
long readUltrasonicDistance(int triggerPin, int echoPin);

/* Pre-defind tasks function */
static void TaskReadDist1(void* pvParameters);  // Task for reading distance at 1st point
static void TaskReadDist2(void* pvParameters);  // Task for reading distance at 2nd point
static void TaskOutput(void *pvParameters);

/* Task handles */
TaskHandle_t Handle_ReadDist1;
TaskHandle_t Handle_ReadDist2;
TaskHandle_t Handle_Output;

void setup() 
{
  Serial.begin(9600);
  Serial.println(F("In Setup function"));  

  xTaskCreate(TaskReadDist1, "ReadDistance1", 128, NULL, 3, &Handle_ReadDist1);
  xTaskCreate(TaskReadDist2, "ReadDistance2", 128, NULL, 3, &Handle_ReadDist2);
  xTaskCreate(TaskOutput,"Output", 128, NULL, 1, &Handle_Output);
}

void loop() 
{
  // Hooked to IDle task, it will run whenever CPU is idle
  //Serial.println(F("Loop function"));
}

/* ReadDistance Task with priority 2*/
static void TaskReadDist1(void* pvParameters)
{
  while(1) {
    //Serial.println(F("sensor task is running"));
    distance = readUltrasonicDistance(trigPin, echoPin);
    flag1 = (distance <= 20) ? true : false;
    vTaskDelay(15);  // one tick is 15 ms
  }
}

static void TaskReadDist2(void* pvParameters)
{
  while(1) {
    if(flag1) {
      Serial.println("flag 1 triggered");
      vTaskDelay(3000 / portTICK_PERIOD_MS);
      distance = readUltrasonicDistance(trigPin, echoPin);
      flag2 = (distance <= 20) ? true : false;
    }
    if (flag2) {
      Serial.println("flag 2 triggered");
    }
    vTaskDelay(15);  // one tick is 15 ms
  }
}

/* Output with priority 2*/
static void TaskOutput(void *pvParameters)
{
  while(1){
    pinMode(LED, OUTPUT);
    if (flag1 && flag2){
      Serial.println("output activate");
      digitalWrite(LED, HIGH);
      outputOff = true;
      
    } else if (!flag1 && outputOff){
      vTaskDelay(1000/portTICK_PERIOD_MS);
      Serial.println("output deactivate");
      digitalWrite(LED, LOW);
      outputOff = false;
    }
  vTaskDelay(15); 
  }
}

/* Function for using ultrasonic sensor*/
long readUltrasonicDistance(int triggerPin, int echoPin)
{
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

void displayDistance()
{
  long detectDistance = readUltrasonicDistance(trigPin, echoPin);
  Serial.print("Distance: ");
  Serial.println(detectDistance);
}
