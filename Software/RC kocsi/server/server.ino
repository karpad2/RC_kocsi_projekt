
#include <WebServer.h>
#include <ESPmDNS.h>

#include "config.h"
#include "filesystem.h"
#include "wifi-control.h"
#include "website.h"
#include "moving.h"


void moving();
void setup(void) {
    Serial.begin(serialPort);
    Serial.print("\n Filesystem:");
    check_fs();
    //WIFI INIT
    Serial.println("[OK]");
    Serial.print("Wifi:");
    wifi_setup();
    Serial.println("[OK]");
    //SERVER INIT
    Serial.print("Website:");
    website_setup();
    Serial.println("[OK]");
    Serial.print("Server:");
    server.begin();
    Serial.println("[OK]");
    Serial.print("Moving:");
    moving_setup();
    Serial.println("[OK]");
    Serial.println("System started!");
}
void loop(void) {
    server.handleClient();
    moving();
    delay(10);
}
