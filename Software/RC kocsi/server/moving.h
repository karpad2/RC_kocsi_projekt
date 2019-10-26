//
// Created by KArpi on 2019. 10. 24..
//


#ifndef RC_KOCSI_MOVING_H
#define RC_KOCSI_MOVING_H
#include <ESP32_Servo.h>
#include <analogWrite.h>
#include "config.h"
#include "sonic.h"
#include <Math.h>
#include "website.h"

double myspeed=0;
Servo myservo;
int abs_speed;
void moving_setup()
{   analogWriteResolution(motor_pin1, 12);
    analogWriteResolution(motor_pin2, 12);

    myservo.attach(servopin,500,2400);
}
double calcmyspeed()
{
    double dist1=getDistance();
    long previous=millis();
    while(previous-millis()<50){};
    return abs(getDistance()-dist1)/(millis()-previous);
}
void set_steering()
{
    if(steering>min_of_steering&&steering<max_of_steering) {
        myservo.write(steering);
        if(steering<center_of_steering) steering++;
        else if (steering>center_of_steering) steering--;
        else steering=center_of_steering;
    }
}
void pwmWrite(int pin,int pwm)
{
    if(pwm>=255)
    {
        analogWrite(pin,255);
    }
    else
        analogWrite(pin,pwm);
}

void moving()
{   set_steering();
    if(speed==0)
    {
        //For brake
        digitalWrite(motor_pin1,HIGH) ;
        digitalWrite(motor_pin2,HIGH) ;
    }
    else if(speed>0)
    {
        if(getDistance()<15) {speed=0;}
        else{
            digitalWrite(motor_pin1,LOW) ;
            pwmWrite(motor_pin2,speed);
            speed--;

        }
    }
    else if(speed<0)
    {
        abs_speed=0-speed;
        digitalWrite(motor_pin2,LOW) ;
        pwmWrite(motor_pin1,abs_speed);
        speed++;
    }

}





#endif //RC_KOCSI_MOVING_H
