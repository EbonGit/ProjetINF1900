/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: Déclaration de l'ensemble des classes de la librairie pour le robot du cours INF1900 (voir dossier pour plus de détail)
 */

#define F_CPU 8000000UL
#include "memoire_24.h"
#include "debug.h"
#include "bouton.h"
#include "can.h"
#include "gestionSon.h"
#include "led.h"
#include "moteur.h"
#include "RS232.h"
//#include "timer1.h" defini dans led.h

// constantes pour ISR et minuterie
#define COM1A0 6
#define CSN12 2
#define CS10 0
#define ICIE1 5
#define OCIE1A 1

#define TXEN0 3
#define RXEN0 4

#define UCSZ02 2
#define UCSZ01 2
#define UCSZ00 1

#define CS00 0
#define WGM0 0
#define COM0A1 7
#define COM0A0 6
#define COM0B1 5
#define COM0B0 4
#define CSN12 2

class Robot : public Memoire24CXXX, 
              public Led, 
              public Timer1, 
              public Moteur, 
              public GestionSon
{
private:
    /* data */
public:
    //constructeur
    Robot(volatile int*);
    //methodes
    void sequenceDebut();
};

