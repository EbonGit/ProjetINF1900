/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */
#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include <timer1.h>

enum EtatLed
{
    ROUGE = (1 << PA0),
    VERT = (1 << PA1),
    OFF = 0x00
};

class Led
{
public:
    Led() = default;
    Led(EtatLed l);
    void changerCouleur(EtatLed l);
    void changerCouleurAmbre(Timer1 t, int t_vert, int t_rouge, int n);
    EtatLed recupererCouleur();
private:
    EtatLed couleur_;
};