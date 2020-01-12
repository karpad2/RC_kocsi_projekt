//
// Created by KArpi on 2019. 10. 22..
//
#ifndef RC_KOCSI_WEBSITE_H
#define RC_KOCSI_WEBSITE_H
#include <String>
#include <WebServer.h>
#include "config.h"
#include "sonic.h"
#include "filesystem.h"


WebServer server(webPort);

String getContentType(String filename) {
    if (server.hasArg("download")) {
        return "application/octet-stream";
    } else if (filename.endsWith(".htm")) {
        return "text/html";
    } else if (filename.endsWith(".html")) {
        return "text/html";
    } else if (filename.endsWith(".css")) {
        return "text/css";
    } else if (filename.endsWith(".js")) {
        return "application/javascript";
    } else if (filename.endsWith(".png")) {
        return "image/png";
    } else if (filename.endsWith(".gif")) {
        return "image/gif";
    } else if (filename.endsWith(".jpg")) {
        return "image/jpeg";
    } else if (filename.endsWith(".ico")) {
        return "image/x-icon";
    } else if (filename.endsWith(".xml")) {
        return "text/xml";
    } else if (filename.endsWith(".pdf")) {
        return "application/x-pdf";
    } else if (filename.endsWith(".zip")) {
        return "application/x-zip";
    } else if (filename.endsWith(".gz")) {
        return "application/x-gzip";
    }
    return "text/plain";
}
bool handleFileRead(String path) {
    Serial.println("handleFileRead: " + path);
    if (path.endsWith("/")) {
        path += "index.htm";
    }
    String contentType = getContentType(path);
    String pathWithGz = path + ".gz";
    if (exists(pathWithGz) || exists(path)) {
        if (exists(pathWithGz)) {
            path += ".gz";
        }
        File file = FILESYSTEM.open(path, "r");
        server.streamFile(file, contentType);
        file.close();
        return true;
    }
    return false;
}

void handleFileUpload() {
    if (server.uri() != "/edit") {
        return;
    }
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
        String filename = upload.filename;
        if (!filename.startsWith("/")) {
            filename = "/" + filename;
        }
        Serial.print("handleFileUpload Name: "); Serial.println(filename);
        fsUploadFile = FILESYSTEM.open(filename, "w");
        filename = String();
    } else if (upload.status == UPLOAD_FILE_WRITE) {
        //Serial.print("handleFileUpload Data: "); Serial.println(upload.currentSize);
        if (fsUploadFile) {
            fsUploadFile.write(upload.buf, upload.currentSize);
        }
    } else if (upload.status == UPLOAD_FILE_END) {
        if (fsUploadFile) {
            fsUploadFile.close();
        }
        Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
    }
}

void handleFileDelete() {
    if (server.args() == 0) {
        return server.send(500, "text/plain", "BAD ARGS");
    }
    String path = server.arg(0);
    Serial.println("handleFileDelete: " + path);
    if (path == "/") {
        return server.send(500, "text/plain", "BAD PATH");
    }
    if (!exists(path)) {
        return server.send(404, "text/plain", "FileNotFound");
    }
    FILESYSTEM.remove(path);
    server.send(200, "text/plain", "");
    path = String();
}

void handleFileCreate() {
    if (server.args() == 0) {
        return server.send(500, "text/plain", "BAD ARGS");
    }
    String path = server.arg(0);
    Serial.println("handleFileCreate: " + path);
    if (path == "/") {
        return server.send(500, "text/plain", "BAD PATH");
    }
    if (exists(path)) {
        return server.send(500, "text/plain", "FILE EXISTS");
    }
    File file = FILESYSTEM.open(path, "w");
    if (file) {
        file.close();
    } else {
        return server.send(500, "text/plain", "CREATE FAILED");
    }
    server.send(200, "text/plain", "");
    path = String();
}

void handleFileList() {
    if (!server.hasArg("dir")) {
        server.send(500, "text/plain", "BAD ARGS");
        return;
    }

    String path = server.arg("dir");
    Serial.println("handleFileList: " + path);


    File root = FILESYSTEM.open(path);
    path = String();

    String output = "[";
    if(root.isDirectory()){
        File file = root.openNextFile();
        while(file){
            if (output != "[") {
                output += ',';
            }
            output += "{\"type\":\"";
            output += (file.isDirectory()) ? "dir" : "file";
            output += "\",\"name\":\"";
            output += String(file.name()).substring(1);
            output += "\"}";
            file = root.openNextFile();
        }
    }
    output += "]";
    server.send(200, "text/json", output);
}
void server_moving()
{
    if(server.hasArg("steering") && server.hasArg("speed"))
    {
        String s_steering=server.arg("steering"),sa_speed=server.arg("speed");
        steering=s_steering.toInt();
        s_speed=sa_speed.toInt();
    }
    String json = "{";
    json += ""+getDistance();
    json += "}";
    server.send(200, "text/plain", json);
}

void website_setup()
{
    Serial.print("Website:");




    server.on("/list", HTTP_GET, handleFileList);
    //load editor
    server.on("/edit", HTTP_GET, []() {
        if (!handleFileRead("/edit.htm")) {
            server.send(404, "text/plain", "FileNotFound");
        }
    });
    //create file
    server.on("/edit", HTTP_PUT, handleFileCreate);
    //delete file
    server.on("/edit", HTTP_DELETE, handleFileDelete);

    server.on("/edit", HTTP_POST, []() {
        server.send(200, "text/plain", "");
    }, handleFileUpload);

    server.onNotFound([]() {
        if (!handleFileRead(server.uri())) {
            server.send(404, "text/plain", "FileNotFound");
        }
    });

    //get heap status, analog input value and all GPIO statuses in one json call
    server.on("/moving",[](){
        Serial.println("Server_moving");
        server_moving();
    });

    server.on("/all", HTTP_GET, []() {
        String json = "{";
        json += "\"heap\":" + String(ESP.getFreeHeap());
        json += ", \"analog\":" + String(analogRead(A0));
        json += ", \"gpio\":" + String((uint32_t)(0));
        json += "}";
        server.send(200, "text/json", json);
        json = String();
    });

    Serial.println(ok);

}

  




#endif //RC_KOCSI_WEBSITE_H
