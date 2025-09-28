#ifndef MOTOR_DC_H
#define MOTOR_DC_H

#include <Arduino.h>

// --- Clase para controlar un motor DC ---
// Diseñada para un driver tipo L298N que usa 2 pines de dirección y 1 pin PWM para velocidad.
class Motor_DC {
public:
  // --- Constructor ---
  // Recibe los pines de control al crear el objeto.
  Motor_DC(int pinA, int pinB, int pinPWM);

  // --- Funciones públicas ---
  void Inicializar_Motor();   // Configura los pines como salida.
  void Adelante(int Velocidad);  // Gira el motor en una dirección.
  void Atras(int Velocidad);    // Gira el motor en la dirección opuesta.
  void Stop();                  // Detiene el motor (freno).

private:
  // --- Pines de control ---
  int A, B;   // Pines de dirección (IN1, IN2).
  int Pwm;    // Pin de velocidad (ENA, ENB).
};

#endif // MOTOR_DC_H