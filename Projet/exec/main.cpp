
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
    
    controlleur.demarrer();

    //controlleur.suivre(3, 35, 4);

    for (int i = 0; i < 4; i++)
    {
        for (int i = 0; i < HUITIEME; i++)
        {
            controlleur.tournerDroite();
        }
        robot.son.ajusterSon(46);
        _delay_ms(200);
        robot.son.stopSon();
    }
    _delay_ms(1000);
    
    for (int i = 0; i < 4; i++)
    {
        for (int i = 0; i < HUITIEME; i++)
        {
            controlleur.tournerGauche();
        }
        robot.son.ajusterSon(46);
        _delay_ms(200);
        robot.son.stopSon();
    }

    while(true){
        DEBUG_PRINT((int)robot.ir.distanceCm());
        _delay_ms(50);
    }

    return 0;
}