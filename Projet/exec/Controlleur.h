/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */
#define F_CPU 8000000UL // 8 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "Transmission.h"

#define DDR_IN 0x00
#define DDR_OUT 0xff

#define HUITIEME 8.0

#define interuptPIN 0x04
#define externePIN 0x08
#define TEMPS_REBONDS 20

#define PERIODE_TOURNER 40
#define DELTA_TOURNER 0

#define RANGE_PETIT 20 //provisoire
#define RANGE_GRAND 30

enum class EtatRobot
{
    INIT,
    DETECTION,
    TRANSMISSION
    
};

enum class TypeBouton
{
    AUCUN,
    INTERUPT,
    EXTERNE
};

extern volatile TypeBouton bouton;

enum class Boussole
{
    NORD,
    NORDEST,
    EST,
    SUDEST,
    SUD,
    SUDOUEST,
    OUEST,
    NORDOUEST
};

class Controlleur
{
private:
   float orientationN_;
   Boussole orientation_;
   int distance_ = -1;
   Boussole poteauBoussole;
   EtatRobot etat_;
   
   EtatDetection etatDetection_ = EtatDetection::ATTENDRE_1;
   EtatTransmission etatTransmission_ = EtatTransmission::DEBUT;
public:
    Robot* robot_;
    //Controlleur() = default;
    Controlleur(Robot*);
    void demarrer();
    void updateOrientation();

    void detecter();
    void transmettre();

    //provisoire
    void afficherPoteau();
    
    //action
    void suivre(int tour_restant, int distance_active, int distance_stop);
    void rechercher();
    void virerDroite();
    void virerGauche();
    void tournerDroite();
    void tournerGauche();
    
};


