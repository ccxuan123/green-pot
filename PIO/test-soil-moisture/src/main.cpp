#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <Servo.h>

#define moistDigital 12
#define servoPin 9
//#define moistAnalog A0
Servo sprayer;  // create servo object to control a servo

/* Task Createion */
//static void TaskSprayWater (void *pvParameters);
//static void TaskReadMoist(void *pvParameters);
static void TaskCheckMoist (void *pvParameters);

/* Task Handle */
//TaskHandle_t Handle_SprayWater;
//TaskHandle_t Handle_ReadMoist;
TaskHandle_t Handle_CheckMoist; 

void setup() {
  Serial.begin(9600);
  Serial.println("In Setup Function");

  //xTaskCreate(TaskSprayWater, "SprayWater", 128, NULL, 0, &Handle_SprayWater);
  //xTaskCreate(TaskReadMoist, "ReadMoist", 128, NULL, 1, &Handle_ReadMoist);
  xTaskCreate(TaskCheckMoist, "CheckMoist", 128, NULL, 1, &Handle_CheckMoist);
}

void loop() {

}

// from moisture sensor module
// wet low analog value, digital low
// dry high analog value, digital high

/* Check moisture task with priority of 1 */
static void TaskCheckMoist(void *pvParameters)
{
  sprayer.attach(servoPin);
  pinMode(moistDigital, INPUT);
  while(1){
    if(digitalRead(moistDigital)){
      Serial.println("Spray water");
      sprayer.write(180);
      vTaskDelay( 2000 / portTICK_PERIOD_MS);
      sprayer.write(0);
    }
    vTaskDelay(2000/portTICK_PERIOD_MS);
  }
}


/* Spray water task with priority of 0 */
/*
static void TaskSprayWater (void *pvParameters)
{
  sprayer.attach(9);
  //sprayer.write(0);
  while(1){
    Serial.println("start 1");
    sprayer.write(30);
    vTaskDelay(2000/portTICK_PERIOD_MS);
    Serial.println("start 2");
    sprayer.write(60);
    vTaskDelay(2000/portTICK_PERIOD_MS);
  }
}*/


/*
static void TaskReadMoist(void *pvParameters)
{
  pinMode(moistAnalog, INPUT);
  pinMode(moistDigital, INPUT);
  while(1){
    Serial.print(analogRead(moistAnalog));
    Serial.print("\t");
    Serial.println(digitalRead(moistDigital));
    vTaskDelay(5);
  }
}*/