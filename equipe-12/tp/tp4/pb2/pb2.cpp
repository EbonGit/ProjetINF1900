/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi
 * Description: Allume une led à la suite de la séquence de 3 appuis sur le boutton
 * Broche IO: Entrées D2, Sorties A1 A2
 *
*/

#define F_CPU 8000000UL // 8 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define DDR_IN 0x00
#define DDR_OUT 0xff
#define DEBOUNCE_TIME 20
#define WAIT_TIME 10000
#define DELAY 100
#define COM1A0 6

//bool boutonMemoire;

enum LedState
{
    RED = (1 << PA0),
    GREEN = (1 << PA1),
    OFF = 0x00
};

volatile uint8_t gMinuterieExpiree = 0;
volatile uint8_t gBoutonPoussoir = 0;

void init()
{
    cli ();
    DDRD = DDR_IN;
    DDRA = DDR_OUT;
    EIMSK |= (1 << INT0) ;
    EICRA |= (1 << INT0);
    sei ();


}

bool debounce()
{

    if (PIND & 0x04)
    {
        _delay_ms(DEBOUNCE_TIME);
        if (PIND & 0x04)
        {
            return true;
        }
    }
    return false;
}

void partirMinuterie ( uint16_t duree ) {

    gMinuterieExpiree = 0;

    // mode CTC du timer 1 avec horloge divisée par 1024

    // interruption après la durée spécifiée

    TCNT1 = 0;

    OCR1A = duree;

    TCCR1A = (1 << COM1A0); //00100000

    TCCR1B = (1 << 2 | 1 << 0);

    TCCR1C = 0;

    TIMSK1 = (1 << 5 | 1 << 1);

}


ISR (TIMER1_COMPA_vect) {

    gMinuterieExpiree = 1;

}


ISR (INT0_vect) {

    

    if(debounce){
        gBoutonPoussoir = 1;
    }

    EIFR |= (1 << INTF0) ;

}



int main()
{

    init();

    _delay_ms(WAIT_TIME);

    

    PORTA = LedState::RED;

    _delay_ms(DELAY);

    PORTA = LedState::OFF;
    
    //MICROSECONDS ?
    gBoutonPoussoir = 0;
    partirMinuterie(WAIT_TIME);
    


    do{
        //
    }while(gBoutonPoussoir == 0 && gMinuterieExpiree == 0);

    if(gBoutonPoussoir == 1 && gMinuterieExpiree == 0){
        PORTA = LedState::GREEN;

    }else{
        PORTA = LedState::RED;
    }

    cli();

    return 0;
}
