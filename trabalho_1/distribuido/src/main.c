#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <estado.h>
#include <sensor.h>
#include <semaforo.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[]){
    if (wiringPiSetup () == -1){
        return 1 ;
    }

    configuraPinosSensor(atoi(argv[3]));
    configuraPinosSemaforo(atoi(argv[3]));
    semaforo(atoi(argv[1]), argv[2]);

    return 0;
};
