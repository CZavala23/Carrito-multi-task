#include "Sensor_TCRT5000.h"
#include <Arduino.h>

// --- Configura el pin del sensor ---
void Sensor::Inicializar()
{
  pinMode(Pin_sensor, INPUT);
}

// --- Realiza una lectura digital del pin ---
int Sensor::Leer_sensor()
{
  return digitalRead(Pin_sensor);
}