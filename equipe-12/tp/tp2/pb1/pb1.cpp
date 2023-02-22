/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi
 * Description: Allume une led à la suite de la séquence de 3 appuis sur le boutton
 * Broche IO: Entrées D2, Sorties A1 A2
 *  +---------+--------+-------+--------+
    | PRÉSENT | ENTRÉE | FUTUR | SORTIE |
    +---------+--------+-------+--------+
    | INIT    | 0      | INIT  | 0      |
    +---------+--------+-------+--------+
    | INIT    | 1      | EA    | 0      |
    +---------+--------+-------+--------+
    | EA      | 0      | EA    | 0      |
    +---------+--------+-------+--------+
    | EA      | 1      | EB    | 0      |
    +---------+--------+-------+--------+
    | EB      | 0      | EB    | 0      |
    +---------+--------+-------+--------+
    | EB      | 1      | EC    | 0      |
    +---------+--------+-------+--------+
    | EC      | X      | INIT  | 1      |
    +---------+--------+-------+--------+
*/

#define F_CPU 8000000UL // 8 MHz
#include <avr/io.h>
#include <util/delay.h>

#define DDR_IN 0x00
#define DDR_OUT 0xff
#define DEBOUNCE_TIME 20
#define WAIT_TIME 2000

bool boutonMemoire;

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

Etat etatPresent;

bool peutChangerEtat;

void init()
{
    DDRD = DDR_IN;
    DDRA = DDR_OUT;
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

int main()
{

    init();

    while (true)
    {
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

                break;
        }

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
