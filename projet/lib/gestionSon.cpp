/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */

#include "gestionSon.h"


GestionSon::GestionSon(){
    ajusterSon(82); // soundOff
}

int GestionSon::calculerTop(int index){
    return (int)round(F_CPU / (freq[index - 45] * DIV_CLOCK)); //calcul pour la fréquence
}

void GestionSon::ajusterSon(int a){

    int top = calculerTop(a);
   
    OCR2A = top;
    OCR2B = top;

    TCCR2A = (1 << WGM21 | 0 << WGM20 | 0 << WGM22 | 0 << COM2A1 | 1 << COM2A0| 1 << COM2B1 | 0 << COM2B0);

    TCCR2B = (1 << CS22 | 1 << CS20); // clock
}

void GestionSon::stopSon(){
    ajusterSon(82);
}