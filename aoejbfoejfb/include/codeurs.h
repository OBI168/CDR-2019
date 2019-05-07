#include <Arduino.h>
#include <pins.h>
#include <parametres.h>

float CompteurDroit;
float CompteurGauche;

float compteur_ticks[2]={0,0};



void InterruptionCodeurDroit_A()
{
  int Etat_A=digitalRead(CodeurDroit_A);
  int Etat_B=digitalRead(CodeurDroit_B);
  if (Etat_B==0)
  {
    if (Etat_A==0)
    {
    }
    else
    {
      CompteurDroit++;//avance
    }
  }
  else
  {
     if (Etat_A==0)
    {
      CompteurDroit++;//avance
    }
    else
    {
      CompteurDroit--;//Recule
    }
  }
}

void InterruptionCodeurDroit_B()
{
  int Etat_A=digitalRead(CodeurDroit_A);
  int Etat_B=digitalRead(CodeurDroit_B);
  if (Etat_A==0)
  {
    if (Etat_B==0)
    {
      CompteurDroit++;//avance
    }
    else
    {
      CompteurDroit--;//Recule
    }
  }
  else
  {
     if (Etat_B==0)
    {
      CompteurDroit--;//Recule
    }
    else
    {
      CompteurDroit++;//avance
    }
  }
}

void InterruptionCodeurGauche_A()
{
  int Etat_A=digitalRead(CodeurGauche_A);
  int Etat_B=digitalRead(CodeurGauche_B);
  if (Etat_B==0)
  {
    if (Etat_A==0)
    {
      CompteurGauche--;//Recule
    }
    else
    {
      CompteurGauche++;//avance
    }
  }
  else
  {
     if (Etat_A==0)
    {
      CompteurGauche++;//avance
    }
    else
    {
      CompteurGauche--;//Recule
    }
  }
}

void InterruptionCodeurGauche_B()
{
  int Etat_A=digitalRead(CodeurGauche_A);
  int Etat_B=digitalRead(CodeurGauche_B);
  if (Etat_A==0)
  {
    if (Etat_B==0)
    {
      CompteurGauche++;//avance
    }
    else
    {
      CompteurGauche--;//Recule
    }
  }
  else
  {
     if (Etat_B==0)
    {
      CompteurGauche--;//Recule
    }
    else
    {
      CompteurGauche++;//avance
    }
  }
}
