//using LDR module
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

//#define LDRANALOGPIN A0
#define LDRDIGITALPIN 12
#define LEDUSB 3 //digital pin with PWM

bool lightState = false;

/* Task Creation */
static void TaskReadLight(void* pvParameters);
static void TaskLight(void* pvParameters);

/* Task Handle */
TaskHandle_t Handle_ReadLight;
TaskHandle_t Handle_Light;

void setup() 
{
  Serial.begin(9600);
  Serial.println("In Setup Function");

  xTaskCreate(TaskReadLight, "ReadLight", 128, NULL, 1, &Handle_ReadLight);
  xTaskCreate(TaskLight, "Light", 128, NULL, 1, &Handle_Light);
}

void loop() 
{
  // Hooked to IDle task, it will run whenever CPU is idle
}

/* ReadLight Task with priority 1 */
static void TaskReadLight (void* pvParameters)
{
  pinMode(LDRDIGITALPIN, INPUT);
  while(1){
    Serial.println("in task read light");
    if (digitalRead(LDRDIGITALPIN) == 1){
      lightState = true;
    } else {
      lightState = false;
    }
    Serial.println(lightState);
    vTaskDelay(15);
  }
}

/* Light Task with priority 1 */
static void TaskLight (void* pvParameters)
{
  pinMode(LEDUSB, OUTPUT);
  Serial.println("prepare output");
  while(1){
    if(lightState){
      digitalWrite(LEDUSB, HIGH);   
    } else {
      digitalWrite(LEDUSB, LOW); 
    }
    vTaskDelay(15);
  }
}