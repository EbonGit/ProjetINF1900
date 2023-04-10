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

#define HUITIEME 40

#define PERIODE_TOURNER 10
#define DELTA_TOURNER 1

#define RANGE_PETIT 20 //provisoire
#define RANGE_GRAND 30

enum EtatRobot
{
    INIT = 0,
    DETECTION = 1,
    TRANSMISSION = 2
    
};

enum TypeBouton
{
    AUCUN = -1,
    INTERUPT = 0,
    EXTERNE = 1
};

enum Boussole
{
    NORD = 0,
    NORDEST = 1,
    EST = 2,
    SUDEST = 3,
    SUD = 4,
    SUDOUEST = 5,
    OUEST = 6,
    NORDOUEST = 7,
};

class Controlleur
{
private:
   Robot* robot_;
   Boussole orientation_;
   EtatRobot etat_;
   TypeBouton bouton_ = TypeBouton::AUCUN;
   
   EtatDetection etatDetection_ = EtatDetection::ATTENDRE_1;
   EtatTransmission etatTransmission_ = EtatTransmission::DEBUT;
public:
    //Controlleur() = default;
    Controlleur(Robot*);
    void demarrer();

    void detecter();
    void transmettre();

    //action
    void suivre(int tour_restant, int distance_active, int distance_stop);
    void virerDroite();
    void virerGauche();
    void tournerDroite();
    void tournerGauche();
    
};


