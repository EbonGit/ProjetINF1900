/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi
 * Description: transmission par RS232 avec le PC
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

    char mots[21] = "Le robot en INF1900\n";

    uint8_t i, j;

    

    for ( i = 0; i < 100; i++ ) {

        for ( j=0; j < 20; j++ ) {

            transmissionUART ( mots[j] );

        }

    }
    

    return 0;
}
