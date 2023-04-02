/*
float distanciaObjetivo(float lat1, float lon1, float lat2, float lon2) { 
  
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

*/
/*  Función para calcular Heading  */

//#include <math.h> //Es importante añadir esta lib para reconocer M_PI y las funciones trigonométricas
/*
float headingObjetivo(float lat1, float lon1, float lat2, float lon2) { 
  
  // Calculamos la diferencia entre las coordenadas
  float dLat = lat2 - lat1;
  float dLon = lon2 - lon1;
  
  // Calculamos el heading utilizando la fórmula de la dirección inicial
  float y = sin(dLon) * cos(lat2);
  float x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon);
  
  h = atan2(y, x) * 180 / M_PI;
  
  if (h < 0) {
    return h += 360;
  }
  
  return h;
}
*/
/* Función para calcular el nuevo par de coordenadas a partir de distancia (metros) y ángulo */

/*         
    para mandar parametros a esta funcion se deben utilzar punteros de la siguiente forma
  esto para poder "retornar" varios valores al mismo tiempo sin alargar el código usando estructuras (structs)
  --- sumarDistancia(latitud, longitud, angulo, &latitud2, &longitud2); ---
  cuidar el tipo de dato que se manda, pues puede ocasionar errores.
*/

void sumarDistancia(float latitud, float longitud, float distancia, float angulo, float lat2, float lon2) {
  
  const float R = 6371000; // radio de la Tierra en metros

  // Convertir el ángulo a radianes
  angulo = angulo * M_PI / 180.0;

  // Calcular la nueva latitud y longitud, modificando el valor de los punteros
  latObj = asin(sin(latitud) * cos(distancia / R) + cos(latitud) * sin(distancia / R) * cos(angulo));
  lonObj = longitud + atan2(sin(angulo) * sin(distancia / R) * cos(latitud), cos(distancia / R) - sin(latitud) * sin(latObj));

}
