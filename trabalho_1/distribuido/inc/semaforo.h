#ifndef SEMAFORO_H
#define SEMAFORO_H

extern int estado;

void semaforo(int porta, char* ip);
void configuraPinosSemaforo(int numero );
void send_data();
#endif 
