#include <Arduino.h>
#include <Arduino_FreeRTOS.h> // import FreeRTOS library
#include <Servo.h>  // import servo library

/* Define pins for each components */
#define ldrDigitalPin 18 // attach LDR digital pin to A4
#define moistDigitalPin 17 // attach moisture sensor digital pin to A3
#define lightPin  10  // attach artificial light to pin 10
#define servoPin 11 // attach servo motor to pin 11 (with pwm)

Servo sprayer; // initiate sprayer as Servo objct

int SPRAYTIME = 2000; // variables to specify the spray time

/* Pre-defined tasks function */
static void TaskCheckLight(void *pvParameters);
static void TaskCheckMoist(void *pvParameters);

void setup() {
  Serial.begin(9600); // initiate serial monitor at 9600 baud rate
  Serial.println("In Step Function");

  sprayer.write(0); // reset servo motor at staring position

  /* Create tasks */
  xTaskCreate(TaskCheckLight, "Check Light", 128, NULL, 1, NULL);
  xTaskCreate(TaskCheckMoist, "Check Moist", 128, NULL, 1, NULL);
}

void loop() {
  ///Serial.println(F("Loop function"));:
}

/* Check light Task with priority of 1 */
static void TaskCheckLight (void *pvParameters)
{
  /* initialize the components */ 
  pinMode(ldrDigitalPin, INPUT);  // LDR module as input
  pinMode(lightPin, OUTPUT);  // Artificial light as output

  while(1){
    /* check if the surroundings is too dark */
    if(digitalRead(ldrDigitalPin)){
      Serial.println("Surrounding is dark");
      digitalWrite(lightPin, HIGH); //Switch on the artificial light
      Serial.println("Activate artificial light");
    } else {
      Serial.println("Surrounding is bright");
      digitalWrite(lightPin, LOW); //Switch off the artificial light
      Serial.println("Deactivate artificial light");
    }
    vTaskDelay (2000/portTICK_PERIOD_MS); // Delay the task for 2s
  }
}

/* Check Moisture task with priority of 1 */
static void TaskCheckMoist (void *pvParameters)
{
  /* initialize the components */
  sprayer.attach(servoPin); // servo motor as output
  pinMode(moistDigitalPin, INPUT);  // moisture sensor as input
  while(1){
    /* Check if the pot is too dry */
    if(digitalRead(moistDigitalPin)){
      Serial.println("The soil is dry");
      Serial.println("Spray water for 2s");

      /* activate sprayer for 2 seconds */
      sprayer.write(180);
      vTaskDelay( SPRAYTIME /portTICK_PERIOD_MS); //to delay for SPRAYTIME
      sprayer.write(0);

      Serial.println("Stop spray water");
    } else {
      Serial.println("The soil is damp");
    }
    vTaskDelay(5000/portTICK_PERIOD_MS); // Delay the task for 5s
  }
}