SerialPIO reyax(10,11);
String reyaxReceived[5];

float lat=0.0,lon=0.0,speed=0.0;

#include <Adafruit_Sensor.h>
#include <Adafruit_GPS.h>
#include <Wire.h>

#define GPSSerial Serial1
#define GPSECHO true


int counter=0;
uint32_t timer1 = millis();
uint32_t timer2 = millis();
Adafruit_GPS GPS(&GPSSerial);

void setup() {
  reyax.begin(115200);
  Serial.begin(115200);
  while(!Serial) {}
 GPS.begin(9600);
}

void loop() {

  char c = GPS.read();
    if (GPSECHO){
      if (c) Serial.print(c);
    }
    if (GPS.newNMEAreceived()) {
      if (!GPS.parse(GPS.lastNMEA()))
        return;
    }
 if (millis() - timer2 > 1000) {
      timer2 = millis();
      if (GPS.fix) {
     
        lat=(GPS.latitude); 
        lon=(GPS.longitude); 
        speed=(GPS.speed);
        
        counter++;   
      }
    }

  if(Serial.available()>0){
    String message = Serial.readStringUntil('\n');
   
   String datos=message+","+(String)lat+","+(String)lon+","+(String)speed;
   loRaSender(datos,1);
   //delay(10); 
  }
}


void loRaSender(String sentencia, int address) {
  int tam=sentencia.length();
  String sentenciaLoRa = "AT+SEND="+String(address)+","+String(tam)+","+sentencia;
  reyax.println(sentenciaLoRa);
}
