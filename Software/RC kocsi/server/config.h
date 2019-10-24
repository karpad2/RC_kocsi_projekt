//
// Created by KArpi on 2019. 10. 22..
//

#ifndef RC_KOCSI_CONFIG_H
#define RC_KOCSI_CONFIG_H

/* Put your SSID & Password */
const char* ssid = "RC-kocsi";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

int webPort=80;
int serialPort=115200;
int servopin=6;
int motor_pin1=7;
int motor_pin2=8;
int motor_speed_k=10;



const char* host = "RC-kocsi";

#endif //RC_KOCSI_CONFIG_H
