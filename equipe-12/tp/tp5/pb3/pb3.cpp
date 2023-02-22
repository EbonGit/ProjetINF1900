/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi
 * Description: lire/ecrire mémoire + transmission avec le PC
 * Broche IO: Sorties A1 A2
 *
 */

#define F_CPU 8000000UL // 8 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h> 
#include "memoire_24.h"

#define DDR_IN 0x00
#define DDR_OUT 0xff
#define DEBOUNCE_TIME 20
#define WAIT_TIME 2000

#define TXEN0 3
#define RXEN0 4

#define UCSZ02 2
#define UCSZ01 2
#define UCSZ00 1
// bool boutonMemoire;

enum LedState
{
    RED = (1 << PA0),
    GREEN = (1 << PA1),
    OFF = 0x00
};

char mots[46] = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*";


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

PORTA = LedState::GREEN;

}

void transmissionUART ( uint8_t donnee ) {

    while(!(UCSR0A & (1 << UDRE0)));

    UDR0 = donnee;
    

}

void init()
{
    DDRD = DDR_IN;
    DDRA = DDR_OUT;

    initialisationUART();
}

int main()
{

    init();

    Memoire24CXXX m;

    int i = 0;
    for(i; i < 46; i++){
        m.ecriture(i, mots[i]);
        _delay_ms(5);
    }
    i = 0;

    uint8_t temp;
   
    while(temp != 0xFF){
        transmissionUART ( temp );
        m.lecture(i, &temp);
        i++;
        

        _delay_ms(5);
    }
    

    return 0;
}
