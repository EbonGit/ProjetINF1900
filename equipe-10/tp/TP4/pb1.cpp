#define F_CPU 8000000UL
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>


volatile uint8_t gEtat = 0; // selon le nom de votre variable


enum State {
    Start, // red
    FirstPress, // amber
    FirstRelease, // green
    SecondPress, // red
    SecondRelease, // off
    ThirdPress, // green
};

volatile State state = Start;

void setLed(State state) {
    switch (state)
    {
        case Start:
        case SecondPress:
            PORTA |= (1<<PA1);
            PORTA &= ~(1<<PA0);
            break;
        case FirstRelease:
        case ThirdPress:
            PORTA |= (1<<PA0);
            PORTA &= ~(1<<PA1);
            break;
        case FirstPress:
            PORTA |= (1<<PA0);
            PORTA &= ~(1<<PA1);
            _delay_ms(5);
            PORTA |= (1<<PA1);
            PORTA &= ~(1<<PA0);
            _delay_ms(5);
            break;
        default:
            PORTA &= ~(1<<PA1 | 1<<PA0);
            break;
    }
}

State getNextState(State last_state) {
    if (last_state == Start && gEtat) {
        return FirstPress;
    } else if (last_state == FirstPress && !gEtat) {
        return FirstRelease;
    } else if (last_state == FirstRelease && gEtat) {
        return SecondPress;
    } else if (last_state == SecondPress && !gEtat) {
        return SecondRelease;
    } else if (last_state == SecondRelease && gEtat) {
        return ThirdPress;
    } else if (last_state == ThirdPress && !gEtat) {
        return Start;
    } else {
        return last_state;
    }
}
void initialisation ( ) {
    // cli est une routine qui bloque toutes les interruptions.
    // Il serait bien mauvais d'être interrompu alors que
    // le microcontrôleur n'est pas prêt...
    cli ();

    // configurer et choisir les ports pour les entrées
    // et les sorties. DDRx... Initialisez bien vos variables
    DDRD &= ~(1 << PORTD2);
    DDRA |= (1 << PORTA0) | (1 << PORTA1);

    // cette procédure ajuste le registre EIMSK
    // de l’ATmega324PA pour permettre les interruptions externes
    EIMSK |= (1 << INT0) ;

    // il faut sensibiliser les interruptions externes aux
    // changements de niveau du bouton-poussoir
    // en ajustant le registre EICRA
    EICRA |= (1 << ISC00);

    // sei permet de recevoir à nouveau des interruptions.
    sei ();
}

ISR ( INT0_vect ) {
    uint8_t read = PIND;
    _delay_ms ( 30 );
    gEtat = (PIND & PIND4 & read) >> 2;
    state = getNextState(state);
    gEtat = 0;
    EIFR |= (1 << INTF0) ;
}

int main()
{
    initialisation();
    while (true) {
        setLed(state);

    }

}