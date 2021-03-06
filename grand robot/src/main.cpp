#include <Arduino.h>
#include<TimerThree.h>
#include <asservissement.h>

float temps = 1000;

void stop()
{
    MD.arret();
    MG.arret();
    pinMode(44, INPUT);
}

void setup()
{
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(CodeurDroit_A), InterruptionCodeurDroit_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CodeurDroit_B), InterruptionCodeurDroit_B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CodeurGauche_A), InterruptionCodeurGauche_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CodeurGauche_B), InterruptionCodeurGauche_B, CHANGE);

  Timer3.initialize(200000); // l'interuption timer se déclenchera toute les 0.2s
  Timer3.attachInterrupt(asservissement);
}

void loop()
 {
//     boolean sign =digitalRead(start);
//
//     while(!sign)
// {
    // MD.avancer(120);
    // MG.avancer(120);
    // Serial.print(" ticksG:  ");
    // Serial.print(compteur_ticks[0]);
    // Serial.print(" ticksD:  ");
    // Serial.print(compteur_ticks[1]);
    // float rot = calcul_position_rot(compteur_ticks);
    // Serial.print(" posRot; ");
    // Serial.print(rot);
    // Serial.println(" ");
//}
}
