#include <estado.h>
#include <wiringPi.h>

void estado1(int pin1, int pin2, int pin3, int pin4) {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
    digitalWrite(pin3, HIGH);
    digitalWrite(pin4, HIGH);
}

void estado2(int pin1, int pin2, int pin3, int pin4) {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
    digitalWrite(pin3, HIGH);
    digitalWrite(pin4, HIGH);
}

void estado3(int pin1, int pin2, int pin3, int pin4) {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, HIGH);
    digitalWrite(pin3, LOW);
    digitalWrite(pin4, HIGH);
}

void estado4(int pin1, int pin2, int pin3, int pin4) {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, HIGH);
    digitalWrite(pin3, HIGH);
    digitalWrite(pin4, LOW);
}

void estado5(int pin1, int pin2, int pin3, int pin4) {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);

    digitalWrite(pin3, HIGH);
    digitalWrite(pin4, LOW);

    delay(500);

    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);

    digitalWrite(pin3, LOW);
    digitalWrite(pin4, LOW);
}

