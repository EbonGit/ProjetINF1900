
/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */
#include "main.h"

volatile int gMinuterieExpiree = 0;


void init(){
    cli();
    DDRD = DDR_OUT;
    DDRB = DDR_OUT;
    DDRA = DDR_OUT;
    PORTD = 0 << PD6; //mise a zero du pin D6 pour le buzzer
    sei();
}

//interruption pour la fonction attendre de la librairie
ISR(TIMER1_COMPA_vect){
    gMinuterieExpiree = 1;
}


int main(){

    init();

    Robot robot = Robot(&gMinuterieExpiree);
    
    while(true);

    return 0;
}