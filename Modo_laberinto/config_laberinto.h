#ifndef CONFIG_LABERINTO_H
#define CONFIG_LABERINTO_H

#include <Arduino.h>

// =======================================================
// ARCHIVO DE CONFIGURACIÓN DEL MODO LABERINTO
// =======================================================


// --- Pines de Sensores Ultrasónicos ---
const int trigFrontal   = A1;
const int echoFrontal   = A2;
const int trigLateral   = A3; // Derecha
const int echoLateral   = A4; // Derecha
const int trigIzquierdo = A0; // Izquierda
const int echoIzquierdo = A5; // Izquierda


// --- Constantes de Comportamiento ---
const int motorSpeed              = 150; // Velocidad base de los motores
const int DistanciaSegura         = 15;  // Umbral para detectar un obstáculo frontal (cm)
const int DistanciaLateralMinima  = 6;   // Umbral para iniciar una corrección lateral (cm)
const int DistanciaAlineacion     = 15;  // Distancia lateral ideal que busca el robot tras un giro (cm)
const int tiempoRetroceso         = 50;  // Duración del pulso de retroceso (ms)
const int tiempoCorreccion        = 150; // Duración del pulso de corrección lateral (ms)


// --- Constantes para Avance y Giro a Pulsos ---
const int tiempoAvance      = 50;  // Duración del pulso de avance (ms)
const int tiempoPausa       = 200; // Pausa entre pulsos de avance (ms)
const int tiempoPulsoGiro   = 100; // Duración de cada "paso" en un giro (ms)
const int pausaEntrePulsos  = 50;  // Pausa entre cada paso de un giro (ms)
const int maxPulsosGiro     = 20;  // Límite de seguridad para evitar giros infinitos


#endif

