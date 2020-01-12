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






/*
void set_steering()
{   //Serial.println("");
    //Serial.print("Servo started:\t");
    if(steering>min_of_steering&&steering<max_of_steering) {
        servo.write(steering);
       // Serial.println(ok);
        if(steering<center_of_steering) steering++;
        else if (steering>center_of_steering) steering--;
        else steering=center_of_steering;
    }
    delay(5);
}*/
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
     //Serial.println("");
     //Serial.print("DC motor started:\t");
        if(s_speed==0)
        {
            motor_pwm(motor_pin1,HIGH);
            motor_pwm(motor_pin2,HIGH);
        }
        else if(s_speed>0)
        {
            motor_pwm(motor_pin1,HIGH);
            motor_pwm(motor_pin2,s_speed);   
        s_speed--;
        }
        else if(s_speed<0)
        {
            int abs_speed=s_speed*-1;
            motor_pwm(motor_pin1,abs_speed);
            motor_pwm(motor_pin2,LOW);
            s_speed++;
            }
            else {}

   // Serial.println(ok);
    delay(5);
}

void d_moving()
{
    //Serial.println("Moving with speed:"+speed+",steering:"+ steering);
  //  set_steering();
    dc_control();
}





#endif //RC_KOCSI_MOVING_H
