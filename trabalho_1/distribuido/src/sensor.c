#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <estado.h>
#include <sensor.h>
#include <pthread.h>
#include <semaforo.h>

volatile struct timeval tempo_subida_25;
volatile struct timeval tempo_descida_25;
volatile struct timeval tempo_subida_30;
volatile struct timeval tempo_descida_30;
volatile struct timeval tempo_subida_3;
volatile struct timeval tempo_descida_3;
volatile struct timeval tempo_subida_2;
volatile struct timeval tempo_descida_2;

int pinos[5] = {0, 0, 0, 0, 0};

int primeira_interrupcao_3 = 0;
int primeira_interrupcao_30 = 0;
int primeira_interrupcao_25 = 0;
int primeira_interrupcao_2 = 0;

int info[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,};

int thread_ativa_25 = 0;
int thread_ativa_3 = 0;

volatile int carro_parado_25 = 0;
volatile int carro_parado_3 = 0;

pthread_t thread_25;
pthread_t thread_3;
pthread_t thread_buzzer;

void configuraPinosSensor(int numero){

    if(numero == 1){
        pinos[0] = 30;
        pinos[1] = 2;
        pinos[2] = 25;
        pinos[3] = 3;
        pinos[4] = 0;
    } else if(numero == 2){
        pinos[0] = 27; 
        pinos[1] = 28;
        pinos[2] = 6;
        pinos[3] = 26;
        pinos[4] = 29;

    }

    pinMode(pinos[0], INPUT);
    pinMode(pinos[1], INPUT);
    pinMode(pinos[2], INPUT);
    pinMode(pinos[3], INPUT);

    pullUpDnControl(pinos[0], PUD_UP);
    pullUpDnControl(pinos[1], PUD_UP);
    pullUpDnControl(pinos[2], PUD_UP);
    pullUpDnControl(pinos[3], PUD_UP);
}

void* aciona_buzzer(void* arg){
    digitalWrite(pinos[4], 1);
    usleep(1000 * 1000);
    digitalWrite(pinos[4], 0);
}

void* is_carro_parado_25(void* arg) {
    int tempo_limite = 500;
    usleep(tempo_limite * 1000);

    if (thread_ativa_25) {
        carro_parado_25 = 1;
    }
    return NULL;
}


void* is_carro_parado_3(void* arg) {
    int tempo_limite = 500;
    usleep(tempo_limite * 1000); 

    if (thread_ativa_3) {
        carro_parado_3 = 1;
    }

    return NULL;
}

void both_callback_25() {
    static int last_state = HIGH;

    int state = digitalRead(pinos[2]);
    gettimeofday(state == HIGH ? &tempo_subida_25 : &tempo_descida_25, NULL);

    if (state != last_state) {
        if (state == HIGH) {
            if (primeira_interrupcao_25 > 1) {
                if (pthread_create(&thread_25, NULL, is_carro_parado_25, NULL) == 0) {
                    thread_ativa_25 = 1;
                } else {
                    perror("Falha ao criar a thread");
                }
            }
        } else {
            if (primeira_interrupcao_25 > 0) {
                if (thread_ativa_25) {
                    pthread_cancel(thread_25);
                    thread_ativa_25 = 0;
                }

                long long int intervalo_micro = (tempo_descida_25.tv_sec - tempo_subida_25.tv_sec) * 1000000 +
                                               (tempo_descida_25.tv_usec - tempo_subida_25.tv_usec);

                long long int intervalo_mili = intervalo_micro / 1000;
                float velocidade_media = ((2.0f) / ((float)intervalo_mili / 1000.0f)) * 3.6f;
                printf("Sensor viaAux1 - Velocidade: %.1f km/h\n", velocidade_media);
                info[14]+= (int) velocidade_media;
                if(velocidade_media > 60){
                    info[7]++;
                    pthread_create(&thread_buzzer, NULL, aciona_buzzer, NULL);   
                }
                if(estado == 1 || estado == 2){
                    info[8]++;
                    pthread_create(&thread_buzzer, NULL, aciona_buzzer, NULL);   
                }
                info[6]++;
            }
        }
        last_state = state;
        primeira_interrupcao_25++;
    }
}


