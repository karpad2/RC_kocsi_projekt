//
// Created by KArpi on 2019. 10. 24..
//

#ifndef RC_KOCSI_FILESYSTEM_H
#define RC_KOCSI_FILESYSTEM_H

#define FILESYSTEM SPIFFS
#define FORMAT_FILESYSTEM false

#include "String.h"
#if FILESYSTEM == FFat
#include <FFat.h>
#endif
#if FILESYSTEM == SPIFFS
#include <SPIFFS.h>
#endif
File fsUploadFile;



String formatBytes(size_t bytes);
void check_fs()
{
    if (FORMAT_FILESYSTEM) FILESYSTEM.format();
    FILESYSTEM.begin();
    {
        File root = FILESYSTEM.open("/");
        File file = root.openNextFile();
        while(file){
            String fileName = file.name();
            size_t fileSize = file.size();
            Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
            file = root.openNextFile();
        }
        Serial.printf("\n");
    }
}

bool exists(String path){
    bool yes = false;
    File file = FILESYSTEM.open(path, "r");
    if(!file.isDirectory()){
        yes = true;
    }
    file.close();
    return yes;
}
String formatBytes(size_t bytes) {
    if (bytes < 1024) {
        return String(bytes) + "B";
    } else if (bytes < (1024 * 1024)) {
        return String(bytes / 1024.0) + "KB";
    } else if (bytes < (1024 * 1024 * 1024)) {
        return String(bytes / 1024.0 / 1024.0) + "MB";
    } else {
        return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
    }
}


#endif //RC_KOCSI_FILESYSTEM_H
