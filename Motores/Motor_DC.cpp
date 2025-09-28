#include "Motor_DC.h"

// --- Constructor ---
// Asigna los pines recibidos a las variables privadas de la clase.
Motor_DC::Motor_DC(int pinA, int pinB, int pinPWM)
: A(pinA), B(pinB), Pwm(pinPWM) {}

// --- Configuración inicial ---
// Establece los pines de control como salidas y detiene el motor.
void Motor_DC::Inicializar_Motor() {
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  // El pin PWM es opcional. Si es negativo, no se usa.
  if (Pwm >= 0) pinMode(Pwm, OUTPUT);
  Stop();
}

// --- Mueve el motor hacia adelante ---
void Motor_DC::Adelante(int Velocidad) {
  digitalWrite(A, HIGH);
  digitalWrite(B, LOW);
  // Aplica la velocidad al pin PWM, limitada entre 0 y 255.
  if (Pwm >= 0) analogWrite(Pwm, constrain(Velocidad, 0, 255));
}

// --- Mueve el motor hacia atrás ---
void Motor_DC::Atras(int Velocidad) {
  digitalWrite(A, LOW);
  digitalWrite(B, HIGH);
  // Aplica la velocidad al pin PWM, limitada entre 0 y 255.
  if (Pwm >= 0) analogWrite(Pwm, constrain(Velocidad, 0, 255));
}

// --- Detiene el motor  ---
// Pone ambos pines de dirección en LOW, lo que causa un frenado en la mayoría de drivers.
void Motor_DC::Stop() {
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  // Asegura que la velocidad sea cero.
  if (Pwm >= 0) analogWrite(Pwm, 0);
}
