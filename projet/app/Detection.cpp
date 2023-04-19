/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: description des fonctions de la phase detection du controlleur.
 */

#include "Controlleur.h"

void Controlleur::detecter(){

    switch (etatDetection_)

    {

    case EtatDetection::ATTENDRE_1:

        robot_->led.changerCouleurAmbre(robot_->timer, ratioVert, ratioRouge, 1); //allume led orange (7 vert et 3 rouge 1 fois)

        switch (bouton)

            {

            case TypeBouton::AUCUN:

                break;


            case TypeBouton::INTERUPT:

                robot_->led.changerCouleur(EtatLed::OFF);

                DEBUG_PRINT("INTERUPT");

                bouton = TypeBouton::AUCUN;

                etatDetection_ = EtatDetection::HAUT;

                break;

            

            case TypeBouton::EXTERNE:

                robot_->led.changerCouleur(EtatLed::OFF);

                DEBUG_PRINT("EXTERNE");

                bouton = TypeBouton::AUCUN;

                etatDetection_ = EtatDetection::DROIT;

                break;

            }

        break;

    

    case EtatDetection::HAUT:

        DEBUG_PRINT("HAUT");

        orientationN_ = ZERO * ENTIER;

        updateOrientation();

        robot_->led.changerCouleur(EtatLed::VERT);

        _delay_ms(DEUX_SECONDE);

        robot_->led.changerCouleur(EtatLed::OFF);

        etatDetection_ = EtatDetection::RECHERCHE;

        break;

    case EtatDetection::DROIT:

        DEBUG_PRINT("DROIT");

        orientationN_ = ENTIER / 4; //quart de tour = EST

        updateOrientation();

        robot_->led.changerCouleur(EtatLed::ROUGE);

        _delay_ms(DEUX_SECONDE);

        robot_->led.changerCouleur(EtatLed::OFF);

        etatDetection_ = EtatDetection::RECHERCHE;

        break;

    case EtatDetection::RECHERCHE:

        distance_ = inconnu;

        nombreTour = 0;
        
        rechercher(0, RANGE_PETIT);

        if(distance_ == inconnu){
            robot_->moteur.ajustementPWM(ZERO, ZERO);
            _delay_ms(UNE_SECONDE);

            nombreTour = 0;
            
            rechercher(1, RANGE_GRAND);

            if(distance_ == inconnu){

            robot_->moteur.ajustementPWM(ZERO, ZERO);

            etatDetection_ = EtatDetection::NON_TROUVE;

            break;

            }

        }

        enregistrerPoteau();

        updateOrientation();

        poteauBoussole = orientation_;

        etatDetection_ = EtatDetection::TROUVE;

        break;

    case EtatDetection::TROUVE:

        if(distance_ == 0){

            DEBUG_PRINT("PETIT");

        }

        if(distance_ == 1){

            DEBUG_PRINT("GRAND");

        }

        etatDetection_ = EtatDetection::AVANCER;

        break;

    //TODO si ne trouve pas

    case EtatDetection::NON_TROUVE:

        robot_->son.ajusterSon(60);
        _delay_ms(DEUX_SECONDE);
        robot_->son.stopSon();
        //led rouge 2Hz
        while(true){
            //DEBUG_PRINT("il y a rien");
            robot_->led.changerCouleur(EtatLed::ROUGE);
            _delay_ms(UNE_SECONDE/4);
            robot_->led.changerCouleur(EtatLed::OFF);
            _delay_ms(UNE_SECONDE/4);
        }

        break;

    case EtatDetection::AVANCER:

        suivre(SUIVRE_PARAM, RANGE_GRAND, DISTANCE_STOP);

        etatDetection_ = EtatDetection::PROCHE;

        break;

    case EtatDetection::PROCHE:

        DEBUG_PRINT("PROCHE");

        for (uint8_t i = 0; i < 3; i++) //3 bips aigus
        {
            robot_->son.ajusterSon(56);
            _delay_ms(DELAI_SON);
            robot_->son.stopSon();
            _delay_ms(DELAI_SON);
        }


        etatDetection_ = EtatDetection::ATTENDRE_2;

        break;

    case EtatDetection::ATTENDRE_2:

        switch (bouton)

            {

            case TypeBouton::AUCUN:  
                
                robot_->led.changerCouleurAmbre(robot_->timer, ratioVert, ratioRouge, SECONDE/(4*(ratioRouge + ratioVert)));
                robot_->led.changerCouleur(EtatLed::OFF);
                _delay_ms(DELAI_LED);

                break;

            

            case TypeBouton::INTERUPT:

                DEBUG_PRINT("INTERUPT");

                bouton = TypeBouton::AUCUN;

                distance_ = inconnu;

                orientationN_ = ZERO * ENTIER;

                updateOrientation();

                etatDetection_ = EtatDetection::RECHERCHE;

                break;

            

            case TypeBouton::EXTERNE:

                DEBUG_PRINT("EXTERNE");

                bouton = TypeBouton::AUCUN;

                break;

            }

        break;

    }

    
}

