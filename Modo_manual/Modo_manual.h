#ifndef MODO_MANUAL_H
#define MODO_MANUAL_H

#include <Arduino.h>
#include "../Motores/Motor_DC.h" 

// --- Clase para el Modo de Control Manual ---
// Permite controlar el robot en tiempo real a través de un Stream de datos.
// Recibe comandos de un solo carácter (ej. 'W', 'A', 'S', 'D') y mantiene el movimiento
// hasta que se recibe el comando de parada ('P').
class Modo_manual {
  public:
    // --- Constructor ---
    // Recibe referencias a los dos motores y al canal de comunicación (ej. Serial).
    Modo_manual(Motor_DC& mIzq, Motor_DC& mDer, Stream& bt);

    // --- Funciones Públicas ---
    
    // Inicializa los motores y establece la velocidad de operación.
    void begin(uint8_t velocidad = 255);
    
    // Función principal que se debe llamar en el loop. Lee y procesa los comandos.
    void update();                 

    // Establece una nueva velocidad de movimiento.
    void setVel(uint8_t vel);

    // Devuelve la velocidad actual.
    uint8_t vel() const { return _vel; }

  private:
    // --- Miembros Privados ---
    Motor_DC& _L;       // Referencia al motor izquierdo.
    Motor_DC& _R;       // Referencia al motor derecho.
    Stream&   _bt;      // Referencia al stream de comunicación (Bluetooth).

    uint8_t _vel = 255; // Almacena la velocidad actual de los motores.
    char    _lastCmd = 'P'; // Guarda el último comando de movimiento recibido.

    // --- Funciones de Movimiento Internas ---
    void Parar();
    void Frente();
    void Reversa();
    void GiroIzquierda();   // Giro sobre su propio eje.
    void GiroDerecha();     // Giro sobre su propio eje.
    void SuaveIzquierda();  // Giro en arco hacia adelante.
    void SuaveDerecha();    // Giro en arco hacia adelante.
    void SuaveIzqReversa(); // Giro en arco hacia atrás.
    void SuaveDerReversa(); // Giro en arco hacia atrás.

    // --- Procesador de Comandos ---
    // Procesa un carácter de comando y llama a la función de movimiento correspondiente.
    void aplicar(char c);          
};

#endif // MODO_MANUAL_H