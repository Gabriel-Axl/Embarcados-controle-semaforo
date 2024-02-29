#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <estado.h>
#include <sensor.h>
#include <pthread.h>
#include <relatorio.h>

pthread_t thread_relatorio;

int estado = 0;
int pinosSemaforo[] = {0,0,0,0,0,0};

int porta1;
char ip1[9];

void send_data(){
    relatorio(porta1, ip1);
}

void configuraPinosSemaforo(int numero ){
    if(numero == 1){
        pinosSemaforo[0] =  13;
        pinosSemaforo[1] =  14;
        pinosSemaforo[2] =  21;
        pinosSemaforo[3] =  22;
        pinosSemaforo[4] =  23;
        pinosSemaforo[5] =  24;
    }else if(numero == 2){
        pinosSemaforo[0] =  12;
        pinosSemaforo[1] =  10;
        pinosSemaforo[2] =  31;
        pinosSemaforo[3] =  1;
        pinosSemaforo[4] =  4;
        pinosSemaforo[5] =  5;
    }

    pinMode (pinosSemaforo[0], OUTPUT); 
    pinMode (pinosSemaforo[1], OUTPUT); 
    pinMode (pinosSemaforo[2], OUTPUT); 
    pinMode (pinosSemaforo[3], OUTPUT); 

    pinMode (pinosSemaforo[4], INPUT);  
    pinMode (pinosSemaforo[5], INPUT); 

    pinMode(pinos[4], OUTPUT);

    pullUpDnControl(pinosSemaforo[4], PUD_UP);
    pullUpDnControl(pinosSemaforo[5], PUD_UP);

}

void semaforo(int porta, char* ip){

    porta1 = porta;
    strcpy(ip1, ip);

    pthread_create(&thread_relatorio, NULL, send_data, NULL);
    int pin1 = pinosSemaforo[0];
    int pin2 = pinosSemaforo[1];
    int pin3 = pinosSemaforo[2];
    int pin4 = pinosSemaforo[3];

    wiringPiISR(pinos[0], INT_EDGE_BOTH, &both_callback_30);
    wiringPiISR(pinos[1], INT_EDGE_BOTH, &both_callback_2);
    wiringPiISR(pinos[2], INT_EDGE_BOTH, &both_callback_25);
    wiringPiISR(pinos[3], INT_EDGE_BOTH, &both_callback_3);

    while (1)
    {
        int countverde = 0;
        int clicado = 0;
        digitalWrite(pinosSemaforo[4], 1);
        digitalWrite(pinosSemaforo[5], 1);
        while (countverde < 20){
            if (received_info[0]==1)
            {
                while(received_info[0]==1){
                    estado5(pin1, pin2, pin3, pin4);
                    sleep(1);
                }
                break;
            }else if(received_info[1]==1){
                estado = 1;
                while(received_info[1]==1){
                    estado1(pin1, pin2, pin3, pin4);
                    sleep(1);
                }
                break;
            }
            
            if ((digitalRead(pinosSemaforo[4]) == HIGH || countverde < 10) && (digitalRead(pinosSemaforo[5]) == HIGH || countverde < 10) && (carro_parado_25 == 0  || countverde < 10) && (carro_parado_3 == 0  || countverde < 10))
            {
                estado1(pin1, pin2, pin3, pin4);
                estado = 1;
                sleep(1);
                countverde++;
            }else{
                if (digitalRead(pinosSemaforo[4])  == LOW || digitalRead(pinosSemaforo[5]) == LOW || carro_parado_25 == 1 || carro_parado_3 == 1)
                {
                    carro_parado_25 = 0;
                    carro_parado_3 = 0;
                    clicado = 1;
                    estado2(pin1, pin2, pin3, pin4);
                    estado = 2;
                    sleep(2);
                    digitalWrite(pinosSemaforo[4], 1);
                    digitalWrite(pinosSemaforo[5], 1);
                    break;
                }
                
            }
        }

        if(clicado == 0){
            estado2(pin1, pin2, pin3, pin4);
            estado = 2;
            sleep(2);
        }

        int countVermelho = 0;
        clicado = 0;
        while (countVermelho < 10)
        {
            if (received_info[0]==1)
            {
                while(received_info[0]==1){
                    estado5(pin1, pin2, pin3, pin4);
                    sleep(1);
                    
                }
                break;
            }else if(received_info[1]==1){
                estado4(pin1, pin2, pin3, pin4);
                sleep(2);
                estado = 1;
                while(received_info[1]==1){
                    estado1(pin1, pin2, pin3, pin4);
                    sleep(1);
                }
                clicado=1;
                break;
            }
            if((digitalRead(pinosSemaforo[4])  == HIGH || countVermelho < 5 ) && (digitalRead(pinosSemaforo[5]) == HIGH  || countVermelho < 5)){
                estado3(pin1, pin2, pin3, pin4);
                estado = 3;
                sleep(1);
                countVermelho++;
            }else{
                if(digitalRead(pinosSemaforo[4])  == LOW || digitalRead(pinosSemaforo[5]) == LOW){
                    clicado = 1;
                    estado4(pin1, pin2, pin3, pin4);
                    estado = 4;
                    sleep(2);
                    digitalWrite(pinosSemaforo[4], 1);
                    digitalWrite(pinosSemaforo[5], 1);
                    break;
                }
            }
 
        }
        if(clicado == 0){
            estado4(pin1, pin2, pin3, pin4);
            estado = 4;
            sleep(2);
        }
    }
}