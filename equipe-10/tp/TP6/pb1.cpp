#define F_CPU 8000000UL
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>
#include "memoire_24.cpp"
#include "memoire_24.h"
using namespace std;
//b0 et b1 pour la DEL et d2 pour bouton