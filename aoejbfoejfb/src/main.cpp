
#include <Arduino.h>

#include <pins.h>
#include <asservissement.h>
#include <TimerThree.h>


bool test;

void setup()
{
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(CodeurDroit_A), InterruptionCodeurDroit_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CodeurDroit_B), InterruptionCodeurDroit_B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CodeurGauche_A), InterruptionCodeurGauche_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CodeurGauche_B), InterruptionCodeurGauche_B, CHANGE);

  Timer3.initialize(200000); // l'interuption timer se déclenchera toute les 0.2ms
  Timer3.attachInterrupt(deplacement);
}

void loop()
{
  Serial.print(compteur_ticks[1]);
  Serial.print("  ");
  Serial.print(compteur[1]);
  Serial.print("  ");
  Serial.print(erreur_lin[1]);
  Serial.print("  ");
  Serial.print(commande_lin);
  Serial.print("  ");
  Serial.print(commande_PWM_G[1]);
  Serial.print("  ");
  Serial.println(commande_PWM_D[1]);
  delay(100);
}
