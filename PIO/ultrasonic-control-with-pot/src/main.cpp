#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

TaskHandle_t TaskHandle_ReadDistance;
TaskHandle_t TaskHandle_LedOutput;

#define ultraPot A1
const int echoPin = 8; // attach pin 8 Arduino to pin Echo of HC-SR04
const int trigPin = 9; //attach pin 9 Arduino to pin Trig of HC-SR04
const int LED = 11;
long distance;
int distanceLimit;


long readUltrasonicDistance(int triggerPin, int echoPin);
void displayDistance();

static void ReadDistance(void *pvParameters);
static void Led(void *pvParameters);

void setup() {
  Serial.begin(9600);
  Serial.println(F("In Setup function"));  

  xTaskCreate(ReadDistance, "ReadDistance", 128, NULL, 3, &TaskHandle_ReadDistance);
  xTaskCreate(Led,"led", 128, NULL, 1, &TaskHandle_LedOutput);
}

void loop() {
  // Hooked to IDle task, it will run whenever CPU is idle
  //Serial.println(F("Loop function"));
}

/* ReadDistance Task with priority 3*/
static void ReadDistance(void *pvParameters){
  while(1){
    //Serial.println(F("sensor task is running"));
    distance = readUltrasonicDistance(trigPin, echoPin);
    Serial.print("Distance: ");
    Serial.println(distance);
    vTaskDelay(1);  // one tick is 15 ms
  }
}

/* LedOutput with priority 1*/
static void Led(void *pvParameters){
  pinMode(LED, OUTPUT);
  pinMode(ultraPot, INPUT);
  while(1){
    distanceLimit = map(analogRead(ultraPot), 0, 1023, 5, 50);
    if (distance <= distanceLimit){
      digitalWrite(LED, HIGH);
      //Serial.println("LED activate");
    }
    else {
      digitalWrite(LED, LOW);
      //Serial.println("LED deactivate");
    }
    vTaskDelay(1);
  }
}


/*functions for ultrasonic sensor*/
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
