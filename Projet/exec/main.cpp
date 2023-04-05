
/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */
#include "Controlleur.h"

volatile int gMinuterieExpiree = 0;

//interruption pour la fonction attendre de la librairie
ISR(TIMER1_COMPA_vect){
    gMinuterieExpiree = 1;
}

void init(){
     cli();
    DDRD = DDR_OUT;
    DDRB = DDR_OUT;
    DDRA = DDR_OUT xor (1 << PA0);
    PORTD = 0 << PD6; //mise a zero du pin D6 pour le buzzer
    sei();
    DEBUG_PRINT("debut");
}


int main(){
    init();

    Robot robot = Robot(&gMinuterieExpiree);
    Controlleur controlleur = Controlleur(&robot);
    
    controlleur.suivre();

    return 0;
}