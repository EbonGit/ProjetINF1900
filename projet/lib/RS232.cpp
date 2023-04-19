/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */

#include "RS232.h"

void RS232::initialisationUART (void)
{
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

void RS232::transmissionUART(uint8_t donnee)
{
    while(!(UCSR0A & (1 << UDRE0)));
    donnee_ = donnee;
    UDR0 = donnee_;
}

uint8_t RS232::recevoirUART( void )
{
    while ( !(UCSR0A & (1<<RXC0)) );
    return UDR0;
}
