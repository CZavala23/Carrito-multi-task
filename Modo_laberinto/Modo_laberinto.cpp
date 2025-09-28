#include "Modo_laberinto.h"
#include "config_laberinto.h"

Modo_laberinto::Modo_laberinto(Motor_DC& motorIzq, Motor_DC& motorDer)
  : _motorIzq(motorIzq), _motorDer(motorDer) {
}

// Inicialización del modo
void Modo_laberinto::begin() {
  // Configurar los pines de los sensores ultrasónicos
  pinMode(trigFrontal, OUTPUT);
  pinMode(echoFrontal, INPUT);
  pinMode(trigLateral, OUTPUT);
  pinMode(echoLateral, INPUT);
  pinMode(trigIzquierdo, OUTPUT);
  pinMode(echoIzquierdo, INPUT);
  
  _motorIzq.Stop();
  _motorDer.Stop();
}

void Modo_laberinto::update() {
  long dFrente = medirDistancia(trigFrontal, echoFrontal);
  long dDerecha = medirDistancia(trigLateral, echoLateral);
  long dIzquierda = medirDistancia(trigIzquierdo, echoIzquierdo);

  // PRIORIDAD 1: Obstáculo frontal
  if (dFrente != -1 && dFrente <= DistanciaSegura) {
    _motorIzq.Stop(); _motorDer.Stop(); delay(200);
    _motorIzq.Atras(motorSpeed); _motorDer.Atras(motorSpeed); delay(tiempoRetroceso);
    _motorIzq.Stop(); _motorDer.Stop(); delay(200);
    delay(50); // Pausa anti-interferencia (ultrasonico)
    
    long dDerechaDecision = medirDistancia(trigLateral, echoLateral);
    long dIzquierdaDecision = medirDistancia(trigIzquierdo, echoIzquierdo);

    if (dDerechaDecision > dIzquierdaDecision && dDerechaDecision > (DistanciaSegura * 1.5)) {
      girarDerechaInteligente();
    } else if (dIzquierdaDecision > DistanciaSegura) {
      girarIzquierdaInteligente();
    } else {
      girarDerechaInteligente();
      girarDerechaInteligente();
    }
    _motorIzq.Stop(); _motorDer.Stop(); delay(200);

  // PRIORIDAD 2: Corrección lateral
  } else if (dDerecha != -1 && dDerecha <= DistanciaLateralMinima) {
    // Muy cerca a la DERECHA -> Gira a la IZQUIERDA para alejarse
    _motorIzq.Atras(motorSpeed); _motorDer.Adelante(motorSpeed); // CORREGIDO
    delay(tiempoCorreccion);
    _motorIzq.Stop(); _motorDer.Stop();
  } else if (dIzquierda != -1 && dIzquierda <= DistanciaLateralMinima) {
    // Muy cerca a la IZQUIERDA -> Gira a la DERECHA para alejarse
    _motorIzq.Adelante(motorSpeed); _motorDer.Atras(motorSpeed); // CORREGIDO
    delay(tiempoCorreccion);
    _motorIzq.Stop(); _motorDer.Stop();
    
  // PRIORIDAD 3: Avance normal
  } else {
    _motorIzq.Adelante(motorSpeed); _motorDer.Adelante(motorSpeed);
    delay(tiempoAvance);
    _motorIzq.Stop(); _motorDer.Stop();
    delay(tiempoPausa);
  }
}

// --- Funciones internas ---

long Modo_laberinto::medirDistancia(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duracion = pulseIn(echoPin, HIGH, 20000);
  long distancia = duracion * 0.034 / 2;
  return distancia > 0 ? distancia : -1;
}

void Modo_laberinto::girarDerechaInteligente() {
  int pulsosRealizados = 0;
  bool frenteDespejado = false;

  while (pulsosRealizados < maxPulsosGiro) {
    _motorIzq.Adelante(motorSpeed); _motorDer.Atras(motorSpeed); delay(tiempoPulsoGiro);
    _motorIzq.Stop(); _motorDer.Stop(); delay(pausaEntrePulsos);
    pulsosRealizados++;

    if (!frenteDespejado) {
      long dFrenteActual = medirDistancia(trigFrontal, echoFrontal);
      if (dFrenteActual > (DistanciaSegura + 5) || dFrenteActual == -1) {
        frenteDespejado = true;
      }
    } else {
      long dIzquierdaActual = medirDistancia(trigIzquierdo, echoIzquierdo);
      if (dIzquierdaActual != -1 && dIzquierdaActual < (DistanciaAlineacion + 5) && dIzquierdaActual > (DistanciaAlineacion - 5)) {
        break;
      }
    }
  }
}

void Modo_laberinto::girarIzquierdaInteligente() {
  int pulsosRealizados = 0;
  bool frenteDespejado = false;

  while (pulsosRealizados < maxPulsosGiro) {
    _motorIzq.Atras(motorSpeed); _motorDer.Adelante(motorSpeed); delay(tiempoPulsoGiro);
    _motorIzq.Stop(); _motorDer.Stop(); delay(pausaEntrePulsos);
    pulsosRealizados++;

    if (!frenteDespejado) {
      long dFrenteActual = medirDistancia(trigFrontal, echoFrontal);
      if (dFrenteActual > (DistanciaSegura + 5) || dFrenteActual == -1) {
        frenteDespejado = true;
      }
    } else {
      long dDerechaActual = medirDistancia(trigLateral, echoLateral);
      if (dDerechaActual != -1 && dDerechaActual < (DistanciaAlineacion + 5) && dDerechaActual > (DistanciaAlineacion - 5)) {
        break;
      }
    }
  }
}
