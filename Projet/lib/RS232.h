/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */
#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>

class RS232
{
public:
    RS232() = default;
    void initialisationUART(void);
    void transmissionUART(uint8_t donnee);
    uint8_t recevoirUART(void);

private:
    uint8_t donnee_;
};
