
#include <WebServer.h>
#include <ESPmDNS.h>

#include "config.h"
#include "filesystem.h"
#include "wifi-control.h"
#include "website.h"
#include "moving.h"


void moving();
void setup(void) {
    Serial.begin(115200);
    Serial.print("\n");

    check_fs();
    //WIFI INIT
    wifi_setup();

    //SERVER INIT
    website_setup();


    server.begin();
    Serial.println("HTTP server started");
}

void loop(void) {
    server.handleClient();
   // moving();
}
