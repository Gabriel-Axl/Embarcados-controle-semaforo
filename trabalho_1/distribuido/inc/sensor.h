#ifndef SENSOR_H
#define SENSOR_H

#include <pthread.h>
#include <sys/time.h>

extern int info[];
extern int pinos[];

extern volatile struct timeval tempo_subida_25;
extern volatile struct timeval tempo_descida_25;
extern volatile struct timeval tempo_subida_30;
extern volatile struct timeval tempo_descida_30;
extern volatile struct timeval tempo_subida_3;
extern volatile struct timeval tempo_descida_3;
extern volatile struct timeval tempo_subida_2;
extern volatile struct timeval tempo_descida_2;

extern int primeira_interrupcao_25;
extern int primeira_interrupcao_30;
extern int primeira_interrupcao_3;
extern int primeira_interrupcao_2;

extern pthread_t thread_25;
extern int thread_ativa_25;
extern pthread_t thread_3;
extern int thread_ativa_3;

extern volatile int carro_parado_25;
extern volatile int carro_parado_3;

extern pthread_t thread_buzzer;

void configuraPinosSensor(int numero);
void* aciona_buzzer(void* arg);
void* is_carro_parado_25(void* arg);
void* is_carro_parado_3(void* arg);
void both_callback_25();
void both_callback_30();
void both_callback_3();
void both_callback_2();

#endif /* SENSORES_H */
