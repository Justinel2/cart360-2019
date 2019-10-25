#include <Keyboard.h> // library to simulate keyboard events
#define LED_PIN 9
int sensorPin = A0; // define sensor port (analog A0)
int value = 0;
int thresh1 = 100;
int thresh2 = 270;
int thresh3 = 400;
int thresh4 = 700;
boolean sensorState = false;
boolean releaseState = true;
void setup() {
// put your setup code hexre, to run once:
pinMode(LED_PIN, OUTPUT);
Keyboard.begin();
Serial.begin(9600);
}
void loop() {
// put your main code here, to run repeatedly:
value = analogRead(sensorPin);
Serial.println(value);
if (value > 50){
delay(1000);
value = analogRead(sensorPin);
if (value > thresh1 && value <thresh2){
  if(sensorState != releaseState){
    Keyboard.press(KEY_UP_ARROW);
    Serial.println("UP");
    //sensorState = !sensorState;
    releaseState = sensorState;
    Keyboard.releaseAll();
  }
   else{
   Keyboard.releaseAll();
   digitalWrite(LED_PIN,LOW);
  }
}
else if (value > thresh2 && value < thresh3){
  if(sensorState != releaseState){
    Keyboard.press(KEY_DOWN_ARROW);
    Serial.println("DOWN");
    //sensorState = !sensorState;
    releaseState = sensorState;
    Keyboard.releaseAll();
  }
   else{
   Keyboard.releaseAll();
   digitalWrite(LED_PIN,LOW);
  }
}
else if (value > thresh3 && value < thresh4){
  if(sensorState != releaseState){
    Keyboard.press(KEY_LEFT_ARROW);
    Serial.println("LEFT");
    //sensorState = !sensorState;
    releaseState = sensorState;
    Keyboard.releaseAll();
  }
   else{
   Keyboard.releaseAll();
   digitalWrite(LED_PIN,LOW);
  }
}
else if (value > thresh4){
  if(sensorState != releaseState){
    Keyboard.press(KEY_RIGHT_ARROW);
    Serial.println("RIGHT");
    //sensorState = !sensorState;
    releaseState = sensorState;
    Keyboard.releaseAll();
  }
  else{
   Keyboard.releaseAll();
   digitalWrite(LED_PIN,LOW);
  }
}
}
else if (value < 215){
releaseState = !sensorState;
Serial.println("LED HIGH");
digitalWrite(LED_PIN,HIGH);
}
delay(300);
}
