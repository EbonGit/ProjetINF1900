#include "debug.h"
#include "librairie.h"
#include <avr/io.h>
#include <stdio.h>

#define TEMPS_ATTENDRE 5

RS232 RS;
bool est_init = false;

void printRS232(int n){
    
    if(!est_init){
        RS.initialisationUART();
        est_init = true;
    }
   
    char s[10];
    sprintf(s, "%d", n);
    int i = 0;
    do
    {  
        RS.transmissionUART(s[i]);
        _delay_ms(TEMPS_ATTENDRE);
        i++;
    }while(s[i] != '\0');

    RS.transmissionUART('\n');
    _delay_ms(TEMPS_ATTENDRE);
    
}

void printRS232(char c[]){
    
    if(!est_init){
        RS.initialisationUART();
        est_init = true;
    }

    int i = 0;
    do
    {  
        RS.transmissionUART(c[i]);
        _delay_ms(TEMPS_ATTENDRE);
        i++;
    }while(c[i] != '\0');

    RS.transmissionUART('\n');
    _delay_ms(TEMPS_ATTENDRE);
}

