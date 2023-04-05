/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */
#define F_CPU 8000000UL // 8 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <librairie.h>

#define DDR_IN 0x00
#define DDR_OUT 0xff

class Controlleur
{
private:
   Robot* robot_;
public:
    //Controlleur() = default;
    Controlleur(Robot*);
    void suivre();
    void virerDroite();
    void virerGauche();
    
};


