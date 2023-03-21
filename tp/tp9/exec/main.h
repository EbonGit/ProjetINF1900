/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */
#define F_CPU 8000000UL // 8 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <librairie.h>

#define DDR_IN 0x00
#define DDR_OUT 0xff

#define DIV_CLOCK 1024
#define MS25 195
#define TEMPS_90DEG 3250

class GestionSon{
public:
    GestionSon();
    float freq[38] = {110, 116.54, 123.47, 130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185.0, 196.0, 207.65,
                    220.0, 233.08, 246.94, 261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.0,
                    415.30, 440, 466.16, 493.88, 523.25, 554.37, 587.33, 622.25, 659.26, 698.46, 739.99,
                    783.99, 830.61, 880.0, 0};

    int longueurMusic = 9;
    // D4 D4 D5 A4 AB4 G4 F4 D4 F4 G4
    int music[10] = {62, 62, 74, 69, 70, 67, 65, 62, 65, 67};

    int calculerTop(int);
    void ajusterSon(int);
    void stopSon();
};


class Programme
{
private:
    int index_ = -1;
    bool actif_ = false;
protected:
    uint16_t nOctets_;
public:
    //constructeur
    Programme() = default;
    //setter-getter
    int getIndex() {return index_;};
    int getIndexpp() { return ++index_;};
    bool getActif() {return actif_;};
    void setIndex(int i) {index_ = i;};
    void setActif(bool b) {actif_ = b;};
    //methodes
    virtual void lireNombreOctets() = 0;
    bool estFini();

};

class Robot : public Memoire24CXXX, 
              public Led, 
              public Timer1, 
              public Moteur, 
              public GestionSon, 
              public Programme
{
private:
public:
    //constructeur
    Robot(volatile int*);
    //getter-setter
    int getOctets() {return nOctets_;};
    //methodes
    void sequenceDebut();
    void lireNombreOctets() override;
};