#include <BleKeyboard.h>
#include "Arduino.h"
#define pin0 4
#define pin1 18
#define pin2 19
#define ADCPin 34
hw_timer_t *timer=NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
BleKeyboard bleKeyboard("music_controler","RYAN",95);
char a,b,s,t=0;
void IRAM_ATTR Timer_Hander(){
portENTER_CRITICAL_ISR(&timerMux);
if(s)
{
if(a!=digitalRead(pin1)||b!= digitalRead(pin2))
{
  if(a==b)
  {
  if(a)
  {
    if(digitalRead(pin1))bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
    else bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
    }
  else 
  {
    if(digitalRead(pin2))bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
    else bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
    }
    t=0;
}
  a = digitalRead(pin1);
b = digitalRead(pin2);
  }
}
else{
  if(a!=digitalRead(pin1)||b!= digitalRead(pin2))
{
  if(a==b)
  {
  if(a)
  {
    if(digitalRead(pin1))bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
    else bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
    }
  else 
  {
    if(digitalRead(pin2))bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
    else bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
    }
}
  a = digitalRead(pin1);
b = digitalRead(pin2);
  }
  }
portEXIT_CRITICAL_ISR(&timerMux);
xSemaphoreGiveFromISR(timerSemaphore, NULL);
}
void isr(){
  if(digitalRead(pin0)==0)
  {s=1;t=1;    
    }
  else 
  {
    if(s==1&&t==1)bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
    s=0;t=0;
    }
  }
void setup() {
// put your setup code here, to run once:
pinMode(pin0, INPUT_PULLUP);
pinMode(pin1, INPUT_PULLUP);
pinMode(pin2, INPUT_PULLUP);
//adcAttachPin(ADCPin);
//adcStart(ADCPin);
timerSemaphore=xSemaphoreCreateBinary();
timer = timerBegin(0, 80, true);
// 配置定时器中断函数
timerAttachInterrupt(timer, &Timer_Hander, true);
// Set alarm to call onTimer function every second (value in microseconds).
// Repeat the alarm (third parameter)
timerAlarmWrite(timer, 10000, true);
// Start an alarm
a = digitalRead(pin1);
b = digitalRead(pin2);
//bleKeyboard.batteryLevel=(100*(analogRead(ADCPin)-1861))/745;
bleKeyboard.begin();

}
void loop() {
  if(bleKeyboard.isConnected()) {
timerAlarmEnable(timer);//使能定时器函数
attachInterrupt(pin0, isr, CHANGE );
char i = 0;
while(1);
  }
  delay(5000);
}
