/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description:
 */
#include "bouton.h"


Bouton::Bouton(int masque){
    masque_ = masque;
}

void Bouton::init(){
    cli ();
    EIMSK |= (1 << INT0) | (1 << INT1);
    EICRA |= (1 << ISC11) | (1 << ISC01) | (1 << ISC00);
    sei ();

    peutChangerEtat_ = true;
}

bool Bouton::debounce()
{

    if (PIND & masque_)
    {
        _delay_ms(DEBOUNCE_TIME);
        if (PIND & masque_)
        {
            return true;
        }
    }
    return false;
}

bool Bouton::estActif(){
    if (debounce() && peutChangerEtat_)
    {
        peutChangerEtat_ = false;
        return true;
    }
    if (!(PIND & masque_))
    {
        peutChangerEtat_ = true;
    }
    return false;
}