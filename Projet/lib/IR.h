/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description:
 */
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "can.h"
#include "debug.h"

#define VREF 4.3 //IMPORTANT A CHANGER SI SUR PILE 9V

class IR
{
private:
    double getVolt();
    void init();
    
public:
    IR();
    double distanceInch();
    double distanceCm();
    can c = can();

};

