// Auteurs: GHABIA Mohammed Amine et ZEROUALI Amine
// Description du programme: Programation d'une machine a etats simples avec une DEL bicolore.
//  Entrees: PORTD2 (l'interrupteur)
//  Sorties: PORTA0, PORTA1 (la DEL)
// table d'etat : 
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | Etat présent | Entrée D (interrupteur) | Etat suivant | Sortie A1 | Sortie A0 | Couleur LED |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | OFF          | 0                       | OFF          | 1         | 0         | rouge       |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | OFF          | 1                       | PRESS 1      | 1         | 0         | rouge       |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | PRESS 1      | 1                       | PRESS 1      | x         | x         | ambre       |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | PRESS 1      | 0                       | RELEASE 1    | x         | x         | ambre       |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | RELEASE 1    | 0                       | RELEASE 1    | 0         | 1         | vert        |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | RELEASE 1    | 1                       | PRESS 2      | 0         | 1         | vert        |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | PRESS 2      | 1                       | PRESS 2      | 1         | 0         | rouge       |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | PRESS 2      | 0                       | RELEASE 2    | 1         | 0         | rouge       |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | RELEASE 2    | 0                       | RELEASE 2    | 0         | 0         | eteint      |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | RELEASE 2    | 1                       | PRESS 3      | 0         | 0         | eteint      |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | PRESS 3      | 1                       | PRESS 3      | 0         | 1         | vert        |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | PRESS 3      | 0                       | RELEASE 3    | 0         | 1         | vert        |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | RELEASE 3    | 0                       | RELEASE 3    | 0         | 1         | vert        |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+
// | RELEASE 3    | 1                       | OFF          | 0         | 0         | eteint      |
// +--------------+-------------------------+--------------+-----------+-----------+-------------+

#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>
#define debounce_time 20
#define DelaiVertAllume 2000
#define vert 0x01
#define rouge 0x02
#define eteint 0x00
#define delaiChangementCouleur 5

enum class State
{
	off,
	press1,
	release1,
	press2,
	release2,
	press3,
	release3
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
	State state = State::off;
	init();
	while (true)
	{
		switch (state) {
			case State::off :
				PORTA = rouge;
				if (VerifAppuye())
				{
					state = State::press1;
				}
				break;
			case State::press1 :
				while (VerifAppuye())
				{
					PORTA = rouge;
					_delay_ms(delaiChangementCouleur);
					PORTA = vert;
					_delay_ms(delaiChangementCouleur);
				}
				if (VerifAppuye() == false)
				{
					state = State::release1;
				}
				break;
			case State::release1 :
				PORTA = vert;
				if (VerifAppuye())
				{
					state = State::press2;
				}
				break;
			case State::press2 :
				PORTA = rouge;
				if (VerifAppuye() == false)
				{
					state = State::release2;
				}
				break;
			case State::release2 :
				PORTA = eteint;
				if (VerifAppuye())
				{
					state = State::press3;
				}
				break;
			case State::press3 :
				PORTA = vert;
				if (VerifAppuye() == false)
				{
					state = State::release3;
				}
				break;
			case State::release3 :
				state = State::off;
				break;
		}
	}
}
