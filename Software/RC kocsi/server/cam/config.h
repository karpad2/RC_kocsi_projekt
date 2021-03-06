//
// Created by KArpi on 2019. 10. 22..
//

#ifndef RC_KOCSI_CONFIG_H
#define RC_KOCSI_CONFIG_H

/* Put your SSID & Password */
const char* ssid = "RC-kocsi";  // Enter SSID here
const char* password = "12345678";  //Enter Password here
const char* host = "RC-kocsi";
/* Put IP Address details */
IPAddress local_ip_server(192,168,1,1);
IPAddress local_ip_camera(192,168,1,200);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

int steering=90;
int speed=0;
int webPort=80;
int serialPort=115200;
int servopin=5;
int motor_pin1=16;
int motor_pin2=17;
int motor_speed_k=10;
int front_ultrasonic_echo=11;
int ultrasonic_trigg=12;
int min_of_steering=75;
int steering_dist=37;
int max_of_steering=min_of_steering+steering_dist;
int center_of_steering=(max_of_steering-steering_dist/2);
int servo_channel=1;
int motor_pin1_channel=2;
int motor_pin2_channel=3;

#endif //RC_KOCSI_CONFIG_H
