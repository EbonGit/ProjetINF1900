/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */
#include <librairie.h>

#define UNE_SECONDE 1000

#define DEUX_SECONDE 2000

#define DISTANCE_STOP 15

#define SUIVRE_PARAM 3

const uint8_t inconnu = -1;

#define VITESSE 250

#define DELAI_STOP 100

const uint8_t decalageCertain = 4;

const uint8_t centrageCertain = 7;


enum class EtatDetection
{
    ATTENDRE_1,
    HAUT,
    DROIT,
    RECHERCHE,
    TROUVE,
    NON_TROUVE,
    AVANCER,
    PROCHE,
    ATTENDRE_2
};


