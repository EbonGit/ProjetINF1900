/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */

#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#define WGM0 0

enum Direction
{
    AVANT = 0,
    ARRIERE = (1 << PB2 | 1 << PB5),
    DROITE = (0 << PB2 | 1 << PB5),
    GAUCHE = (1 << PB2 | 0 << PB5)
};

class Moteur
{
public:
    Moteur() = default;
    //void init();
    void ajustementPWM(int a, int b);
    void changerDirection(Direction d);
    Direction getDirection(void);


private:
    Direction direction_ = Direction::AVANT;
    int vitesse_a_ = 0;
    int vitesse_b_ = 0;
};