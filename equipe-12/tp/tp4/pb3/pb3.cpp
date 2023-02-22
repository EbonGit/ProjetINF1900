/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi
 * Description: Attenue une DEL progressivement en 3 secondes grace à un signal PWM
 * Broche IO: 
 *  
*/

#define F_CPU 8000000UL // 8 MHz
#include <avr/io.h>
#include <util/delay.h>


#define DDR_IN 0x00
#define DDR_OUT 0xff
#define DEBOUNCE_TIME 20

#define B 1000 // frequence 1 kHz
#define DUREE 2000.0 // durée du fade = 3s

#define CS11 1
#define WGM10 0
#define COM1A1 7
#define COM1A0 6
#define COM1B1 5
#define COM1B0 4

int vitesses[5]  = {255, 191, 127, 64, 0};

enum SensState
{
    AVANT = 0,
    ARRIERE = (1 << PD2 | 1 << PD3),
    DROITE = (0 << PD2 | 1 << PD3),
    GAUCHE = (1 << PD2 | 0 << PD3)
};

void init();
void delay(int16_t);

void ajustementPwm (int a, int b) {

    // mise à un des sorties OC1A et OC1B sur comparaison

    // réussie en mode PWM 8 bits, phase correcte

    // et valeur de TOP fixe à 0xFF (mode #1 de la table 16-5

    // page 130 de la description technique du ATmega324PA)

    OCR1A = a;

    OCR1B = b;


    // division d'horloge par 8 - implique une fréquence de PWM fixe

    TCCR1A = (1 << WGM10 | 1 << COM1A1 | 0 << COM1A0 | 1 << COM1B1 | 0 << COM1B0);

    TCCR1B = (1 << CS11);

    TCCR1C = 0;

}


int main()
{
    
    init();

    PORTD = SensState::AVANT;

    while(true){
        for(int v : vitesses){
            ajustementPwm(v,v);
            delay(2000);
        }
    }

    return 0;
}

void init()
{
    DDRD = DDR_OUT;
    

}

void delay(int16_t temps){
    for(int16_t j = 0; j < temps; j++){
        _delay_ms(1);
    }
}

