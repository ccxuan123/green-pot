#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
//#pragma GCC ("03")

TaskHandle_t TaskHandle_ReadDistance;
TaskHandle_t TaskHandle_CheckObject;
TaskHandle_t TaskHandle_Output;
TaskHandle_t TaskHandle_blink;

const int echoPin = 8; 
const int trigPin = 9; 
const int LED = 11;

long distance;

bool outputState = false;
bool flag1 = false;
bool flag2 = false;
bool outputOff = false;

/* function for using ultrasonic sensor */
void displayDistance();
long readUltrasonicDistance(int triggerPin, int echoPin);

/* Task creation */
static void TaskReadDistance(void* pvParameters);
static void TaskCheckObject(void* pvParameters);
static void TaskOutput(void *pvParameters);
static void TaskBlink(void *pvParameters);

void setup() 
{
  Serial.begin(9600);
  Serial.println(F("In Setup function"));  

  xTaskCreate(TaskReadDistance, "ReadDistance", 128, NULL, 3, &TaskHandle_ReadDistance);
  xTaskCreate(TaskCheckObject, "Check Object", 128, NULL, 3, &TaskHandle_CheckObject);
  xTaskCreate(TaskOutput,"Output", 128, NULL, 2, &TaskHandle_Output);
  xTaskCreate(TaskBlink, "Blink", 128, NULL, 1, &TaskHandle_blink);
}

void loop() 
{
  // Hooked to IDle task, it will run whenever CPU is idle
  //Serial.println(F("Loop function"));
}

/* ReadDistance Task with priority 2*/
static void TaskReadDistance(void* pvParameters)
{
  while(1) {
    //Serial.println(F("sensor task is running"));
    distance = readUltrasonicDistance(trigPin, echoPin);
    displayDistance();
    /*
    flag1 = (distance <= 20) ? true : false;
    if (flag1) {
      Serial.println("flag 1 triggered");
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      distance = readUltrasonicDistance(trigPin, echoPin);
      flag2 = (distance <= 20) ? true : false;
    }
    if (flag2) {
      Serial.println("flag 2 triggered");
    }*/
    vTaskDelay(5);  // one tick is 15 ms
  }
}

static void TaskCheckObject(void* pvParameters)
{
  while(1){
    if (distance <= 20) {
      Serial.println("flag1 triggered");
      flag1 = true;
    } else {
      flag1 = false;
    }
    //vTaskDelay(2000 / portTICK_PERIOD_MS);
    if(flag1){
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      Serial.println("flag2 triggered");
      flag2 = (distance <= 20) ? true : false ;
    }
    vTaskDelay(5);
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
    }
    if ( !flag1 && outputOff ){
    //if (outputOff){  
      //vTaskDelay(1000 / portTICK_PERIOD_MS);
      Serial.println("output deactivate");
      digitalWrite(LED, LOW);
      outputOff = false;
    }
  vTaskDelay(5); 
  }
}

/* TaskBlink with priority 2*/
static void TaskBlink(void *pvParameters)
{
  pinMode(LED_BUILTIN, OUTPUT);
  while(1) {
    digitalWrite(LED_BUILTIN, HIGH);   
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
    digitalWrite(LED_BUILTIN, LOW);   
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
  }
}

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