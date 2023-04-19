#define F_CPU 8000000UL // 8 MHz
#include "SVG_Creator.h"
#include <util/delay.h>

SVG_Creator::SVG_Creator() {
	
}

void SVG_Creator::create(GenerateurPoints* generator) {

	generator->trierPoints();
	generator->enveloppeConvexe();
	generator->calculatePolygonArea();
	
	transmitSVG(generator);

}


void transmissionUART ( uint8_t donnee ) {

    while(!(UCSR0A & (1 << UDRE0)));
    UDR0 = donnee;
    
}



void SVG_Creator::sendViaUSART(const char* text, int size) {
	//for (size_t j = 0; j < size; j++)
	//{
	//	cout << text[j];
	//}
	//cout << "fin de transmission" <<endl;
	//cout << text << endl;
	for(int i = 0; i < size; i++){
		
		if((i%10) == 0){
			PORTA = (1 << PA3);
			_delay_ms(50);
			PORTA = (0 << PA2) | (0 << PA3);
			_delay_ms(50);
		}
		
		crc ^= (*text);
        for (int j = 0; j < 8; j++) {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;
            else
                crc = (crc >> 1);
        }
		
		transmissionUART(*text++);
		//_delay_ms(5);
		
	}
}




void SVG_Creator::transmitSVG(GenerateurPoints* generator) {
	
	// transmit header
	for (size_t i = 0; i < 2; i++)
	{
		sendViaUSART(text_init[i], strlen(text_init[i]));
	}
	
	// transmit polygon line
	char line_polygon[256]="<polygon points='";

	for (size_t i = 0; i < MAX_POINTS; i++)
	{
		if (generator->enveloppe[i].estValide == false)
			break;
	
		// 110 pixels per inches + 96 pixels for centering + 95 pixels for top clearance
		int x = generator->enveloppe[i].x() * 110 + 95 + 96;
		// SVG y axis is reversed, 480 for total height - 110 pixels per inches + 75 pixels for left clearance + 48 for centering
		int y = 480 - (generator->enveloppe[i].y() * 110 + 75) + 48;

		char buffer[50];
		sprintf(buffer, " %d,%d", x, y);
		strcat(line_polygon, buffer);
	}
	strcat(line_polygon, "' fill='green' stroke='black' stroke-width='2'/>");
	sendViaUSART(line_polygon, strlen(line_polygon));
	
	// transmit squares
	double start_x = 188.5;
	double start_y = 120.5;
	double x;
	for (size_t i = 0; i < 4; i++)
	{
		x = start_x;
		for (size_t i = 0; i < 8; i++)
		{
			char buffer[256] = "<rect width='5' height='5'";

			char b1[20];
			int intx = (int)x;
			int decx = (int)(x * 10) % 10;
			sprintf(b1, " x='%d.%d'", intx, decx);

			char b2[50];
			int inty = (int)start_y;
			int decy = (int)(start_y * 10) % 10;
			sprintf(b2, " y='%d.%d' stroke='black' fill='black'/>", inty, decy);
			
			strcat(buffer, b1);
			strcat(buffer, b2);
			
			sendViaUSART(buffer, strlen(buffer));
			x += 110;
		}
		start_y += 110;
	}
	
	
	// transmit circles
	for (size_t i = 0; i < MAX_POINTS; i++)
	{
		if (generator->sorted_points[i].estValide == false)
			break;
		char line[256];
		// 110 pixels per inches + 96 pixels for centering + 95 pixels for top clearance
		int x = generator->sorted_points[i].x() * 110 + 95 + 96;
		// SVG y axis is reversed, 480 for total height - 110 pixels per inches + 75 pixels for left clearance + 48 for centering
		int y = 480 - (generator->sorted_points[i].y() * 110 + 75) + 48;
		sprintf(line, "<circle cx='%d' cy='%d' r='10' stroke='black' stroke-width='2' fill='grey'/>", x, y);
		sendViaUSART(line, strlen(line));
	}
	
	
	// transmit bottom text
	sendViaUSART(text_bottom, strlen(text_bottom));
	
	
	// transmit area
	char line_area[256];
	sprintf(line_area, "<text x='96' y='550' font-family='arial' font-size='20' fill='blue'>AIRE : %d pouces carrees </text>", generator->area_);
	sendViaUSART(line_area, strlen(line_area));

	// transmit end
	sendViaUSART(text_end, strlen(text_end));

}