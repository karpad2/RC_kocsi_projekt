//
// Created by KArpi on 2019. 10. 24..
//


#ifndef RC_KOCSI_MOVING_H
#define RC_KOCSI_MOVING_H
#include <Servo.h>
#include "config.h"
Servo myservo;
int abs_speed;
void moving_setup()
{
    myservo.attach(servopin);

}
void moving(int steering,int speed)
{
    myservo.write(servopin);
    if(speed==0)
    {
        //For brake
        digitalWrite(motor_pin1,HIGH) ;
        digitalWrite(motor_pin2,HIGH) ;
    }
    else if(speed>0)
    {
        digitalWrite(motor_pin1,LOW) ;
        analogWrite(motor_pin2,(int)speed>255?255:speed);
    }
    else if(speed<0)
    {
        abs_speed=0-speed;
        digitalWrite(motor_pin2,LOW) ;
        analogWrite(motor_pin1,(int)abs_speed>255?255:abs_speed);
    }
}



#endif //RC_KOCSI_MOVING_H
