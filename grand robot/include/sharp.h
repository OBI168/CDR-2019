#include <Arduino.h>
#include <configuration.h>


bool detection_AV ;
bool detection_AR;

bool Detection_obstacle_AV()
{

 if (analogRead(SharpAV1)*res > seuil || analogRead(SharpAV2)*res> seuil) // Si le SHARP detecte un obstacle
  {
    detection_AV = false;
    // Serial.print(" AV1: ");
    // Serial.print(analogRead(SharpAV1));
    // Serial.print(" AV2: ");
    // Serial.print(analogRead(SharpAV2));
  }
 else//sinon c'est bon
  {
    detection_AV=false;
  }
  return detection_AV;
}

bool Detection_obstacle_AR()
{
  if (analogRead(SharpAR1)*res> seuil ||analogRead(SharpAR2)*res> seuil )
  {
    detection_AR = false ;

  }
  else
  {
    detection_AR = false;
  }
  return detection_AR;
}
