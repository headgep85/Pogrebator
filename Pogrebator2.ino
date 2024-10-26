#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

#include <ArduinoOTA.h> //OTA Библиотека для OTA-прошивки

#define ONE_WIRE_BUS D2
#define RELE_SWITCH D5 // Устанавливаем управляющий пин для реле
int temp_pogreb;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

char auth[] = "tokken";
char ssid[] = "You'r Wi-fi";
char pass[] = "You'r pwd";


void setup() 
{
Serial.begin(115200);
Blynk.begin(auth, ssid, pass);

 //************************
  ArduinoOTA.setHostname("Pogreb"); //OTA Задаем имя сетевого порта
  //ArduinoOTA.setPassword((const char *)"0000"); //OTA Задаем пароль доступа для удаленной прошивки
  ArduinoOTA.begin(); //OTA Инициализируем OTA
  //***********************
  
sensors.begin();

pinMode (RELE_SWITCH,OUTPUT);
}

BLYNK_WRITE(V5) {   

  temp_pogreb = param.asInt(); // Gets the value stored in V5 as an integer

}

void sendTemps()
{
sensors.requestTemperatures();
float temp = sensors.getTempCByIndex(0); 
float temp1 = sensors.getTempCByIndex(1);
Serial.println(temp);
Serial.println(temp1);
Blynk.virtualWrite(V1, temp);
Blynk.virtualWrite(V2, temp1);

if (int(temp1) < temp_pogreb) {
    digitalWrite(RELE_SWITCH, HIGH); // Включить реле
    Blynk.virtualWrite(V0,HIGH);
    Blynk.setProperty(V0,"onLabel","ON");
}
  else {digitalWrite(RELE_SWITCH, LOW); Blynk.virtualWrite(V0,LOW);
    Blynk.setProperty(V0,"onLabel","OFF");} 
}

void loop() 
{
ArduinoOTA.handle(); // OTA Всегда готовы к прошивке  
Blynk.run();
sendTemps();
}
