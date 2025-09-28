#ifndef SENSOR_TCRT5000_H
#define SENSOR_TCRT5000_H
#include <Arduino.h>

// --- Clase para un sensor infrarrojo TCRT5000 ---
// Encapsula la inicialización y lectura de un pin digital.
class Sensor
{
private:
    int Pin_sensor; // Pin donde está conectado el sensor

public:
    // --- Constructor ---
    // Recibe el número del pin al crear el objeto.
    Sensor(int pin): Pin_sensor(pin){}
    
    // --- Funciones públicas ---
    void Inicializar(); // Configura el pin del sensor como entrada
    int Leer_sensor();  // Devuelve la lectura digital (HIGH o LOW)
};

#endif // SENSOR_TCRT5000_H