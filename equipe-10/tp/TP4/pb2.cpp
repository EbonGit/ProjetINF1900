#define F_CPU 8000000UL
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t gMinuterieExpiree;
volatile uint8_t gBoutonPoussoir;
volatile uint16_t duration;

void partirMinuterie ( ) {
    gMinuterieExpiree = 0;
    // mode CTC du timer 1 avec horloge divisée par 1024
    // interruption après la durée spécifiée
    uint16_t duree = 781;
    duration -= 1;
    TCNT1 = 0;
    OCR1A = duree;
    TCCR1A |= (1 << COM1A0) | (1 << COM1A1);
    TCCR1B |= (1 << CS12) | (1 << CS10);
    TIMSK1 |= (1 << OCIE1A);
    TCCR1C |= (1 << FOC1A);
}

void initialisation ( ) {
    // cli est une routine qui bloque toutes les interruptions.
    // Il serait bien mauvais d'être interrompu alors que
    // le microcontrôleur n'est pas prêt...
    cli ();

    // configurer et choisir les ports pour les entrées
    // et les sorties. DDRx... Initialisez bien vos variables
    DDRD &= ~(1 << PORTD2);
    DDRA = 0xff;

    // cette procédure ajuste le registre EIMSK
    // de l’ATmega324PA pour permettre les interruptions externes
    EIMSK |= (1 << INT0);


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
    gBoutonPoussoir = 1;

    EIFR |= (1 << INTF0) ;
}

ISR ( TIMER1_COMPA_vect ) {
    if (duration > 0) {
        partirMinuterie();
    }else{
        gMinuterieExpiree = 1;
    }
}
void setPin(volatile uint8_t * port, uint8_t pin, uint8_t value=1){
    if (value == 1){
        *port |= (1 << pin);
    }else{
        *port &= ~(1 << pin);
    }
}
int main()
{
    initialisation();
    duration = 100;
    partirMinuterie();
    while (gMinuterieExpiree == 0) {
        // attendre qu'une des deux variables soit modifiée
        // par une ou l'autre des interruptions.
    }
    duration = 1;
    partirMinuterie();
    gBoutonPoussoir = 0;
    while (gMinuterieExpiree == 0) {
        // attendre qu'une des deux variables soit modifiée
        // par une ou l'autre des interruptions.
        setPin(&PORTA, PIN0, 0);
        setPin(&PORTA, PIN1);
    }
    setPin(&PORTA, PIN0, 0);
    setPin(&PORTA, PIN1, 0);
    duration = 10;
    partirMinuterie();
    while (gMinuterieExpiree == 0 && gBoutonPoussoir == 0) {
        // attendre qu'une des deux variables soit modifiée
        // par une ou l'autre des interruptions.
    }
    if (gMinuterieExpiree == 1)
        {
            setPin(&PORTA, PIN0, 0);
            setPin(&PORTA, PIN1);
        }
    else
        {
            setPin(&PORTA, PIN0);
            setPin(&PORTA, PIN1, 0);
        }

}