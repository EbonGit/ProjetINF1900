/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description:
 */
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DEBOUNCE_TIME 20

class Bouton
{
public:
    Bouton() = default;
    Bouton(int masque);
    void init();
    bool estActif();

private:
    bool peutChangerEtat_ = true;
    int masque_ = 0x04;
    bool debounce();
};