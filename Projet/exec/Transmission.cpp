/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */
#include "Controlleur.h"

void Controlleur::transmettre(){
    switch (etatTransmission_)
    {
    case EtatTransmission::DEBUT:
        while (true)
        {
            //int d = (int)robot_->ir.distanceCm();
            //DEBUG_PRINT(d);
            lireMemoire();
            _delay_ms(50);
        }
        break;

    case EtatTransmission::ENVOI:
        
        break;

    case EtatTransmission::FIN:
        
        break;
    }
}

void Controlleur::lireMemoire(){
    uint8_t temp;
    uint8_t i = 0;

    while(temp != 8){
        robot_->memoire.lecture(2*i, &temp);
        DEBUG_PRINT(temp);
        _delay_ms(25);
        robot_->memoire.lecture(2*i+1, &temp);
        DEBUG_PRINT(temp);
        _delay_ms(25);
        i++;
    }
    
}