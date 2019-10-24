
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#define FILESYSTEM SPIFFS
#define FORMAT_FILESYSTEM false
#define DBG_OUTPUT_PORT Serial

#if FILESYSTEM == FFat
#include <FFat.h>
#endif
#if FILESYSTEM == SPIFFS
#include <SPIFFS.h>
#endif
#include "config.h"
#include "filesystem.h"


WebServer server(webPort);
File fsUploadFile;

void setup(void) {
    DBG_OUTPUT_PORT.begin(115200);
    DBG_OUTPUT_PORT.print("\n");
    DBG_OUTPUT_PORT.setDebugOutput(true);
    if (FORMAT_FILESYSTEM) FILESYSTEM.format();
    FILESYSTEM.begin();
    {
        File root = FILESYSTEM.open("/");
        File file = root.openNextFile();
        while(file){
            String fileName = file.name();
            size_t fileSize = file.size();
            DBG_OUTPUT_PORT.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
            file = root.openNextFile();
        }
        DBG_OUTPUT_PORT.printf("\n");
    }


    //WIFI INIT
    DBG_OUTPUT_PORT.printf("Connecting to %s\n", ssid);
    if (String(WiFi.SSID()) != String(ssid)) {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
    }

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        DBG_OUTPUT_PORT.print(".");
    }
    DBG_OUTPUT_PORT.println("");
    DBG_OUTPUT_PORT.print("Connected! IP address: ");
    DBG_OUTPUT_PORT.println(WiFi.localIP());

    MDNS.begin(host);
    DBG_OUTPUT_PORT.print("Open http://");
    DBG_OUTPUT_PORT.print(host);
    DBG_OUTPUT_PORT.println(".local/edit to see the file browser");


    //SERVER INIT
    //list directory
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
    //first callback is called after the request has ended with all parsed arguments
    //second callback handles file uploads at that location
    server.on("/edit", HTTP_POST, []() {
        server.send(200, "text/plain", "");
    }, handleFileUpload);

    server.onNotFound([]() {
        if (!handleFileRead(server.uri())) {
            server.send(404, "text/plain", "FileNotFound");
        }
    });

    //get heap status, analog input value and all GPIO statuses in one json call
    server.on("/all", HTTP_GET, []() {
        String json = "{";
        json += "\"heap\":" + String(ESP.getFreeHeap());
        json += ", \"analog\":" + String(analogRead(A0));
        json += ", \"gpio\":" + String((uint32_t)(0));
        json += "}";
        server.send(200, "text/json", json);
        json = String();
    });
    server.on("/moving", HTTP_GET, []() {
        String json = "{";
        json += "\"heap\":" + String(ESP.getFreeHeap());
        json += ", \"analog\":" + String(analogRead(A0));
        json += ", \"gpio\":" + String((uint32_t)(0));
        json += "}";
        server.send(200, "text/json", json);
        json = String();
    });

    server.begin();
    DBG_OUTPUT_PORT.println("HTTP server started");

}

void loop(void) {
    server.handleClient();
}




bool handleFileRead(String path) {
    DBG_OUTPUT_PORT.println("handleFileRead: " + path);
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
        DBG_OUTPUT_PORT.print("handleFileUpload Name: "); DBG_OUTPUT_PORT.println(filename);
        fsUploadFile = FILESYSTEM.open(filename, "w");
        filename = String();
    } else if (upload.status == UPLOAD_FILE_WRITE) {
        //DBG_OUTPUT_PORT.print("handleFileUpload Data: "); DBG_OUTPUT_PORT.println(upload.currentSize);
        if (fsUploadFile) {
            fsUploadFile.write(upload.buf, upload.currentSize);
        }
    } else if (upload.status == UPLOAD_FILE_END) {
        if (fsUploadFile) {
            fsUploadFile.close();
        }
        DBG_OUTPUT_PORT.print("handleFileUpload Size: "); DBG_OUTPUT_PORT.println(upload.totalSize);
    }
}

void handleFileDelete() {
    if (server.args() == 0) {
        return server.send(500, "text/plain", "BAD ARGS");
    }
    String path = server.arg(0);
    DBG_OUTPUT_PORT.println("handleFileDelete: " + path);
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
    DBG_OUTPUT_PORT.println("handleFileCreate: " + path);
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
    DBG_OUTPUT_PORT.println("handleFileList: " + path);


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


