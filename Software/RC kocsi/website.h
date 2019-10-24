//
// Created by KArpi on 2019. 10. 22..
//
#ifndef RC_KOCSI_WEBSITE_H
#define RC_KOCSI_WEBSITE_H
#include <String>
#include <EEPROM.h>

String website_page="<!DOCTYPE html> <html lang=\"en\"> <head> <style> *{font-family:Roboto,Arial,sans-serif;color:#d2d3d7} body{background-color:#313445;} .main_block{ display: block; text-align: center; margin: 0 auto; box-shadow: 0px 0px 40px #232430; padding: 20px 40px 80px; width: 60%; transition: ease-in-out 1s; position: relative; } .control_buttons { margin: 0.2em; font-size: 2em; width: 2em; height: 2em; border-radius: 10px; border: black solid 1px; } .control_buttons:hover { background-color: #f06952; box-shadow: 0 0 10px #f06952; border-radius: 10px; } i { border: solid #d2d3d7;; border-width: 0 3px 3px 0; display: inline-block; padding: 3px; } .right { transform: rotate(-45deg); -webkit-transform: rotate(-45deg); } .left { transform: rotate(135deg); -webkit-transform: rotate(135deg); } .up { transform: rotate(-135deg); -webkit-transform: rotate(-135deg); } .down { transform: rotate(45deg); -webkit-transform: rotate(45deg); } .cam_image { width: 90%; height: 72%; border:1px white solid; display: block; margin: 0.8em; } </style>"+
                    "<meta charset=\"UTF-8\"> <title>RC Car</title> <script> const kp = 2,ki=5,kd=1,con=2,timerupdate=10; let currentTime, previousTime,elapsedTime,error,lastError,input, output, setPoint,cumError, rateError,dist_sensor=0; let speed=0,steering=60;//60-105 let flagup,flagdown,flagleft,flagright; let canvas,ctx; document.onreadystatechange = () => { if (document.readyState === 'complete') { canvas = document.getElementById(\"myCanvas\"); ctx = canvas.getContext(\"2d\"); setPoint=0; steering=60; speed=0; flagup=false,flagdown=false,flagleft=false,flagright=false; ctx.fillStyle = \"#d2d3d7\"; ctx.textAlign = \"center\"; setInterval(drawing,timerupdate); } }; function computePID(inp){ currentTime = Date().getMilliseconds(); //get current time elapsedTime = (currentTime - previousTime); //compute time elapsed from previous computation error = Setpoint - inp; // determine error cumError += error * elapsedTime; // compute integral rateError = (error - lastError)/elapsedTime; // compute derivative let out = kp*error + ki*cumError + kd*rateError; //PID output lastError = error; //remember current error previousTime = currentTime; //remember current time return out; //have function return the PID output } window.setInterval(loadDoc, timerupdate); window.addEventListener('keydown',keyDownHandler); window.addEventListener('keyup',keyupHandler) function keyDownHandler(event) {let keyPressed = String.fromCharCode(event.keyCode); if (keyPressed === \"W\" || event.keyCode===38) flagup=true; else if (keyPressed === \"D\" || event.keyCode===39) flagright=true; else if (keyPressed === \"S\" || event.keyCode===40) flagdown=true; else if (keyPressed === \"A\"|| event.keyCode===37) flagleft=true; } function touchingHandler(inp) {if (inp===\"U\") flagup=true; else if (inp===\"R\") flagright=true; else if (inp===\"D\") flagdown=true; else if (inp===\"L\") flagleft=true;} function moving() {if(flagup) speed+=con; else if(flagdown) speed-=con; if(flagleft) steering-=con; else if(flagright) steering+=con; if(steering>60) steering-=con/2; else if(steering<60) steering+=con/2; if(speed>0) speed-=con/2; else if(speed<0) speed+=con/2;} function keyupHandler(event) {flagup=false; flagright=false; flagdown=false; flagleft=false;} function loadDoc() {let flag=false; let xhttp = new XMLHttpRequest(); var string_array=\"\"; let array=[]; xhttp.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) flag=true; string_array=this.responseText; array=JSON.parse(string_array); }; let kicsikocsi=\"localhost\" //kicsikocsi ip c�m xhttp.open(\"GET\",kicsikocsi+\"/\"+steering+\"/\"+speed , true); xhttp.send(); moving();} function drawing() { ctx.clearRect(0, 0, canvas.width, canvas.height); ctx.font=\"30px Arial sans-serif\"; ctx.fillText(\"Kicsi Kocsi\", (canvas.width/2)+(steering-60), canvas.height/2+(-speed)); ctx.font=\"15px Arial sans-serif\"; ctx.fillText(\"Dist:\"+dist_sensor, (canvas.width-100), 20); } </script> </head> <body> <div class=\"main_block\"> <h1>RC kocsi</h1> <canvas id=\"myCanvas\" width=\"550\" height=\"450\" style=\"border:1px solid #d3d3d3;\">Your browser does not support the canvas element.</canvas> <div> <div class=\"control_buttons\" onmousedown=\"touchingHandler('L')\" onmouseup=\"keyupHandler()\" style=\"float:left;\"><i class=\"left\"></i></div> <div class=\"control_buttons\" onmousedown=\"touchingHandler('R')\" onmouseup=\"keyupHandler()\" style=\"float:left;\"><i class=\"right\"></i></div> <div class=\"control_buttons\" onmousedown=\"touchingHandler('U')\" onmouseup=\"keyupHandler()\" style=\"float:right;\"><i class=\"up\"></i></div> <div class=\"control_buttons\" onmousedown=\"touchingHandler('D')\" onmouseup=\"keyupHandler()\" style=\"float:right;\"><i class=\"down\"></i></div> </div> </div> </body> </html>";




#endif //RC_KOCSI_WEBSITE_H