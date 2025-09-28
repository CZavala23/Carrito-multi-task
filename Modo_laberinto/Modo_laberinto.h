#ifndef MODO_LABERINTO_H
#define MODO_LABERINTO_H

#include <Arduino.h>
#include "../Motores/Motor_DC.h" // Incluir la clase de los motores

class Modo_laberinto {
  public:
    Modo_laberinto(Motor_DC& motorIzq, Motor_DC& motorDer);

    // Se llama una vez al activar el modo
    void begin();

    // Se llama repetidamente en el loop principal
    void update();

  private:
    // Referencias a los motores
    Motor_DC& _motorIzq;
    Motor_DC& _motorDer;
    
    // Funciones internas del modo laberinto
    long medirDistancia(int trigPin, int echoPin);
    void girarDerechaInteligente();
    void girarIzquierdaInteligente();
};

#endif

