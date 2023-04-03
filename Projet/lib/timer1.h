/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */
#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#define CSN12 2

class Timer1
{
public:
    Timer1(volatile int* gMinuterieExpiree);
    Timer1() = default;
    void attendre(int duree);
    void partirMinuterie(int duree);
    void setVolatile(volatile int* v){gMinuterieExpiree_ = v;};

private:
    volatile int* gMinuterieExpiree_;
};