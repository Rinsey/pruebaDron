
#include <Adafruit_GPS.h>
#include <Adafruit_HMC5883_U.h>
#include <Wire.h>
#include <math.h>

#define GPSSerial Serial1
#define GPSECHO false
#define IMUECHO true
#define HMCECHO true

uint32_t timer1 = millis();
uint32_t timer2 = millis();

SerialPIO reyaxSerial(10,11);

Adafruit_HMC5883_Unified mag;
Adafruit_GPS GPS(&GPSSerial);

float scaler;
boolean scaler_flag = false;
float normal_vector_length;
sensors_event_t event; 
float xv, yv, zv;
float calibrated_values[3];

float headingDegrees;
float latitud, longitud, latitudDegs, longitudDegs;
float latObj, lonObj, distObj;
int sats;

String valoresRaspi[2],telemetria, sentencia;
float distanciaRaspi, anguloRaspi;

void setup() {
  Serial.begin(9600);
  reyaxSerial.begin(115200);
}

void loop() {

  if (millis() - timer1 > 20) {
    timer1 = millis();
    brujula();
  }

  if(Serial.available()){
    sentencia = Serial.readString();
    separador(sentencia);
    distanciaRaspi = valoresRaspi[0].toFloat();
    anguloRaspi = valoresRaspi[1].toFloat();
  }
}

void loop1() {
  gps();
  sumarDistancia(latitudDegs, longitudDegs, distanciaRaspi, anguloRaspi, latObj, lonObj);
  telemetria = String(latitudDegs) + "," + String(longitudDegs) + "," + String(latObj) + "," + String(lonObj);
  loRaSender(telemetria,0);
}

void loRaSender(String sentencia, int address) {
  int size = sizeof(sentencia);
  String sentenciaLoRa = "AT+SEND="+String(address)+","+String(size)+","+sentencia;
}

void separador(String sentencia) {
  int comaIndex = 0, i = 0;
  while ((comaIndex = sentencia.indexOf(',')) != -1) {
    valoresRaspi[i] = sentencia.substring(0, comaIndex);
    sentencia = sentencia.substring(comaIndex + 1);
  }
}
