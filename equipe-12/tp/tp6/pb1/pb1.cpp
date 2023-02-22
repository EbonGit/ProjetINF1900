/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi
 * Description: Allume une led suivant une séquences qui est en partie définie par le temps d'appui sur le bouton externe
 * Broche IO: Entrées D2, Sorties B0 B1
 *  ╔═════════╦════════╦═══════╦════════════╗
    ║ PRÉSENT ║ ENTRÉE ║ FUTUR ║ SORTIE     ║
    ╠═════════╬════════╬═══════╬════════════╣
    ║ INIT    ║ 0      ║ INIT  ║ OFF        ║
    ╠═════════╬════════╬═══════╬════════════╣
    ║ INIT    ║ 1      ║ MIN   ║ OFF        ║
    ╠═════════╬════════╬═══════╬════════════╣
    ║ MIN     ║ 1      ║ MIN   ║ OFF        ║
    ╠═════════╬════════╬═══════╬════════════╣
    ║ MIN     ║ 0      ║ SEQ   ║ OFF        ║
    ╠═════════╬════════╬═══════╬════════════╣
    ║ SEQ     ║ X      ║ INIT  ║ sequence() ║
    ╚═════════╩════════╩═══════╩════════════╝
    
*  sequence() -> GREEN 0.5s -> OFF 2s -> clignote RED/OFF -> GREEN 1s
*/

#define F_CPU 8000000UL // 8 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define DDR_IN 0x00
#define DDR_OUT 0xff

#define TEMPS_REBOND 20
#define DUREE_MIN 781
#define SECONDE 7812

// constantes pour ISR et minuterie
#define COM1A0 6
#define CSN12 2
#define CS10 0
#define ICIE1 5
#define OCIE1A 1


enum EtatLed
{
    RED = (1 << PA0),
    GREEN = (1 << PA1),
    OFF = 0x00
};

enum Etat
{
    INIT = 0, //Initial
    MIN = 1,  //Minuterie
    SEQ = 2   //Sequence
};

//fonction volatile utilisable dans les ISR
volatile Etat etatPresent;
volatile bool peutChangerEtat;

volatile uint8_t gMinuterieExpiree = 0;

//compteur utilisé dans l'état Etat::MIN
uint8_t compteur = 0;

//initialisation du bouton et des ports d'entrées et sorties
void init()
{
    cli ();
    DDRD = DDR_IN;
    DDRB = DDR_OUT;
    EIMSK |= (1 << INT0) ;
    EICRA |= (1 << INT0);
    sei ();

    etatPresent = Etat::INIT;

    peutChangerEtat = true;
}

//fonction de debounce du bouton
bool rebondBas()
{

    if (!(PIND & 0x04)) //on debounce par not(PIND & 0x04) car l'état haut du bouton est quand il est appuyé
    {
        _delay_ms(TEMPS_REBOND);
        if (!(PIND & 0x04))
        {
            return true;
        }
    }
    return false;
}

//change la valeur de EtatPresent par futur si les conditions sont remplies
void passeEtatFutur(Etat futur)
{

    if (rebondBas() && peutChangerEtat)
    {
        peutChangerEtat = false;
        etatPresent = futur;
    }

    if (!(PIND & 0x04))
    {
        peutChangerEtat = true;
        
    }
}

//interruption par le bouton externe, appel passeEtatFutur pour changer d'état
ISR (INT0_vect) {

    switch(etatPresent)
        {
            case Etat::INIT :
                //premier appui
                passeEtatFutur(Etat::MIN);

                break;

            case Etat::MIN :
                //appel quand relache, etatPresent -> Etat::SEQ 
                etatPresent = Etat::SEQ;
                break;

            case Etat::SEQ :
                
                break;
        }

    // Voir la note plus bas pour comprendre cette instruction et son rôle
    EIFR |= (1 << INTF0) ;
}

//fonction qui lance une minuterie, gMinuterieExpiree -> 0
void partirMinuterie ( uint16_t duree ) {

    gMinuterieExpiree = 0;

    // mode CTC du timer 1 avec horloge divisée par 1024

    // interruption après la durée spécifiée

    TCNT1 = 0;

    OCR1A = duree;

    TCCR1A = (1 << COM1A0); //00100000

    TCCR1B = (1 << CSN12 | 1 << CS10);

    TCCR1C = 0;

    TIMSK1 = (1 << ICIE1 | 1 << OCIE1A);

}

//fonction qui fait une pause avec la minuterie interne
void attendre(uint16_t duree){
    partirMinuterie(duree);
    while(gMinuterieExpiree == 0);
}

//interruption de la minuterie -> passe gMinuterieExpiree à 1
ISR (TIMER1_COMPA_vect) {

    gMinuterieExpiree = 1;

}

//sequence de la led dans l'état SEQ
void sequence(){
    
    PORTB = EtatLed::GREEN;
    attendre(SECONDE/2); // attend une demie seconde
    
    PORTB = EtatLed::OFF;
    attendre(SECONDE*2); // attend deux secondes

    uint8_t iterations = compteur/2;

    for (uint8_t i = 0; i < iterations; i++)
    {
        PORTB = EtatLed::RED;
        attendre(SECONDE/2); // attend une demie seconde

        PORTB = EtatLed::OFF;
        attendre(SECONDE/2); // attend une demie seconde
    }

    PORTB = EtatLed::GREEN;
    attendre(SECONDE);
    PORTB = EtatLed::OFF;
    
    compteur = 0;
}

int main()
{
    init();

    //le programme tourne en boucle
    while (true)
    {
        switch (etatPresent)
        {
            case Etat::INIT :
                
                PORTB = EtatLed::OFF;

                break;

            case Etat::MIN :

                //incrémente de 1 chaque 1/10 de seconde le compteur tant que le bouton est appuyé, si le compteur > 120 on passe à Etat::SEQ
                if(compteur >= 120)
                {
                    etatPresent = Etat::SEQ;
                }
                else
                {

                    PORTB = EtatLed::OFF;
                    attendre(DUREE_MIN);
                    
                    if(etatPresent == Etat::MIN)
                    {
                        compteur += 1;
                    }
                    
                }
                break;

            case Etat::SEQ :

                //appel la séquence de la led
                sequence();

                //retour Etat::INIT
                etatPresent = Etat::INIT;
                
                break;

        }
    }

    return 0;
}
