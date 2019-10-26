//
// Created by KArpi on 2019. 10. 22..
//

#ifndef RC_KOCSI_SONIC_H
#define RC_KOCSI_SONIC_H
#include <Ultrasonic.h>
#include "config.h"
#include <String.h>

Ultrasonic front_ultrasonic(ultrasonic_trigg, front_ultrasonic_echo);

String getFrontSensorStringDistance()
{
  
    return ""+String(front_ultrasonic.read());
}
double getDistance()
{
    return  front_ultrasonic.read();
}




#endif //RC_KOCSI_SONIC_H
