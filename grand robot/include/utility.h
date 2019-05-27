#ifndef UT_H
#define UT_H

#include <Arduino.h>
#include <configuration.h>


void maj_data(float liste[2], float nouveau)
{
  liste[1]=liste[0];
  liste[0]=nouveau;
}


float cm_to_ticks(float dist)
{
return dist/resolution;
}


float deg_to_rad(float angle)
{
  return (angle*pi/180);
}


float calcul_cons_rot(float cons)
  {
  float angle_deg = deg_to_rad(cons);
  return entraxe*sin(angle_deg);
  }
/*
calcul la somme d'une liste de deux éléments
*/

float calcul_integrale(float liste[2])
{
  return liste[0]+liste[1];
}

/*
calcul la différence entre les éléments d'une liste de deux éléments
*/
float calcul_derive(float liste[2])
{
  return liste[0]-liste[1];
}

/*
retourne True si le nombre est positive ou nul
*/
boolean positif(float x)
{
  if(x>=0)
    return true;
  else
    return false;
}

#endif
