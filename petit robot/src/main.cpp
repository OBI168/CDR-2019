#include <Arduino.h>
#include<TimerThree.h>
#include <asservissement.h>


void setup()
{
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(CodeurDroit_A), InterruptionCodeurDroit_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CodeurDroit_B), InterruptionCodeurDroit_B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CodeurGauche_A), InterruptionCodeurGauche_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CodeurGauche_B), InterruptionCodeurGauche_B, CHANGE);

  Timer3.initialize(200000); // l'interuption timer se déclenchera toute les 0.2s
  Timer3.attachInterrupt(deplacement);
}

void loop()
{
    // MD.avancer(70);
    // MG.avancer(70);
    // Serial.print(" ticksG:  ");
    // Serial.print(compteur_ticks[0]);
    // Serial.print(" ticksD:  ");
    // Serial.print(compteur_ticks[1]);
    // Serial.println(" ");
}
