#ifndef BASE_ROBOT_H
#define BASE_ROBOT_H

#include <Arduino.h>
#include "../Motores/Motor_DC.h"

// --- Clase que representa la base física del robot ---
// Abstrae el control de los dos motores como una sola unidad.
class Base_Robot
{
private:
    // --- Variables de la clase ---
    Motor_DC& _motor_izq; // Referencia al motor izquierdo
    Motor_DC& _motor_der; // Referencia al motor derecho

public:
    // --- Constructor ---
    // Acepta referencias a los dos objetos Motor_DC ya creados.
    Base_Robot(Motor_DC& motor_izq, Motor_DC& motor_der)
        : _motor_izq(motor_izq), _motor_der(motor_der) {}

    // --- Funciones públicas ---
    void Inicializar_Robot(); // Inicializa ambos motores
    void Adelante(int Velocidad_1, int Velocidad_2); // Mueve ambos motores hacia adelante
    void Atras(int Velocidad_1, int Velocidad_2);   // Mueve ambos motores hacia atrás
    void Stop(); // Detiene ambos motores
    void Mover(int velocidad_izq, int velocidad_der); // Controla ambos motores con dirección
};

#endif // BASE_ROBOT_H