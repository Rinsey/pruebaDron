void brujula(){
  getMagData();  

  float heading = atan2(yv, xv);
  float declinationAngle = 0.185;
  heading += declinationAngle;

  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;

  headingDegrees = heading * 180/M_PI; 
}

void gps(){
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
    sats = (int)GPS.satellites;
    if (GPS.fix) {
      latitud = GPS.latitude;
      longitud = GPS.longitude;
      convCords();
      distObj = distanciaObj(latitudDegs, longitudDegs, latObj, lonObj);
      convCords();
    }
  }
}

void convCords(){
 //Initialize the location.
 float lat = latitud;
 float lon = longitud;
 // Get the first two digits by turning f into an integer, then doing an integer divide by 100;
 int firsttwoLat = ((int)lat)/100; //This assumes that f < 10000.
 int firsttwoLon = ((int)lon)/100; //This assumes that f < 10000.
 float nexttwoLat = lat - (float)(firsttwoLat*100);
 float nexttwoLon = lon - (float)(firsttwoLon*100);
 latitudDegs=(float)(firsttwoLat + nexttwoLat/60.0);
 longitudDegs=-(float)(firsttwoLon + nexttwoLon/60.0);
}

float distanciaObj(float lat1, float lon1, float lat2, float lon2) { 
  
  // Radio de la Tierra en metros
  const float R = 6371000;

  // Convertimos las coordenadas a radianes
  lat1 = lat1 * M_PI / 180;
  lon1 = lon1 * M_PI / 180;
  lat2 = lat2 * M_PI / 180;
  lon2 = lon2 * M_PI / 180;

  // Calculamos la diferencia entre las coordenadas
  float dLat = lat2 - lat1;
  float dLon = lon2 - lon1;

  // Aplicamos la fórmula de Haversine
  float a = sin(dLat/2) * sin(dLat/2) + cos(lat1) * cos(lat2) * sin(dLon/2) * sin(dLon/2);
  float c = 2 * atan2(sqrt(a), sqrt(1-a));
  float d = R * c;

  return d; //Mandamos la distancia de regreso 
}

void getMagData(){
  mag.getEvent(&event);

  xv=event.magnetic.x;
  yv=event.magnetic.y;
  zv=event.magnetic.z;
}

void transformation(float uncalibrated_values[3])    {
  double calibration_matrix[3][3] = {
    {1.315,-0.036,-0.115},
    {0.033,1.358,0.008},
    {0.150,0.022,1.509}  
  };

  double bias[3] = {
    -8.387,
    -2.62,
    -15.539
  };  

  for (int i=0; i<3; ++i) uncalibrated_values[i] = uncalibrated_values[i] - bias[i];
  float result[3] = {0, 0, 0};
  for (int i=0; i<3; ++i)
    for (int j=0; j<3; ++j)
      result[i] += calibration_matrix[i][j] * uncalibrated_values[j];
  for (int i=0; i<3; ++i) calibrated_values[i] = result[i];
}

void vector_length_stabilasation(){
  if (scaler_flag == false){
    getMagData();
    normal_vector_length = sqrt(calibrated_values[0]*calibrated_values[0] + calibrated_values[1]*calibrated_values[1] + calibrated_values[2]*calibrated_values[2]);
    scaler_flag = true;
  } 

  scaler = normal_vector_length/sqrt(calibrated_values[0]*calibrated_values[0] + calibrated_values[1]*calibrated_values[1] + calibrated_values[2]*calibrated_values[2]);
  calibrated_values[0] = calibrated_values[0]*scaler;
  calibrated_values[1] = calibrated_values[1]*scaler;
  calibrated_values[2] = calibrated_values[2]*scaler;
}
