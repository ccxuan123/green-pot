//using LDR module
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#define LDRDIGITALPIN 12
#define LEDUSB 3 //digital pin with PWM

int lightDelay = 2000;

/* Task Creation */
static void TaskCheckLight(void *pvParameters);

/* Task Handle */
TaskHandle_t Handle_CheckLight;

void setup() 
{
  Serial.begin(9600);
  Serial.println("In Setup Function");

  xTaskCreate(TaskCheckLight, "ReadLight", 128, NULL, 1, &Handle_CheckLight);
}

void loop() 
{
  // Hooked to IDle task, it will run whenever CPU is idle
}

/* ReadLight Task with priority 1 */
static void TaskCheckLight (void* pvParameters)
{
  pinMode(LDRDIGITALPIN, INPUT);
  pinMode(LEDUSB, OUTPUT);
  while(1){
    //Serial.println("in task read light");
    if (digitalRead(LDRDIGITALPIN) == 1){
      Serial.println("too dark, on light");
      digitalWrite(LEDUSB, HIGH);
    } else {
      digitalWrite(LEDUSB, LOW);
    }
    vTaskDelay( lightDelay / portTICK_PERIOD_MS);
  }
}
