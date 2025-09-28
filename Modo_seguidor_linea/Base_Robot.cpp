#include "Base_Robot.h"
#include <Arduino.h>

// --- Prepara ambos motores para su uso ---
void Base_Robot::Inicializar_Robot()
{
  _motor_izq.Inicializar_Motor();
  _motor_der.Inicializar_Motor();
}

// --- Mueve el robot hacia atrás ---
void Base_Robot::Atras(int Velocidad_1, int Velocidad_2)
{
  _motor_izq.Atras(Velocidad_1);
  _motor_der.Atras(Velocidad_2);
}

// --- Mueve el robot hacia adelante ---
void Base_Robot::Adelante(int Velocidad_1, int Velocidad_2)
{
  _motor_izq.Adelante(Velocidad_1);
  _motor_der.Adelante(Velocidad_2);
}

// --- Detiene el robot por completo ---
void Base_Robot::Stop()
{
  _motor_izq.Stop();
  _motor_der.Stop();
}

// --- Función de control flexible para el PID ---
// Mueve cada motor adelante o atrás dependiendo del signo de la velocidad.
void Base_Robot::Mover(int velocidad_izq, int velocidad_der)
{
    // Control del motor izquierdo
    if (velocidad_izq > 0) {
        _motor_izq.Adelante(velocidad_izq);
    } else {
        _motor_izq.Atras(abs(velocidad_izq));
    }

    // Control del motor derecho
    if (velocidad_der > 0) {
        _motor_der.Adelante(velocidad_der);
    } else {
        _motor_der.Atras(abs(velocidad_der));
    }
}
