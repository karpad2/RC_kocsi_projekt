//
// Created by KArpi on 2019. 10. 25..
//

#ifndef RC_KOCSI_WIFI_CONTROL_H
#define RC_KOCSI_WIFI_CONTROL_H

#include <WiFi.h>
#include "config.h"

void wifi_setup()
{
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    MDNS.begin(host);
    Serial.print("Open http://");
    Serial.print(host);
    Serial.println(".local/edit to see the file browser");
}


#endif //RC_KOCSI_WIFI_CONTROL_H
