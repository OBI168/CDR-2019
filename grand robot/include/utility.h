#ifndef UT_H
#define UT_H

#include <Arduino.h>
#include <configuration.h>


void maj_data(float liste[2], float nouveau)
{
  liste[1]=liste[0];
  liste[0]=nouveau;
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
