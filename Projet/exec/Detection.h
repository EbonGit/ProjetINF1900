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

#define DELAI_SON 300

#define DELAI_LED 250

const uint8_t decalageCertain = 4;

const uint8_t centrageCertain = 7;

const uint8_t ratioVert = 7;

const uint8_t ratioRouge = 3;

#define SECONDE 7812


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