/*ancienne version
void Controlleur::rechercher(int valeur, int distanceLimite){

    int remisZero = 0;


    robot_->moteur.changerDirection(Direction::DROITE);

    robot_->moteur.ajustementPWM(200,200);

    _delay_ms(50);

    robot_->moteur.ajustementPWM(80,80);

    while(true){

        robot_->moteur.ajustementPWM(0,0);

        _delay_ms(10);

        robot_->moteur.ajustementPWM(100,100);

        remisZero++;

        float d = robot_->ir.distanceCm();

        if(d < distanceLimite && d != 0){

            DEBUG_PRINT(d);

            DEBUG_PRINT("HIT");

            //distance_ = valeur;

            break;

        }

        orientationN_++;
        nombreTour++;

        //DEBUG_PRINT(orientationN_);

        updateOrientation();

        if((valeur == 0 && nombreTour == ENTIER) || (valeur == 1 && nombreTour == ENTIER)){
            return;
        }

        if(remisZero > 15){
            distance_ = -1;
            certitude = 0;

        }

    }

    robot_->moteur.changerDirection(Direction::AVANT);

    robot_->moteur.ajustementPWM(0,0);

    if(certitude == 0){
        distance_ = valeur;
    }

    for (uint8_t i = 0; i < 8; i++)

    {

        tournerGauche();

    }

    certitude++;

    estCertain(valeur, distanceLimite);

}
*/

void Controlleur::rechercher(int valeur, int distanceLimite){
    while(true){
        robot_->moteur.changerDirection(Direction::DROITE);
        robot_->moteur.ajustementPWM(VITESSE, VITESSE);
        int d = (int)robot_->ir.distanceCm();
        //_delay_ms(10);
        robot_->moteur.ajustementPWM(ZERO, ZERO);
        if(d < distanceLimite && d != 0){
            if(estCertain(distanceLimite)){
                break;
            }
        }
        _delay_ms(DELAI_STOP);
        nombreTour++;
        orientationN_++;
        updateOrientation();

        if((valeur == 0 && nombreTour == ENTIER) || (valeur == 1 && nombreTour == ENTIER)){
            return;
        }

    }

    distance_ = valeur;
}

void Controlleur::afficherPoteau(){

    DEBUG_PRINT(distance_);

    switch (poteauBoussole)

    {

    case Boussole::NORD:

        DEBUG_PRINT("NORD");

        break;

    case Boussole::NORDEST:

        DEBUG_PRINT("NORDEST");

        break;

    case Boussole::EST:

        DEBUG_PRINT("EST");

        break;

    case Boussole::SUDEST:

        DEBUG_PRINT("SUDEST");

        break;

    case Boussole::SUD:

        DEBUG_PRINT("SUD");

        break;

    case Boussole::SUDOUEST:

        DEBUG_PRINT("SUDOUEST");

        break;

    case Boussole::OUEST:

        DEBUG_PRINT("OUEST");

        break;

    case Boussole::NORDOUEST:

        DEBUG_PRINT("NORDOUEST");

        break;

    }

}

void Controlleur::enregistrerPoteau(){

    robot_->memoire.ecriture(nombrePoteau, distance_);

    _delay_ms(TEMPS_MEMOIRE);

    robot_->memoire.ecriture(nombrePoteau + 1, orientation_);

    _delay_ms(TEMPS_MEMOIRE);

    nombrePoteau = nombrePoteau + 2;

}
/*
void Controlleur::estCertain(int valeur, int distanceLimite){

    bool trouve = true;

    orientationN_ = orientationN_ - 5; //balance
    nombreTour = nombreTour - 5;

    for (uint8_t i = 0; i < 4; i++)

    {

        int distance = (int)robot_->ir.distanceCm();

        _delay_ms(50);

        if(distance > distanceLimite){

            trouve = false;

        }

    }

    if(certitude > 3){

        //distance_ = valeur;

    }

    else{

        if(!trouve){
            //distance_ = -1;
            rechercher(valeur, distanceLimite);

        }

        else{

            
        }

    }

}
*/
bool Controlleur::estCertain(int distanceLimite){
    for (int i = 0; i < decalageCertain; i++)
    {
        tournerGauche();
    }
    bool result = false;
    for (int i = 0; i < centrageCertain; i++)
    {   
        int d = (int)robot_->ir.distanceCm();
        if(d < distanceLimite && d != 0){
            result = true;
        }
        tournerDroite();
    }
    return result;
    
    
}