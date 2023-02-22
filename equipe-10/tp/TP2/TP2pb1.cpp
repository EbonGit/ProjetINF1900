// Auteurs: GHABIA Mohammed Amine et ZEROUALI Amine
// Description du programme: Programation d'une machine a etats simples avec une DEL bicolore.
//  Entrees: PORTD2 (l'interrupteur)
//  Sorties: PORTA0, PORTA1 (la DEL)
// table d'etat : 
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | Etat présent | Entrée D (interrupteur) | Etat suivant | Sortie A1 | Sortie A0 | Couleur LED |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | PRESS 1      | 1                       | PRESS 1      | 0         | 0         | off         |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | PRESS 1      | 0                       | RELEASE 1    | 0         | 0         | off         |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | RELEASE 1    | 0                       | RELEASE 1    | 0         | 0         | off         |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | RELEASE 1    | 1                       | PRESS 2      | 0         | 0         | off         |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | PRESS 2      | 1                       | PRESS 2      | 0         | 0         | off         |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | PRESS 2      | 0                       | RELEASE 2    | 0         | 0         | off         |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | RELEASE 2    | 0                       | RELEASE 2    | 0         | 0         | off         |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | RELEASE 2    | 1                       | PRESS 3      | 0         | 0         | off         |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | PRESS 3      | 1                       | PRESS 3      | 0         | 0         | off         |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | PRESS 3      | 0                       | RELEASE 3    | 0         | 0         | off         |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | RELEASE 3    | 0                       | ON           | 0         | 0         | off         |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | RELEASE 3    | 1                       | PRESS 1      | 0         | 0         | off         |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | ON           | 0                       | ON (2sec)    | 0         | 1         | vert        |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | ON           | 1                       | PRESS 1      | 0         | 1         | vert        |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+

#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>
#define debounce_time 20
#define DelaiVertAllume 2000
#define allumeVert 0x01
#define eteint 0x00

enum class State
{
    press1,
    release1,
    press2,
    release2,
    press3,
    release3,
    on
};

bool VerifAppuye()
{
    if (PIND & (1 << PD2))
    {
        _delay_ms(debounce_time);
        if (PIND & (1 << PD2))
        {
            return true;
        }
    }
    return false;
}

void init()
{
    DDRA |= (1 << PA0) | (1 << PA1);
    DDRD &= ~(1 << PD2);
}

int main()
{
    State state = State::press1;
    init();
    while (true)
    {
        switch (state) {
            case State::press1 :
                if (VerifAppuye())
                {
                    state = State::release1;
                }
                break;
            case State::release1 :
                if (VerifAppuye() == false)
                {
                    state = State::press2;
                }
                break;
            case State::press2 :
                if (VerifAppuye())
                {
                    state = State::release2;
                }
                break;
            case State::release2 :
                if (VerifAppuye() == false)
                {
                    state = State::press3;
                }
                break;
            case State::press3 :
                if (VerifAppuye())
                {
                    state = State::release3;
                }
                break;
            case State::release3 :
                if (VerifAppuye() == false)
                {
                    state = State::on;
                }
                break;
            case State::on :
                PORTA = allumeVert;
                _delay_ms(DelaiVertAllume);
                state = State::press1;
                PORTA = eteint;
                break;
        }
    }
}
