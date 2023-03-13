#define F_CPU 8000000UL // 8 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <librairie.h>


void init(){

}

int main(){
    Memoire24CXXX m;

    int index = -1;

    uint8_t nOctets1;
    uint8_t nOctets2;

    m.lecture(++index, &nOctets1);
    m.lecture(++index, &nOctets2);

    uint16_t nOctets = (((uint16_t)nOctets1 << 8) | nOctets2) - 2;

    DEBUG_PRINT(nOctets);

    init();

    for (int i = 0; i < (nOctets/2); i++)
    {
        uint8_t opcode;
        uint8_t arg;

        m.lecture(++index, &opcode);

        if(opcode != 0xFF){
            m.lecture(++index, &arg);
        }

        switch (opcode)
        {
        case 0x01:
            DEBUG_PRINT("DEBUT");
            break;
        
        case 0x02:
            DEBUG_PRINT("ATTENDRE");
            DEBUG_PRINT(arg);
            break;

        case 0x44:
            DEBUG_PRINT("ALLUMER LED");
            DEBUG_PRINT(arg);
            break;
        
        case 0x45:
            DEBUG_PRINT("ETEINDRE LED");
            break;
        
        case 0x48:
            DEBUG_PRINT("JOUER SON");
            DEBUG_PRINT(arg);
            break;

        case 0x09:
            DEBUG_PRINT("STOP SON");
            break;

        case 0x60:
            DEBUG_PRINT("STOP MOTEUR");
            break;
        
        case 0x61:
            DEBUG_PRINT("STOP MOTEUR");
            break;

        case 0x62:
            DEBUG_PRINT("AVANCER");
            DEBUG_PRINT(arg);
            break;

        case 0x63:
            DEBUG_PRINT("RECULER");
            DEBUG_PRINT(arg);
            break;

        case 0x64:
            DEBUG_PRINT("TOURNER DROITE");
            break;

        case 0x65:
            DEBUG_PRINT("TOURNER GAUCHE");
            break;

        case 0xC0:
            DEBUG_PRINT("DEBUT BOUCLE");
            DEBUG_PRINT(arg);
            break;

        case 0xC1:
            DEBUG_PRINT("FIN BOUCLE");
            break;
        
        case 0xFF:
            DEBUG_PRINT("FIN");
            break;
        
        }
    }
    

    return 0;
}