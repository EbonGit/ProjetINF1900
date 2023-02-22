/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi
 * Description: Allume une led à la suite de la séquence de 3 appuis sur le boutton
*/

#define F_CPU 8000000UL // 8 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define DDR_IN 0x00
#define DDR_OUT 0xff
#define DEBOUNCE_TIME 20
#define WAIT_TIME 2000

//bool boutonMemoire;

enum LedState
{
    RED = (1 << PA0),
    GREEN = (1 << PA1),
    OFF = 0x00
};

enum Etat
{
    INIT = 0,
    EA = 1,
    EB = 2,
    EC = 3
};

volatile Etat etatPresent;

volatile bool peutChangerEtat;

void init()
{
    cli ();
    DDRD = DDR_IN;
    DDRA = DDR_OUT;
    EIMSK |= (1 << INT0) ;
    EICRA |= (1 << INT0);
    sei ();

    etatPresent = Etat::INIT;

    peutChangerEtat = true;
}

bool debounce()
{

    if (PIND & 0x04)
    {
        _delay_ms(DEBOUNCE_TIME);
        if (PIND & 0x04)
        {
            return true;
        }
    }
    return false;
}

void passeEtatFutur(Etat futur)
{

    if (debounce() && peutChangerEtat)
    {
        peutChangerEtat = false;
        etatPresent = futur;
    }

    if (!(PIND & 0x04))
    {
        peutChangerEtat = true;
    }
}

ISR (INT0_vect) {

    switch (etatPresent)
        {

            case Etat::INIT :

                passeEtatFutur(Etat::EA);

                break;

            case Etat::EA :

                passeEtatFutur(Etat::EB);

                break;

            case Etat::EB :

                passeEtatFutur(Etat::EC);

                break;

            case Etat::EC :

                _delay_ms(WAIT_TIME);

                etatPresent = Etat::INIT;

                //mettre l'etat à true pour pouvoir appuyer de nouveau dans Init
                peutChangerEtat = true;

                break;
        }

    // Voir la note plus bas pour comprendre cette instruction et son rôle

    EIFR |= (1 << INTF0) ;

}


int main()
{

    init();

    while (true)
    {
        

        switch (etatPresent)
        {

            case Etat::INIT :

                PORTA = LedState::OFF;

                break;

            case Etat::EA :

                PORTA = LedState::OFF;

                break;

            case Etat::EB :

                PORTA = LedState::OFF;

                break;

            case Etat::EC :

                PORTA = LedState::GREEN;

                break;
        }
    }

    return 0;
}
