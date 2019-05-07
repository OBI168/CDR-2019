
#include <Arduino.h>

#include <pins.h>
#include <asservissement.h>
#include <TimerThree.h>




void setup()
{
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(CodeurDroit_A), InterruptionCodeurDroit_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CodeurDroit_B), InterruptionCodeurDroit_B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CodeurGauche_A), InterruptionCodeurGauche_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CodeurGauche_B), InterruptionCodeurGauche_B, CHANGE);

  Timer3.initialize(50000); // délai
  Timer3.attachInterrupt(asser(consigne_lin, consigne_rot));
}

void loop()
{

}
