#include "Moteur.h"

Moteur::Moteur(byte pin1, byte pin2, byte pin_vitesse) : pin1_(pin1), pin2_(pin2), pin_vitesse_(pin_vitesse)
{
  pinMode(pin1_, OUTPUT);
  pinMode(pin2_, OUTPUT);
  digitalWrite(pin1_, LOW);
  digitalWrite(pin2_, LOW);
  analogWrite(pin_vitesse_,0);
}

void Moteur::avancer(byte vitesse)
{
  digitalWrite(pin1_, HIGH);
  digitalWrite(pin2_, LOW);
  analogWrite(pin_vitesse_, vitesse);
}

void Moteur::reculer(byte vitesse)
{
  digitalWrite(pin1_, LOW);
  digitalWrite(pin2_, HIGH);
  analogWrite(pin_vitesse_, vitesse);
}
void Moteur::arret()
{
  digitalWrite(pin1_, LOW);
  digitalWrite(pin2_, LOW);
}
