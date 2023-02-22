#define F_CPU 8000000UL
#include <avr/io.h> 
#include <util/delay.h>
#include "memoire_24.h"

const uint16_t adresse = 0x0000;
uint8_t donne[] = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*";
const uint8_t longueur= sizeof(donne)/sizeof(uint8_t);
uint8_t donnee[longueur] ="3";

void init()
{
    DDRA |= (1 << PA0) | (1 << PA1); // force les bits a 1
    DDRD &= ~(1 << PD2);             // force les bits a 0
}

int main (){
    init();
    Memoire24CXXX objet;
    objet.ecriture(
    adresse, 
    donne,
    longueur);

    _delay_ms(5);

    objet.lecture(
    adresse, 
    donnee,
    longueur);

    for( int i = 0; i < longueur ; i++ )  {
        if (donne[i] != donnee[i])
            break;
        else
            PORTA = 0x01;
    }
   

}
