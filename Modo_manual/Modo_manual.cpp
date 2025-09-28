// --- Implementación de la clase Modo_manual ---
#include "Modo_manual.h"

// --- Constructor ---
// Inicializa las referencias a los motores y al stream de comunicación.
Modo_manual::Modo_manual(Motor_DC& mIzq, Motor_DC& mDer, Stream& bt)
: _L(mIzq), _R(mDer), _bt(bt) {}

// --- Inicialización del Modo ---
// Configura la velocidad, inicializa los motores y se asegura de que el robot comience detenido.
void Modo_manual::begin(uint8_t velocidad) {
  _vel = velocidad;
  _L.Inicializar_Motor();
  _R.Inicializar_Motor();
  Parar();
  _lastCmd = 'P'; // El último comando es 'Parar'.
}

// --- Bucle de Actualización ---
// Se ejecuta continuamente para leer nuevos comandos del Bluetooth.
void Modo_manual::update() {
  // Procesa todos los caracteres disponibles en el buffer de entrada.
  while (_bt.available()) {
    char c = _bt.read();
    if (c == '\r' || c == '\n') continue; // Ignora caracteres de nueva línea.
    if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';  // Convierte el comando a mayúscula.
    aplicar(c); // Procesa el comando.
  }
}

// --- Establecer Velocidad ---
void Modo_manual::setVel(uint8_t vel) {
  _vel = vel;
  aplicar(_lastCmd);  // Vuelve a aplicar el último movimiento para que la nueva velocidad tenga efecto.
}

// --- Aplicar Comando ---
// Centraliza toda la lógica de control, traduciendo un carácter a un movimiento.
void Modo_manual::aplicar(char c) {
  switch (c) {
    // Comandos de movimiento básicos (WASD)
    case 'W': Frente();           _lastCmd = 'W'; break;
    case 'S': Reversa();          _lastCmd = 'S'; break;
    case 'A': GiroIzquierda();    _lastCmd = 'A'; break;
    case 'D': GiroDerecha();      _lastCmd = 'D'; break;

    // Comandos para giros suaves en arco (opcionales)
    case 'Q': SuaveIzquierda();   _lastCmd = 'Q'; break;
    case 'E': SuaveDerecha();     _lastCmd = 'E'; break;
    case 'Z': SuaveDerReversa();  _lastCmd = 'Z'; break;
    case 'C': SuaveIzqReversa();  _lastCmd = 'C'; break;

    // Comandos de compatibilidad con otros mapeos de teclas
    case 'F': GiroIzquierda();    _lastCmd = 'F'; break;
    case 'R': GiroDerecha();      _lastCmd = 'R'; break;

    // Comando de parada (esencial)
    case 'P': Parar();            _lastCmd = 'P'; break;

    // Comandos para ajustar la velocidad remotamente
    case '+': setVel((_vel <= 245) ? (_vel + 10) : 255); break; // Aumenta la velocidad
    case '-': setVel((_vel >= 10)  ? (_vel - 10) : 0);   break; // Disminuye la velocidad

    default: break; // Ignora cualquier otro carácter no reconocido.
  }
}

// --- Implementación de los Movimientos ---
// Cada función llama a los métodos correspondientes en los objetos de motor.
void Modo_manual::Parar()            { _L.Stop();            _R.Stop(); }
void Modo_manual::Frente()           { _L.Adelante(_vel);    _R.Adelante(_vel); }
void Modo_manual::Reversa()          { _L.Atras(_vel);       _R.Atras(_vel); }
void Modo_manual::GiroIzquierda()    { _L.Atras(_vel);       _R.Adelante(_vel); } // Gira sobre su eje
void Modo_manual::GiroDerecha()      { _L.Adelante(_vel);    _R.Atras(_vel); } // Gira sobre su eje
void Modo_manual::SuaveIzquierda()   { _L.Stop();            _R.Adelante(_vel); } // Gira usando una rueda
void Modo_manual::SuaveDerecha()     { _L.Adelante(_vel);    _R.Stop(); }         // Gira usando una rueda
void Modo_manual::SuaveIzqReversa()  { _L.Stop();            _R.Atras(_vel); }
void Modo_manual::SuaveDerReversa()  { _L.Atras(_vel);       _R.Stop(); }
