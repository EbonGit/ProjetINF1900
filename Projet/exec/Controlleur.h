/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali

 * Description: 

 */

#define F_CPU 8000000UL // 8 MHz

#include <avr/io.h>

#include <util/delay.h>

#include <avr/interrupt.h>

#include "Transmission.h"

#include "GenerateurPoints.h"

#include "SVG_Creator.h"

#define DDR_IN 0x00

#define DDR_OUT 0xff

#define ENTIER 190

#define interuptPIN 0x04

#define externePIN 0x08

#define TEMPS_REBONDS 20

const uint8_t fin = 8;

const uint16_t vitesseMax = 255;

#define ZERO 0

#define PERIODE_TOURNER 25

#define DELTA_TOURNER 0

#define RANGE_PETIT 35 //provisoire

#define RANGE_GRAND 130

const uint8_t TAILLE_MEMOIRE = 20;

const uint8_t TEMPS_MEMOIRE = 25;

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

enum Boussole

{

    NORD = 0,

    NORDEST = 1,

    EST = 2,

    SUDEST = 3,

    SUD = 4,

    SUDOUEST = 5,

    OUEST = 6,

    NORDOUEST = 7

};

class Controlleur

{

private:

   float orientationN_;

   Boussole orientation_;

   int distance_ = -1;

   Boussole poteauBoussole;

   int nombrePoteau = 0;

   EtatRobot etat_;

    //int certitude = 0;

    int nombreTour = 0;

   

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

    void enregistrerPoteau();

    void lireMemoire();

    GenerateurPoints generateur;

    //action

    void suivre(int tour_restant, int distance_active, int distance_stop);

    bool estCertain(int);

    void rechercher(int, int);

    void virerDroite();

    void virerGauche();

    void tournerDroite();

    void tournerGauche();

    

};
