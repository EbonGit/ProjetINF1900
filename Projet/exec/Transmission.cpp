/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */
#include "Controlleur.h"

void Controlleur::transmettre(){
    switch (etatTransmission_)
    {
    case EtatTransmission::DEBUT:
        DEBUG_PRINT("DEBUT");
        break;

    case EtatTransmission::ENVOI:
        
        break;

    case EtatTransmission::FIN:
        
        break;
    }
}