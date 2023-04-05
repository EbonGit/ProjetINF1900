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
    uint16_t val_ = (c.lecture(PORTA & 0x00));
    double val = (double)VREF/1024*val_;
    return val;
}

double IR::distanceCm(){
    return (29.988 * pow(getVolt(), -1.173));
}

double IR::distanceInch(){
    return (distanceCm()/2.54);
}