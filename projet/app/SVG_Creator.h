#pragma once

#include <string.h>

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h> 

#include "GenerateurPoints.h"

using namespace std;

#define F_CPU 8000000UL // 8 MHz

#define TXEN0 3
#define RXEN0 4

#define UCSZ02 2
#define UCSZ01 2
#define UCSZ00 1

class SVG_Creator
{
public:
	SVG_Creator();

    void create(GenerateurPoints* generator);
    void transmitSVG(GenerateurPoints*);
    uint32_t getCRC(){ return crc; };
    void sendViaUSART(const char* text, int size);
private:
	const char* text_init[2] = {
	"<svg width='100%' height='100%' xmlns='http://www.w3.org/2000/svg' viewbox='0 0 1152 576'>",
    "<rect width='960' height='480' x='96' y='48' stroke='black' fill='none'/>"
	};
    const char* text_bottom = "<text x='96' y='36' font-family='arial' font-size='20' fill='blue'>section 01 --equipe 1012 --K - RIM</text>";
    const char* text_end = "</svg>";
    uint32_t crc = 0xFFFFFFFF;
};

