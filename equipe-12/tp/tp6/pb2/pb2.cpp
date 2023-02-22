/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi
 * Description: Allume une LED d'une couleur spécifiée en fonction de la luminosité captée par une photoresistance
 * Broche IO: Entrées A0, Sorties B0 B1
*/


#define F_CPU 8000000UL // 8 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "can.cpp"

#define DDR_IN 0x00
#define DDR_OUT 0xff

/* CONSTANTE POUR RS232

// #define TXEN0 3
// #define RXEN0 4
// #define UCSZ02 2
// #define UCSZ01 2
// #define UCSZ00 1
*/

#define TEMPS_ROUGE_AMBRE 2
#define TEMPS_VERT_AMBRE 2

// seuils trouvés le jour du TP avec condition lumineuse de la salle
#define SEUIL_CAPTEUR_BAS 190
#define SEUIL_CAPTEUR_HAUT 240


enum EtatLed
{
    ROUGE = (1 << PA0),
    VERT = (1 << PA1),
    OFF = 0x00
};

enum Etat
{
    E_VERT = 0, 
    E_AMBRE = 1,  
    E_ROUGE = 2   
};


/* UTILE POUR DEBUG ET OBTENIR LES SEUILS DE LA PHOTORESISTANCE VIA RS232

void initialisationUART ( void ) {

// 2400 bauds. Nous vous donnons la valeur des deux

// premiers registres pour vous éviter des complications.

UBRR0H = 0;

UBRR0L = 0xCF;

// permettre la réception et la transmission par le UART0

UCSR0A = 0;

UCSR0B = (1 << RXEN0) | (1 << TXEN0);

// Format des trames: 8 bits, 1 stop bits, sans parité

UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);

}

void transmissionUART ( uint8_t donnee ) {

    while(!(UCSR0A & (1 << UDRE0)));

    UDR0 = donnee;
    

}
*/

void allumeLedAmbre(){
    
    // ratio pour allumer la led en orange
    PORTB = EtatLed::ROUGE;
    _delay_ms(TEMPS_ROUGE_AMBRE);
    
    PORTB = EtatLed::VERT;
    _delay_ms(TEMPS_VERT_AMBRE);
}


//initialisation du bouton et des ports d'entrées et sorties
void init()
{
    DDRA = DDR_IN;
    DDRB = DDR_OUT;
    /* INITIALISATION DU PROTOCOLE RS232
    initialisationUART();
    */
}


int main()
{
    init();

    can c = can();
    Etat etatPresent;
    
    //le programme tourne en boucle
    while (true)
    {
        
        uint16_t val = (c.lecture(PORTA & 0x00)) >> 2;
        
        //transmissionUART(val);

        // vérifier la valeur renvoyée par la photorésistance

        if(val < SEUIL_CAPTEUR_BAS) // Sombre
        { 
            etatPresent = Etat::E_VERT;
        }
        else if (val < SEUIL_CAPTEUR_HAUT) // Ambient
        {
            etatPresent = Etat::E_AMBRE;
        }
        else // Lumineux
        {
            etatPresent = Etat::E_ROUGE;
        }
        

        switch (etatPresent)
        {

            case Etat::E_VERT :
                
                PORTB = EtatLed::VERT;

                break;

            case Etat::E_AMBRE :

                allumeLedAmbre();

                break;

            case Etat::E_ROUGE :

                PORTB = EtatLed::ROUGE;
                
                break;

        }
        
    }

    return 0;
}
