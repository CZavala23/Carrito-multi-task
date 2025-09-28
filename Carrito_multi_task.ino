#include <Arduino.h>
#include <string.h>
#include <stdlib.h>


// Se incluyen las cabeceras (.h) para las clases de cada módulo.
#include "Motores/Motor_DC.h"
#include "Modo_seguidor_linea/Base_Robot.h" 
#include "Modo_manual/Modo_manual.h"
#include "Modo_seguidor_linea/Modo_seguidor_linea.h"
#include "Modo_laberinto/Modo_laberinto.h"

// Se incluyen los .cpp para forzar la compilación.
#include "Motores/Motor_DC.cpp"
#include "Modo_seguidor_linea/Base_Robot.cpp"
#include "Modo_seguidor_linea/Sensor_TCRT5000.cpp"
#include "Modo_manual/Modo_manual.cpp"
#include "Modo_seguidor_linea/Modo_seguidor_linea.cpp"
#include "Modo_laberinto/Modo_laberinto.cpp"

// ====== CONFIGURACIÓN DE SENSORES Y PID ======
const int NUM_SENSORES = 5;
const uint8_t PINES_SENSORES[NUM_SENSORES] = {2, 3, 4, 5, 6};
const bool NEGRO_ES_LOW = true;

// --- Constantes del controlador PID ---
const int Kp = 50, Ki = 0, Kd = 15, VEL_BASE = 85;


// ====== OBJETOS GLOBALES (Motores y Modos) ======
// --- Instancias de los motores DC ---
Motor_DC MIZQ(13, 12, 11);
Motor_DC MDER(10,  8,  9);

//Crea la instancia de la base del robot
Base_Robot robot(MIZQ, MDER);

// 3. Crea las instancias de los modos
Modo_manual         manual(MIZQ, MDER, Serial);
Modo_seguidor_linea seguidor(robot, PINES_SENSORES, NUM_SENSORES, NEGRO_ES_LOW);
Modo_laberinto      laberinto(MIZQ, MDER);

// ====== Selector de Modo======
// --- Define los posibles modos de operación ---
enum class Modo : uint8_t { NINGUNO = 0, MANUAL, SEGUIDOR, LABERINTO };
Modo modo_actual = Modo::NINGUNO;

// --- Prototipo de la función para cambiar de modo ---
void cambiarModo(Modo nuevo_modo);


// ====== LECTURA DE SENSORES ======
// --- Lee un sensor y devuelve 1 si detecta la línea negra ---
inline uint8_t leeSensor(uint8_t pin) {
  uint8_t v = digitalRead(pin);
  return NEGRO_ES_LOW ? (v == LOW) : (v == HIGH);
}

// --- Envía el patrón de sensores por Serial solo si este cambia ---
void imprimirPatronSiCambia() {
  static uint8_t ultimo_patron[NUM_SENSORES] = {255};
  uint8_t patron_actual[NUM_SENSORES];
  bool ha_cambiado = false;

  for (int i = 0; i < NUM_SENSORES; i++) {
    patron_actual[i] = leeSensor(PINES_SENSORES[i]);
    if (patron_actual[i] != ultimo_patron[i]) {
      ha_cambiado = true;
    }
  }

  if (!ha_cambiado) return;

  Serial.print('(');
  for (int i = 0; i < NUM_SENSORES; i++) {
    ultimo_patron[i] = patron_actual[i];
    Serial.print(ultimo_patron[i]);
  }
  Serial.println(')');
}

// ====== SETUP ======
// --- Función de configuración inicial ---
void setup() {
  Serial.begin(9600);

  // Configura los pines de los sensores como entrada.
  for (int i = 0; i < NUM_SENSORES; i++) {
    pinMode(PINES_SENSORES[i], INPUT);
  }
}

// ====== LOOP ======
// --- Bucle principal del programa ---
void loop() {
  // Procesa comandos de cambio de modo recibidos por Serial.
  if (Serial.available()) {
    char comando = toupper(Serial.read());

    switch (comando) {
      case 'U': cambiarModo(Modo::MANUAL);    break;
      case 'I': cambiarModo(Modo::SEGUIDOR);  break;
      case 'O': cambiarModo(Modo::LABERINTO); break;
    }
  }

  // Ejecuta la lógica correspondiente al modo activo.
  switch (modo_actual) {
    case Modo::MANUAL:    manual.update();    break;
    case Modo::SEGUIDOR:
      imprimirPatronSiCambia();
      seguidor.Modo_Seguidor(Kp, Ki, Kd, VEL_BASE);
      break;
    case Modo::LABERINTO: laberinto.update(); break;
    case Modo::NINGUNO:
    default: // Si no hay modo activo, detiene los motores.
      MIZQ.Stop();
      MDER.Stop();
      break;
  }
}

// ====== IMPLEMENTACIÓN DE FUNCIONES AUXILIARES ======
// --- Gestiona la transición entre modos de operación ---
void cambiarModo(Modo nuevo_modo) {
  if (nuevo_modo == modo_actual) return;

  modo_actual = nuevo_modo;
  
  MIZQ.Stop();
  MDER.Stop();

  // Ejecuta la función de inicialización del nuevo modo.
  switch (modo_actual) {
    case Modo::MANUAL:    manual.begin(255);  break;
    case Modo::SEGUIDOR:  seguidor.Inicializar(); break;
    case Modo::LABERINTO: laberinto.begin();    break;
    case Modo::NINGUNO:
    default: break;
  }
}