void both_callback_30() {
    static int last_state = HIGH;

    int state = digitalRead(pinos[0]);
    gettimeofday(state == HIGH ? &tempo_subida_30 : &tempo_descida_30, NULL);

    if (state != last_state) {
        if (state == HIGH) {
            if (primeira_interrupcao_30 > 1) {
            }
        } else {
            if (primeira_interrupcao_30 > 0) {

                long long int intervalo_micro = (tempo_descida_30.tv_sec - tempo_subida_30.tv_sec) * 1000000 +
                                               (tempo_descida_30.tv_usec - tempo_subida_30.tv_usec);

                long long int intervalo_mili = intervalo_micro / 1000;

                float velocidade_media = ((2.0f) / ((float)intervalo_mili / 1000.0f)) * 3.6f;
                printf("Sensor viaMain 1 - Velocidade: %.1f km/h\n", velocidade_media);
                info[12]+= (int) velocidade_media;
                if(velocidade_media > 80){
                    info[1]++;
                    pthread_create(&thread_buzzer, NULL, aciona_buzzer, NULL);
                }
                if(estado == 3 || estado == 4){
                    info[2]++;
                    pthread_create(&thread_buzzer, NULL, aciona_buzzer, NULL);
                }
                info[0]++;
            }
        }
        last_state = state;
        primeira_interrupcao_30++;
    }
}

void both_callback_3() {
    static int last_state = HIGH;

    int state = digitalRead(pinos[3]);
    gettimeofday(state == HIGH ? &tempo_subida_3 : &tempo_descida_3, NULL);

    if (state != last_state) {
        if (state == HIGH) {
            if (primeira_interrupcao_3 > 1) {
                if (pthread_create(&thread_25, NULL, is_carro_parado_25, NULL) == 0) {
                    thread_ativa_25 = 1;
                } else {
                    perror("Falha ao criar a thread");
                }
            }
        } else {
            if (primeira_interrupcao_3 > 0) {
                if (thread_ativa_3) {
                    pthread_cancel(thread_25);
                    thread_ativa_25 = 0;
                }

                long long int intervalo_micro = (tempo_descida_3.tv_sec - tempo_subida_3.tv_sec) * 1000000 +
                                               (tempo_descida_3.tv_usec - tempo_subida_3.tv_usec);

                long long int intervalo_mili = intervalo_micro / 1000;
                float velocidade_media = ((2.0f) / ((float)intervalo_mili / 1000.0f)) * 3.6f;
                printf("Sensor viaAux2 - Velocidade: %.1f km/h\n", velocidade_media);
                info[15]+= (int) velocidade_media;
                if(velocidade_media > 60){
                    info[10]++;
                    pthread_create(&thread_buzzer, NULL, aciona_buzzer, NULL);
                }
                if(estado == 1 || estado == 2){
                    info[11]++;
                    pthread_create(&thread_buzzer, NULL, aciona_buzzer, NULL);
                }
                info[9]++;
            }
        }
        last_state = state;
        primeira_interrupcao_3++;
    }
}

void both_callback_2() {
    static int last_state = HIGH;

    int state = digitalRead(pinos[1]);
    gettimeofday(state == HIGH ? &tempo_subida_2 : &tempo_descida_2, NULL);

    if (state != last_state) {
        if (state == HIGH) {
            if (primeira_interrupcao_2 > 1) {
            }
        } else {
            if (primeira_interrupcao_2 > 0) {

                long long int intervalo_micro = (tempo_descida_2.tv_sec - tempo_subida_2.tv_sec) * 1000000 +
                                               (tempo_descida_2.tv_usec - tempo_subida_2.tv_usec);

                long long int intervalo_mili = intervalo_micro / 1000;

                float velocidade_media = ((2.0f) / ((float)intervalo_mili / 1000.0f)) * 3.6f;
                printf("Sensor viaMain 2 - Velocidade: %.1f km/h\n", velocidade_media);
                info[13]+= (int) velocidade_media;
                if(velocidade_media > 80){
                    info[4]++;
                    pthread_create(&thread_buzzer, NULL, aciona_buzzer, NULL);
                }

                if(estado == 3 || estado == 4){
                    info[5]++;
                    pthread_create(&thread_buzzer, NULL, aciona_buzzer, NULL);
                }
                info[3]++;
            }
        }
        last_state = state;
        primeira_interrupcao_2++;
    }
}
