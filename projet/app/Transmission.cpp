/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: description des fonctions de la phase transmission du controlleur.
 */
#include "Controlleur.h"

void Controlleur::transmettre(){
    switch (etatTransmission_)
    {
    case EtatTransmission::DEBUT:

        //ajout manuel de poteau dans la mémoire
        // robot_->memoire.ecriture(0, 0);
        // _delay_ms(25);
        // robot_->memoire.ecriture(1, 0);
        // _delay_ms(25);
        // robot_->memoire.ecriture(2, 0);
        // _delay_ms(25);
        // robot_->memoire.ecriture(3, 2);
        // _delay_ms(25);
        // robot_->memoire.ecriture(4, 0);
        // _delay_ms(25);
        // robot_->memoire.ecriture(5, 4);
        // _delay_ms(25);
        
        lireMemoire();
        robot_->rs232.transmissionUART(DEBUT_SVG); //début transmission SVG

        robot_->led.changerCouleur(EtatLed::VERT);
        _delay_ms(DEUX_SECONDE);

        etatTransmission_ = EtatTransmission::ENVOI;
        break;

    case EtatTransmission::ENVOI:

        SVG_Creator svg;
        svg.create(&generateur);

        robot_->rs232.transmissionUART(FIN_SVG); //fin transmission SVG

        uint32_t crc = svg.getCRC() ^ 0xFFFFFFFF;
        
        //envoi du crc32
        char buffer[taille_buffer];
        sprintf(buffer, "%lx", crc);
        DEBUG_PRINT(buffer);
        

        _delay_ms(TEMPS_CRC32);
        robot_->rs232.transmissionUART(FIN_RS232); //fin RS232
        etat_ = EtatRobot::INIT;
        break;

    }
}

void Controlleur::lireMemoire(){
    uint8_t tempDistance;
    uint8_t tempOrientation;
    uint8_t i = 0;

    while(tempDistance != fin){

        robot_->memoire.lecture(2*i, &tempDistance);
        DEBUG_PRINT(tempDistance);
        _delay_ms(TEMPS_MEMOIRE);

        robot_->memoire.lecture(2*i+1, &tempOrientation);
        DEBUG_PRINT(tempOrientation);
        _delay_ms(TEMPS_MEMOIRE);
        i++;
        if(tempDistance != fin){
            //ajout information du poteau dans le generateur de SVG
            generateur.detecter((GenerateurPoints::Direction)tempOrientation, tempDistance + 1);
        }
    }
    
}