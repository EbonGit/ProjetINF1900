/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */
#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include <timer1.h>

enum EtatLed
{
    ROUGE = (1 << PA2),
    VERT = (1 << PA3),
    OFF = (0 << PA2) | (0 << PA3)
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