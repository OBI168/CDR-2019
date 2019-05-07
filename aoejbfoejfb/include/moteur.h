#include <Arduino.h>

class Moteur
{
  public :
    Moteur(byte pin1, byte pin2, byte pin_vitesse);
    void arret();
    void avancer(byte vitesse);
    void reculer(byte vitesse);
  private :
    int sens;
    byte pin1_;
    byte pin2_;
    byte pin_vitesse_;
};
