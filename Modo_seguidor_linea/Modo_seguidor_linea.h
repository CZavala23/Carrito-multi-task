#ifndef MODO_SEGUIDOR_LINEA_H
#define MODO_SEGUIDOR_LINEA_H

#include <Arduino.h>
#include "Base_Robot.h" // Se necesita la definición de la clase Base_Robot

// --- Clase para el modo Seguidor de Línea ---
// Gestiona la lógica del PID para seguir una línea negra.
class Modo_seguidor_linea
{
private:
    // --- Variables de la clase ---
    Base_Robot& _robot; // Referencia al objeto robot principal para mover los motores
    const uint8_t* _pines_sensores; // Puntero a un array con los pines de los sensores
    int _num_sensores;    // Cantidad de sensores en el array
    bool _negroEsLOW;     // Lógica del sensor (si el negro se lee como LOW o HIGH)

    // --- Variables del PID ---
    int   _sensor[5];     // Almacena la lectura digital (0 o 1) de los sensores
    float _error = 0, _P = 0, _I = 0, _D = 0, _PID = 0;
    float _error_anterior = 0;

public:
    // --- Constructor ---
    // Acepta una referencia al robot, un array con los pines de los sensores y la lógica del sensor.
    Modo_seguidor_linea(Base_Robot& robot, const uint8_t* pines_sensores, int num_sensores, bool negroEsLOW)
        : _robot(robot), // Inicializa la referencia al robot
          _pines_sensores(pines_sensores),
          _num_sensores(num_sensores),
          _negroEsLOW(negroEsLOW)
    {}

    // --- Funciones públicas ---
    void Inicializar(); // Prepara el robot para este modo
    void Lectura_de_sensores(); // Lee los sensores y calcula el error
    void Modo_Seguidor(int kP, int kI, int kD, int Velocidad); // Ejecuta un ciclo del algoritmo PID
};

#endif // MODO_SEGUIDOR_LINEA_H