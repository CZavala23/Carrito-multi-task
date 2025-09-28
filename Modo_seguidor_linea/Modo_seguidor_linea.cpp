#include "Modo_seguidor_linea.h"
#include <Arduino.h>

// --- Inicializa los componentes necesarios para este modo ---
void Modo_seguidor_linea::Inicializar()
{
    // Solo necesita inicializar los motores a través del objeto robot.
    // Los pines de los sensores se configuran en el setup() del archivo principal.
    _robot.Inicializar_Robot();
    _I = 0; // Resetea el término integral al iniciar
    _error_anterior = 0; // Resetea el error anterior
}

// --- Ejecuta el algoritmo de control PID ---
void Modo_seguidor_linea::Modo_Seguidor(int Kp, int Ki, int Kd, int Velocidad)
{
    // 1. Lee los sensores para saber dónde está la línea
    Lectura_de_sensores();

    // 2. Calcula los términos del PID
    _P = _error;
    _I = _I + _error; // **CORRECCIÓN CRÍTICA:** El integral suma el error actual
    _D = _error - _error_anterior;

    // 3. Calcula la corrección total del PID
    _PID = (Kp * _P) + (Ki * _I) + (Kd * _D);

    // 4. Guarda los valores actuales para el próximo ciclo
    _error_anterior = _error;

    // 5. Calcula la velocidad para cada motor
    int velocidad_motor_izquierdo = Velocidad - _PID;
    int velocidad_motor_derecho   = Velocidad + _PID;

    // 6. Limita las velocidades para que no excedan los valores de PWM (-255 a 255)
    // **CORRECCIÓN CRÍTICA:** La función constrain() devuelve el valor, no modifica la variable.
    velocidad_motor_izquierdo = constrain(velocidad_motor_izquierdo, -255, 255);
    velocidad_motor_derecho   = constrain(velocidad_motor_derecho,   -255, 255);
    
    // 7. Mueve el robot con las velocidades calculadas
    _robot.Mover(velocidad_motor_izquierdo, velocidad_motor_derecho);
}

// --- Lee los sensores y calcula el error de desviación ---
void Modo_seguidor_linea::Lectura_de_sensores()
{
    // Normaliza la lectura del sensor a 0 (blanco) o 1 (negro) según la lógica definida
    for (int i = 0; i < _num_sensores; i++) {
        int lectura = digitalRead(_pines_sensores[i]);
        _sensor[i] = _negroEsLOW ? (lectura == LOW) : (lectura == HIGH);
    }

    // Calcula un valor de error ponderado basado en qué sensores ven la línea
    // El valor es 0 si está centrado, negativo si se desvía a la izquierda, positivo a la derecha.
    if      ((_sensor[0]==0)&&(_sensor[1]==0)&&(_sensor[2]==1)&&(_sensor[3]==0)&&(_sensor[4]==0)) _error = 0;
    else if ((_sensor[0]==0)&&(_sensor[1]==1)&&(_sensor[2]==1)&&(_sensor[3]==0)&&(_sensor[4]==0)) _error = -1;
    else if ((_sensor[0]==0)&&(_sensor[1]==1)&&(_sensor[2]==0)&&(_sensor[3]==0)&&(_sensor[4]==0)) _error = -2;
    else if ((_sensor[0]==1)&&(_sensor[1]==1)&&(_sensor[2]==0)&&(_sensor[3]==0)&&(_sensor[4]==0)) _error = -3;
    else if ((_sensor[0]==1)&&(_sensor[1]==0)&&(_sensor[2]==0)&&(_sensor[3]==0)&&(_sensor[4]==0)) _error = -4;
    else if ((_sensor[0]==0)&&(_sensor[1]==0)&&(_sensor[2]==1)&&(_sensor[3]==1)&&(_sensor[4]==0)) _error = 1;
    else if ((_sensor[0]==0)&&(_sensor[1]==0)&&(_sensor[2]==0)&&(_sensor[3]==1)&&(_sensor[4]==0)) _error = 2;
    else if ((_sensor[0]==0)&&(_sensor[1]==0)&&(_sensor[2]==0)&&(_sensor[3]==1)&&(_sensor[4]==1)) _error = 3;
    else if ((_sensor[0]==0)&&(_sensor[1]==0)&&(_sensor[2]==0)&&(_sensor[3]==0)&&(_sensor[4]==1)) _error = 4;
    // Si se pierde la línea, mantiene el último valor de error para intentar encontrarla
}