#include <Arduino_FreeRTOS.h>
#pragma GCC ("03")

TaskHandle_t TaskHandle_ReadDistance1;
TaskHandle_t TaskHandle_ReadDistance2;
TaskHandle_t TaskHandle_Output;
TaskHandle_t TaskHandle_blink;

const int echoPin = 8; 
const int trigPin = 9; 
const int LED = 11;

long distance;

bool outputState = false;
bool flag1 = false;
bool flag2 = false;

/* function for using ultrasonic sensor */
void displayDistance();
long readUltrasonicDistance(int triggerPin, int echoPin);

/* Task creation */
static void TaskReadDistance1(void* pvParameters);
static void TaskReadDistance2(void* pvParameters);
static void TaskOutput(void *pvParameters);
static void TaskBlink(void *pvParameters);

void setup() {
  Serial.begin(9600);
  Serial.println(F("In Setup function"));  

  xTaskCreate(TaskReadDistance1, "ReadDistance1", 128, NULL, 3, &TaskHandle_ReadDistance1);
  xTaskCreate(TaskReadDistance2, "ReadDistance2", 128, NULL, 3, &TaskHandle_ReadDistance2);
  xTaskCreate(TaskOutput,"Output", 128, NULL, 1, &TaskHandle_Output);
  xTaskCreate(TaskBlink, "Blink", 128, NULL, 1, &TaskHandle_blink);

}

void loop() {
  // Hooked to IDle task, it will run whenever CPU is idle
  //Serial.println(F("Loop function"));
}

/* ReadDistance Task with priority 2*/
static void TaskReadDistance1(void* pvParameters){
  while(1){
    //Serial.println(F("sensor task is running"));
    distance = readUltrasonicDistance(trigPin, echoPin);
    //displayDistance();

    //outputState = (distance <= 20) ? true : false;  //true if distance <= 20, else is false
    flag1 = (distance <= 20) ? true : false;
    
    vTaskDelay(1);  // one tick is 15 ms
  }
}

static void TaskReadDistance2(void* pvParameters){
  while(1){
    if(flag1){
      Serial.println("flag 1 triggered");
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      distance = readUltrasonicDistance(trigPin, echoPin);
      flag2 = (distance <= 20) ? true : false;
    if (flag2){
      Serial.println("flag 2 triggered");
    }
    }
    vTaskDelay(1);  // one tick is 15 ms
  }
}

/* Output with priority 2*/
static void TaskOutput(void *pvParameters){
  pinMode(LED, OUTPUT);
  while(1){
    if (flag1 && flag2){
      Serial.println("output activate");
      digitalWrite(LED, HIGH);
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      digitalWrite(LED, LOW);
      Serial.println("output deactivate");
      //Serial.println("LED activate");
    }
    vTaskDelay(1); 
  }
}


/* TaskBlink with priority 2*/
static void TaskBlink(void *pvParameters){
  pinMode(LED_BUILTIN, OUTPUT);
  while(1){
    digitalWrite(LED_BUILTIN, HIGH);   
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
    digitalWrite(LED_BUILTIN, LOW);   
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
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
