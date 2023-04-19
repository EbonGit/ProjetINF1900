
/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: instantiation du robot ainsi que de son controlleur.
 */
#include "Controlleur.h"

volatile int gMinuterieExpiree = 0;

//interruption pour la fonction attendre de la librairie
ISR(TIMER1_COMPA_vect){
    gMinuterieExpiree = 1;
}

void init(){
    cli();
    //initialisation des PINS
    DDRD = DDR_OUT xor (1 << PD2) xor (1 << PD3);
    DDRB = DDR_OUT;
    DDRA = DDR_OUT xor (1 << PA0);
    PORTD = 0 << PD6; //mise a zero du pin D6 pour le buzzer
    //masque pour les interuptions
    EIMSK |= (1 << INT0) | (1 << INT1);
    EICRA |= (1 << ISC11) | (1 << ISC01) | (1 << ISC00);
    sei();
    DEBUG_PRINT("debut");
}


int main(){
    init();

    Robot robot = Robot(&gMinuterieExpiree);
    Controlleur controlleur = Controlleur(&robot);
    
    controlleur.demarrer();

    return 0;
}