#define FILESYSTEM SPIFFS
#define FORMAT_FILESYSTEM false
#define FREQ 50
#define RESOLUTION 8


#include <String.h>
#include <SPIFFS.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <WiFi.h>
#include <Servo.h>
#include <Ultrasonic.h>

/* Put your SSID & Password */
const char *ssid = "RC-kocsi";  // Enter SSID here
const char *password = "12345678";  //Enter Password here
const char *host = "RC-kocsi";
const char *ok = "[OK]";
/* Put IP Address details */
IPAddress local_ip_server(192, 168, 1, 1);
IPAddress local_ip_camera(192, 168, 1, 200);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

int c_steering = 95;
int steering=0;
int s_speed = 0;
int webPort = 80;
int serialPort = 115200;
int servopin = 4;
int motor_pin1 = 0;
int motor_pin2 = 2;
int motor_speed_k = 10;
int ultrasonic_echo = 11;
int ultrasonic_trigg = 13;
int min_of_steering = 75;
int steering_dist = 15;
int servo_channel = 1;
int motor_pin1_channel = 2;
int motor_pin2_channel = 3;
Servo servo;
File fsUploadFile;
WebServer server(webPort);
String reverse_text;
String s_steering = "";
String sa_speed = "";
String json = "";
Ultrasonic ultrasonic(27, 26);
void wifi_setup();

void moving_setup();

void set_steering();

int getDistance();

void motor_pwm(int _motorpin, int _speed);

void dc_control();

void d_moving();

String getContentType(String filename);

bool handleFileRead(String path);

void handleFileUpload();

void handleFileDelete();

void handleFileCreate();

void handleFileList();

void server_moving();

void website_setup();

void variables_setup() {
    reverse_text = "";
    s_steering = "";
    sa_speed = "";
    json = "";
    pinMode(servopin,OUTPUT);
    pinMode(motor_pin1,OUTPUT);
    pinMode(motor_pin2,OUTPUT);
}




void setup(void) {
    variables_setup();
    Serial.begin(serialPort);
    Serial.print("\n Filesystem:");
    check_fs();
    //WIFI INIT
    Serial.println(ok);
    website_setup();
    Serial.print("Wifi:");
    wifi_setup();
    Serial.println(ok);
    Serial.print("Moving:");
    moving_setup();
    Serial.println(ok);
    Serial.print("Server:");
    server.begin();
    Serial.println(ok);
    Serial.println("System started!");
}

void loop(void) {

    server.handleClient();
    //dc_control();
    server_moving();
    }

void wifi_setup() {
    WiFi.enableAP(true);
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip_server, gateway, subnet);
    MDNS.begin(host);
    Serial.print("Open http://");
    Serial.print(WiFi.softAPIP());
    Serial.println("/edit to see the file browser");
}

void moving_setup() {
    servo.attach(
            servopin,
            servo_channel,
            90,
            100);
    ledcSetup(motor_pin1_channel, FREQ, RESOLUTION);
    ledcSetup(motor_pin2_channel, FREQ, RESOLUTION);
    ledcAttachPin(motor_pin1, motor_pin1_channel);
    ledcAttachPin(motor_pin2, motor_pin2_channel);
}

void dc_control() {
    if(steering>=4) steering=4;
    else if(steering<=-3) steering=-3;
    servo.write(c_steering+steering);
    delay(5);
    if (s_speed == 0) {
        ledcWrite(motor_pin1_channel,0);
        delay(5);
        ledcWrite(motor_pin2_channel,0);
        delay(5);
    } else if (s_speed > 0) {
        ledcWrite(motor_pin1_channel,(s_speed*8)<=255?s_speed*8:255);
        delay(5);
        ledcWrite(motor_pin2_channel,0);
        delay(5);

    } else  {
        int abs_speed=s_speed*(-1);
        ledcWrite(motor_pin1_channel,0);
        delay(5);
        ledcWrite(motor_pin2_channel,(abs_speed*8)<=255?(abs_speed*8):255);
        delay(5);
    }

     Serial.println("Moving with:"+String(s_speed)+","+String(steering));
    delay(5);
}



int getDistance() {
    //return  front_ultrasonic.read();
    return ultrasonic.read();
}


String formatBytes(size_t bytes);

void check_fs() {
    if (FORMAT_FILESYSTEM) FILESYSTEM.format();
    FILESYSTEM.begin();
    {
        File root = FILESYSTEM.open("/");
        File file = root.openNextFile();
        while (file) {
            String fileName = file.name();
            size_t fileSize = file.size();
            Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
            file = root.openNextFile();
        }
        Serial.printf("\n");
    }
}

bool exists(String path) {
    bool yes = false;
    File file = FILESYSTEM.open(path, "r");
    if (!file.isDirectory()) {
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
    } else {
        return String(bytes / 1024.0 / 1024.0) + "MB";
    }
}

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
        path += "index.html";
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
    HTTPUpload &upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
        String filename = upload.filename;
        if (!filename.startsWith("/")) {
            filename = "/" + filename;
        }
        Serial.print("handleFileUpload Name: ");
        Serial.println(filename);
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
        Serial.print("handleFileUpload Size: ");
        Serial.println(upload.totalSize);
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
    if (root.isDirectory()) {
        File file = root.openNextFile();
        while (file) {
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

void server_moving() {
    json = "{";
    json += "\"heap\":" + String(ESP.getFreeHeap());
    json += ", \"distance\":" + String(getDistance());
    json += "}";
    reverse_text = json;
    json=String();
}

void website_setup() {
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
    server.on("/moving", []() {
        server.send(200, "text/plain", reverse_text);
        if  (server.hasArg("speed") && server.hasArg("steering")) {
            s_steering=server.arg("steering");
            sa_speed = server.arg("speed");
            steering = s_steering.toInt();
            s_speed = sa_speed.toInt();
            Serial.println("Intreperted..."+s_steering+","+sa_speed+";integer:"+ String(s_speed)+", "+ String(steering));
            s_steering=String();
            sa_speed=String();
            dc_control();
            server_moving();
        }
        //console="Server_moving, speed:" + String(s_speed) + ", steering:" + String(steering));
        delay(10);
    });

    server.on("/getservo", []() {
        String servo_text="";
        servo_text+="Servo angle:";
        servo_text+=String(servo.read());
        server.send(200, "text/plain", servo_text);
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
