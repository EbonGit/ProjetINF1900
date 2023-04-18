/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */
#include "Detection.h"

#define DEBUT_SVG 0x02

#define FIN_SVG 0x03

#define FIN_RS232 0x04

const uint8_t taille_buffer = 16;

#define TEMPS_CRC32 100


enum EtatTransmission
{
    DEBUT,
    ENVOI
};