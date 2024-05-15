#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>

//SPIFFS (obsoleto, no permite subdirectorios)
//SPI Flash File System

//LittleFS (subdirectorios)

uint16_t tcp_port = 80;
AsyncWebServer server( tcp_port );

const char sta_ssid[] = "LAB_ELECTRONICA";
const char sta_pass[] = "LabElec2022";
const char ap_ssid[]  = "TD3_AP_Test";
const char ap_pass[]  = "password";

const IPAddress ap_ip(10,10,10,10);
const IPAddress ap_gateway(10,10,10,1);
const IPAddress ap_subnet(255,255,255,0);

void home_handler(AsyncWebServerRequest *request)
{
    const char html[] = R"=====(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Title</title>
</head>
<body>
    <H1>Hola</H1><br>
    <input type="button"
           value="Click me"
           onclick="js_function()">    
    <!-- very easy -->
</body>
<script>
//javascript (C/C++)
function js_function()
{
    console.log("hello console!!");
}
</script>
<style>

</style>
</html>
    )=====";
    request->send(200,"text/html",html);
}

void home1_handler(AsyncWebServerRequest *request)
{
    request->send(SPIFFS,"/home.html");
}

void gauge_handler(AsyncWebServerRequest *request)
{
    request->send(SPIFFS,"/gauge_test.html");
}
void gauge_lib_handler(AsyncWebServerRequest *request)
{
    request->send(SPIFFS,"/gauge.min.js");
}
void ajax_handler(AsyncWebServerRequest *request)
{
    //read from sensors...
    char data[] = "23.5";
//    Serial.printf("ajax args: %d\n",request->args());
    for( int i=0 ; i<request->args() ; i++ )
    {
        String argName  = request->argName(i);
        String argValue = request->arg(i);
        Serial.printf("%s = %s\n",argName.c_str(),argValue.c_str());
    }
    if( request->hasParam("led3") )
    {
        String state = request->arg("led3");
        if( state == "on" )
            digitalWrite(2,HIGH);
        else
            digitalWrite(2,LOW);
    }
    request->send(200,"text/plain",data);
}
void style_handler(AsyncWebServerRequest *request)
{
    request->send(SPIFFS,"/style.css","text/css");
}
void setup()
{
    Serial.begin(115200);//8N1
    pinMode(2,OUTPUT);

    WiFi.softAPConfig( ap_ip , ap_gateway , ap_subnet );
    WiFi.softAP( ap_ssid , ap_pass );

    if( !SPIFFS.begin() )
        Serial.println("Error SPIFFS not working");

    server.on("/home" ,home_handler);
    server.on("/home1",home1_handler);
    server.on("/gauge",gauge_handler);
    server.on("/gauge.min.js",gauge_lib_handler);
    server.on("/ajax_test",ajax_handler);
    server.on("/style.css",style_handler);

    //server listo para recibir peticiones (de paginas web)
    server.begin();
}

void loop()
{

//    delay(1000);
//    Serial.println("Hello world!\n");
}
