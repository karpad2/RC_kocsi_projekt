//
// Created by KArpi on 2019. 10. 24..
//


#ifndef RC_KOCSI_MOVING_H
#define RC_KOCSI_MOVING_H
#define COUNT_LOW 0
 #define COUNT_HIGH 8888
 #define TIMER_WIDTH 16
//#include <ESP32_Servo.h>
//#include <analogWrite.h>
#include "config.h"
//#include "website.h"
#include "esp32-hal-ledc.h"

int myspeed=0;
int abs_speed;

void moving_setup()
{
    ledcSetup(servo_channel, 50, TIMER_WIDTH); // channel 1, 50 Hz, 16-bit width
    ledcSetup(motor_pin1_channel, 50, TIMER_WIDTH);
    ledcSetup(motor_pin2_channel, 50, TIMER_WIDTH);
    ledcAttachPin(servopin, servo_channel);   // GPIO 22 assigned to channel 1
    ledcAttachPin(motor_pin1,motor_pin1_channel);
    ledcAttachPin(motor_pin2,motor_pin2_channel);
}


void set_steering()
{
    if(steering>min_of_steering&&steering<max_of_steering) {
        ledcWrite(1, (steering));
        if(steering<center_of_steering) steering++;
        else if (steering>center_of_steering) steering--;
        else steering=center_of_steering;
    }
}
void moving()
{
    //Serial.println("Moving with speed:"+speed+",steering:"+ steering);
    set_steering();
    if(speed==0)
    {
        //For brake
        ledcWrite(motor_pin1_channel, 100);
        ledcWrite(motor_pin2_channel, 100);
    }
    /*
    else if(speed>0)
    {
        if(getDistance()<15) {speed=0;}
        else{
            digitalWrite(motor_pin1,LOW) ;
            //pwmWrite(motor_pin2,speed);
            speed--;

        }
    }
    else if(speed<0)
    {
        abs_speed=0-speed;
        digitalWrite(motor_pin2,LOW) ;
        //pwmWrite(motor_pin1,abs_speed);
        speed++;
    }*/

}





#endif //RC_KOCSI_MOVING_H
