/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description:
 */
#include "IR.h"

IR::IR(){
    init();
}

void IR::init(){
    DEBUG_PRINT("init IR");
}

double IR::getVolt(){
    uint16_t val_1 = (c.lecture(PORTA & 0x00));
    _delay_ms(50);
    uint16_t val_2 = (c.lecture(PORTA & 0x00));
    // if(val_1 == 0 || val_2 == 0){
    //     return 0;
    // }
    if(val_1 <= val_2){
        return (double)VREF/1023*(val_2);
    }
    else{
        return (double)VREF/1023*(val_1);
    }
    
    
    //double val = (double)VREF/1024*((val_1+val_2)/2);
    //return val;
}

double IR::distanceCm(){
    return 27.0 * pow(getVolt(), -1.2);
}

double IR::distanceInch(){
    return (distanceCm()/2.54);
}