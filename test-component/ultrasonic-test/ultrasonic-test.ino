
#include <Arduino_FreeRTOS.h>
#pragma GCC ("03")

TaskHandle_t TaskHandle_ReadDistance;
TaskHandle_t TaskHandle_LedOutput;
TaskHandle_t TaskHandle_blink;

const int echoPin = 8; // attach pin 8 Arduino to pin Echo of HC-SR04
const int trigPin = 9; //attach pin 9 Arduino to pin Trig of HC-SR04
const int LED = 11;
long distance;

void ReadDistance(void* pvParameters);
static void Led(void *pvParameters);
static void TaskBlink(void *pvParameters);

void setup() {
  Serial.begin(9600);
  Serial.println(F("In Setup function"));  

  xTaskCreate(ReadDistance, "ReadDistance", 128, NULL, 3, &TaskHandle_ReadDistance);
  xTaskCreate(Led,"led", 128, NULL, 1, &TaskHandle_LedOutput);
  xTaskCreate(TaskBlink, "Blink", 128, NULL, 1, &TaskHandle_blink);

}

void loop() {
  // Hooked to IDle task, it will run whenever CPU is idle
  Serial.println(F("Loop function"));
}

/* ReadDistance Task with priority 2*/
static void ReadDistance(void* pvParameters)
{
  while(1)
  {
    Serial.println(F("sensor task is running"));
    distance = readUltrasonicDistance(trigPin, echoPin);
    Serial.print("Distance: ");
    Serial.println(distance);
    vTaskDelay(1);  // one tick is 15 ms
  }
}

/* LedOutput with priority 2*/
static void Led(void *pvParameters)
{
  pinMode(LED, OUTPUT);
  while(1)
  {
    if (distance <= 10)
    {
      digitalWrite(LED, HIGH);
      Serial.println("LED activate");
    }
    else 
    {
      digitalWrite(LED, LOW);
      Serial.println("LED deactivate");
    }
    vTaskDelay(1);
  }
}


/* TaskBlink with priority 2*/
static void TaskBlink(void *pvParameters)
{
  pinMode(LED_BUILTIN, OUTPUT);
  while(1)
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  }
}


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

void displayDistance(){
  long detectDistance = readUltrasonicDistance(trigPin, echoPin);
  Serial.print("Distance: ");
  Serial.println(detectDistance);
}
