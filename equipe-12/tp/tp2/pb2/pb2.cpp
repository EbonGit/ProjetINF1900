/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi
 * Description: Allume une led suivant une séquence décrite si dessous
 * Broche IO: Entrées D2, Sorties A1 A2
 *  ╔═════════╦════════╦═════════╦════════╗
    ║ PRÉSENT ║ ENTRÉE ║ FUTUR   ║ SORTIE ║
    ╠═════════╬════════╬═════════╬════════╣
    ║ E_RED   ║ 0      ║ E_OFF   ║ RED    ║
    ╠═════════╬════════╬═════════╬════════╣
    ║ E_RED   ║ 1      ║ E_AMBER ║ RED    ║
    ╠═════════╬════════╬═════════╬════════╣
    ║ E_AMBER ║ 0      ║ E_GREEN ║ AMBER  ║
    ╠═════════╬════════╬═════════╬════════╣
    ║ E_GREEN ║ X      ║ E_RED   ║ GREEN  ║
    ╠═════════╬════════╬═════════╬════════╣
    ║ E_OFF   ║ 1      ║ E_GREEN ║ OFF    ║
    ╚═════════╩════════╩═════════╩════════╝
*/

#define F_CPU 8000000UL // 8 MHz
#include <avr/io.h>
#include <util/delay.h>

#define DDR_IN 0x00
#define DDR_OUT 0xff
#define DEBOUNCE_TIME 20
#define WAIT_TIME 2000

#define AMBER_RED_TIME 3
#define AMBER_GREEN_TIME 1

bool boutonMemoire;

enum LedState
{
    RED = (1 << PA0),
    GREEN = (1 << PA1),
    OFF = 0x00
};

enum Etat
{
    E_RED = 0,
    E_AMBER = 1,
    E_GREEN = 2,
    E_OFF = 3
};

Etat etatPresent;

bool boutonEstPresse;

void init()
{
    DDRD = DDR_IN;
    DDRA = DDR_OUT;
    etatPresent = Etat::E_RED;

    boutonEstPresse = false;
}

void allumeLedAmbre(){
    
    PORTA = LedState::RED;
    _delay_ms(AMBER_RED_TIME);
    
    PORTA = LedState::GREEN;
    _delay_ms(AMBER_GREEN_TIME);
}

bool debounceUp()
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

bool debounceDown(){
    
    if (!(PIND & 0x04))
    {
        _delay_ms(DEBOUNCE_TIME);
        if (!(PIND & 0x04))
        {
            return true;
        }
    }

    return false;
}

void passeEtatFutur(Etat futurUp, Etat futurDown)
{

    if (debounceUp() && !boutonEstPresse)
    {
        boutonEstPresse = true;
        etatPresent = futurUp;
    }

    if (debounceDown() &&  boutonEstPresse)
    {
        boutonEstPresse = false;
        etatPresent = futurDown;
    }

}

int main()
{

    init();

    while (true)
    {
        switch (etatPresent)
        {
            case Etat::E_RED : 

                passeEtatFutur(Etat::E_AMBER, Etat::E_OFF);

                break;

            case Etat::E_AMBER :

                passeEtatFutur(Etat::E_AMBER, Etat::E_GREEN);

                break;

            case Etat::E_GREEN :

                passeEtatFutur(Etat::E_RED, Etat::E_RED);

                break;

            case Etat::E_OFF :

                passeEtatFutur(Etat::E_GREEN, Etat::E_OFF);

                break;
        }

        switch (etatPresent)
        {
            case Etat::E_RED : 

                PORTA = LedState::RED;

                break;

            case Etat::E_AMBER :

                allumeLedAmbre();

                break;

            case Etat::E_GREEN :

                PORTA = LedState::GREEN;

                break;

            case Etat::E_OFF :

                PORTA = LedState::OFF;

                break;
        
        }
    }

    return 0;
}
