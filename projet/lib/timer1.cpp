/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */
#include "timer1.h"

Timer1::Timer1(volatile int* gMinuterieExpiree){
    
    gMinuterieExpiree_ = gMinuterieExpiree;
}

void Timer1::partirMinuterie(int duree){
    
    *gMinuterieExpiree_ = 0;

    // mode CTC du timer 1 avec horloge divisée par 1024

    // interruption après la durée spécifiée

    TCNT1 = 0;

    OCR1A = duree;

    TCCR1A = (1 << COM1A0); //00100000

    TCCR1B = (1 << CSN12 | 1 << CS10);

    TCCR1C = 0;

    TIMSK1 = (1 << ICIE1 | 1 << OCIE1A);
}

void Timer1::attendre(int duree){
    partirMinuterie(duree);
    while(*gMinuterieExpiree_ == 0);
}