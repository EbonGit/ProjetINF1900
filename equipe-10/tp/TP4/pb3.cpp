#define F_CPU 8000000UL
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

void ajustementPwm(uint16_t valeur)
{

    // mise à un des sorties OC1A et OC1B sur comparaison

    // réussie en mode PWM 8 bits, phase correcte

    // et valeur de TOP fixe à 0xFF (mode #1 de la table 16-5

    // page 130 de la description technique du ATmega324PA)

    OCR1A = valeur;

    OCR1B = valeur;

    // division d'horloge par 8 - implique une fréquence de PWM fixe

    TCCR1A |= (1 << COM1B1) | (1 << COM1A1) | (1 << WGM10);
    
    TCCR1B = (1 << CS11);

    TCCR1C = 0;
}


void initialisation ( ) {

    DDRD = 0xff;


}


int main()
{
    initialisation();
    while(true){
        for (int i = 0; i < 255; i++){
            ajustementPwm(i);
            _delay_ms(100);
        }
        for(int i = 255; i > 0; i--  ){
            ajustementPwm(i);
            _delay_ms(100)  ;
        }
    }
}