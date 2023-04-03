/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */

#include "moteur.h"


void Moteur::ajustementPWM(int a, int b)
{
    vitesse_a_ = a;
    vitesse_b_ = b;

    //TODO: changer pour Timer0 

    OCR0A = a;

    OCR0B = b;


    // division d'horloge par 8 - implique une fréquence de PWM fixe

    TCCR0A = (1 << WGM0 | 1 << COM0A1 | 0 << COM0A0 | 1 << COM0B1 | 0 << COM0B0);

    TCCR0B = (1 << CS00);

}

void Moteur::changerDirection(Direction d)
{
    direction_ = d;
    PORTB = d;
}

Direction Moteur::getDirection(void)
{
    return direction_;
}