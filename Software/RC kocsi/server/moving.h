//
// Created by KArpi on 2019. 10. 24..
//


#ifndef RC_KOCSI_MOVING_H
#define RC_KOCSI_MOVING_H
#define FREQ 30000
#define RESOLUTION 8
#include "config.h"
#include <Servo.h>
int myspeed=0,previous_speed=0;
int abs_speed;
Servo servo;


void moving_setup()
{
    servo.attach(servopin);
    pinMode(motor_pin1,OUTPUT);
    pinMode(motor_pin2,OUTPUT);
    ledcSetup(motor_pin1_channel,FREQ,RESOLUTION);
    ledcSetup(motor_pin2_channel,FREQ,RESOLUTION);
    ledcAttachPin(motor_pin1,motor_pin1_channel);
    ledcAttachPin(motor_pin2,motor_pin2_channel);
}


void set_steering()
{   Serial.println("");
    Serial.print("Servo started:\t");
    if(steering>min_of_steering&&steering<max_of_steering) {
        servo.write(steering);
        Serial.println(ok);
        if(steering<center_of_steering) steering++;
        else if (steering>center_of_steering) steering--;
        else steering=center_of_steering;
    }
}
void motor_pwm(int _motorpin,int _speed)
{   int tmp=0,tmp1=0;
    if(_motorpin==motor_pin1) tmp=motor_pin1_channel;
    else tmp=motor_pin2_channel; 
       if(_speed==HIGH) tmp1=255;
       else if(_speed==LOW) tmp1=0;
        else
        {
        if(_speed>=255) tmp1=255;
        else tmp1 = _speed;
        }
        ledcWrite(tmp,tmp1);
}


void dc_control()
{
     Serial.println("");
     Serial.print("DC motor started:\t");
        if(speed==0)
        {
            motor_pwm(motor_pin1,HIGH);
            motor_pwm(motor_pin2,HIGH);
        }
        else if(speed>0)
        {
            motor_pwm(motor_pin1,HIGH);
            motor_pwm(motor_pin2,speed);   
        speed--;
        }
        else if(speed<0)
        {
            int abs_speed=speed*-1;
            motor_pwm(motor_pin1,abs_speed);
            motor_pwm(motor_pin2,LOW);   
            speed++;
            }
            else {}

    Serial.println(ok);
}




void moving()
{
    //Serial.println("Moving with speed:"+speed+",steering:"+ steering);
    set_steering();
    dc_control();
}





#endif //RC_KOCSI_MOVING_H
