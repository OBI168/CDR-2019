
#include <Arduino.h>
#include <utility.h>


float compteur_ticks[2];




void InterruptionCodeurDroit_A()
{
  int Etat_A=digitalRead(CodeurDroit_A);
  int Etat_B=digitalRead(CodeurDroit_B);
  if (Etat_B==0)
  {
    if (Etat_A==0)
    {
      compteur_ticks[1]--;//recule
    }
    else
    {
      compteur_ticks[1]++;//avance
    }
  }
  else
  {
     if (Etat_A==0)
    {
      compteur_ticks[1]++;//avance
    }
    else
    {
      compteur_ticks[1]--;//Recule
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
      compteur_ticks[1]++;//avance
    }
    else
    {
      compteur_ticks[1]--;//Recule
    }
  }
  else
  {
     if (Etat_B==0)
    {
      compteur_ticks[1]--;//Recule
    }
    else
    {
      compteur_ticks[1]++;//avance
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
      compteur_ticks[0]--; //Recule
    }
    else
    {
      compteur_ticks[0]++; //avance
    }
  }
  else
  {
     if (Etat_A==0)
    {
      compteur_ticks[0]++;//avance
    }
    else
    {
      compteur_ticks[0]--;//Recule
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
      compteur_ticks[0]++; //avance
    }
    else
    {
      compteur_ticks[0]--; //Recule
    }
  }
  else
  {
     if (Etat_B==0)
    {
      compteur_ticks[0]--; //Recule
    }
    else
    {
      compteur_ticks[0]++; //avance
    }
  }
}
