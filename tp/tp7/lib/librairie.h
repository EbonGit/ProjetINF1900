/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: Déclaration de l'ensemble des classes de la librairie pour le robot du cours INF1900 (voir dossier pour plus de détail)
 */

# define F_CPU 8000000UL
#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "memoire_24.h"
#include "debug.h"

#define DEBOUNCE_TIME 20

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

enum Direction
{
    AVANT = 0,
    ARRIERE = (1 << PB2 | 1 << PB5),
    DROITE = (0 << PB2 | 1 << PB5),
    GAUCHE = (1 << PB2 | 0 << PB5)
};

enum EtatLed
{
    ROUGE = (1 << PA0),
    VERT = (1 << PA1),
    OFF = 0x00
};

class Bouton
{
public:
    Bouton() = default;
    Bouton(int masque);
    void init();
    bool estActif();

private:
    bool peutChangerEtat_ = true;
    int masque_ = 0x04;
    bool debounce();
};

class Timer1
{
public:
    Timer1(volatile int* gMinuterieExpiree);
    void attendre(int duree);
    void partirMinuterie(int duree);

private:
    volatile int* gMinuterieExpiree_;
};

class RS232
{
public:
    RS232() = default;
    void initialisationUART(void);
    void transmissionUART(uint8_t donnee);

private:
    uint8_t donnee_;
};

class can
{
public:

   can();
   ~can();

   // retourne la valeur numerique correspondant a la valeur
   // analogique sur le port A.  pos doit etre entre 0 et 7
   // inclusivement.  Seulement les 10 bits de poids faible
   // sont significatifs.
   uint16_t lecture(uint8_t pos);

private:
};

class Moteur
{
public:
    Moteur() = default;
    //void init();
    void ajustementPWM(int a, int b);
    void changerDirection(Direction d);
    Direction getDirection(void);


private:
    Direction direction_ = Direction::AVANT;
    int vitesse_a_ = 0;
    int vitesse_b_ = 0;
};

class Led
{
public:
    Led() = default;
    Led(EtatLed l);
    void changerCouleur(EtatLed l);
    void changerCouleurAmbre(Timer1 t, int t_vert, int t_rouge, int n);
    EtatLed recupererCouleur();
private:
    EtatLed couleur_;
};