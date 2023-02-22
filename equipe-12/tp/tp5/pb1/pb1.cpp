/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi
 * Description: ecrit et lit dans la mémoire externe
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

// bool boutonMemoire;

enum LedState
{
    RED = (1 << PA0),
    GREEN = (1 << PA1),
    OFF = 0x00
};

char mots[46] = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*";


void init()
{
    DDRD = DDR_IN;
    DDRA = DDR_OUT;
}

int main()
{

    init();

    Memoire24CXXX m;

    for(int i = 0; i < 46; i++){
        m.ecriture(i, mots[i]);
        _delay_ms(5);
    }


    bool erreur = false;

    for(int i = 0; i < 46; i++){
        uint8_t temp;
        m.lecture(i, &temp);
        if(temp != mots[i]){
            erreur = true;
            break;
        }
        _delay_ms(5);
    }

    if(erreur == false){
        PORTA = LedState::GREEN;
    }
    else{
        PORTA = LedState::RED;
    }
    
    

    return 0;
}
