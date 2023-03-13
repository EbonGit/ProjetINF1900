#define F_CPU 8000000UL // 8 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <librairie.h>


int main(){

    RS232 rs;
    rs.initialisationUART();

    Memoire24CXXX m;

    int index = 0;
    uint8_t temp;
    
    
   
    
    while(temp != 0xFF){
        temp = rs.recevoirUART();
        m.ecriture(index, temp);
        _delay_ms(5);
        index++;
    }

    while(true){
        int index_ = 0;
       

        for(int i = 0; i < index ; i++){
            m.lecture(index_, &temp);
            rs.transmissionUART(temp);
            _delay_ms(50);
            index_++;
        }  
        _delay_ms(2000);
    }  

    return 0;
}