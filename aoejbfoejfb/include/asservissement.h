#include <Arduino.h>
#include <codeurs.h>
#include <moteur.h>
#include <math.h>

Moteur MD(MoteurDroit_1,MoteurDroit_2,PWMMoteurDroit);
Moteur MG(MoteurGauche_1,MoteurGauche_2,PWMMoteurGauche);

/*
l'indice n d'une liste de taille n indque la valeur échantilloné à l'instant t
l'indice n-1 indique l'échantillon à l'instant n-1
*/
float commande_lin;
float commande_rot;

float new_erreur_lin;  //nouvelle erreur linéaire
float new_erreur_rot; //nouvelle ereeur linéaire
float int_erreur_rot; //intégrale de l'erreur de rotation
float int_erreur_lin; //intégrale de l'erreur linéaire
float der_erreur_rot; //dérivé de l'erreur de rotation
float der_erreur_lin; //dérivé de l'erreur linéaire
float position_lin;   //position linéaire (en cm)
float position_rot;   //angle de roation (en degré)



float compteur[2]={0,0};  // liste de la position gauche/droite en cm
float commande_mot_D[2]={0,0}; //commande du moteur droit
float commande_mot_G[2]={0,0}; //commande du moteur gauche
float commande_PWM_D[2]={0,0};  // commande du moteur droit en PWM
float commande_PWM_G[2]={0,0};  // commande du moteur gauche en PWM

float erreur_lin[2]={0,0}; // liste qui contient l'erreur linéaire à l'instant n et n-1
float erreur_rot[2]={0,0};  // liste qui contient l'erreur de rotation à l'instant n et n-1

float consigne_lin=70; // consigne de la distance rectiligne en cm
float consigne_rot=0; // cosigne d'angle en degré




/*
convertit les ticks en cm
*/
float tick_to_cm(float ticks[2])
{
  float cm[2];
  for(int i=0; i>2; i++)
  {
    cm[i]=resolution*ticks[i];
  }
  return *cm;
}

/*
calcul l'angle de rotation du robot en
*/
float calcul_position_rot(float liste[2])
{
  return 180*(atan(liste[2]-liste[1])/entraxe)/pi;
}

/*
calcul la position du robot
*/
float calcul_position_lin(float liste[2])
{
  return (liste[2]-liste[1])/2;
}

/*
mets à jour une liste avec une nouvelle valeur
@liste : liste à modifier
@nouveau : nouvelle valeur à mettre dans la liste
*/
void maj_data(float liste[2], float nouveau)
{
  liste[0]=liste[1];
  liste[1]=nouveau;
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
  return liste[1]-liste[0];
}

float calcul_erreur(float consigne, float position)
{
  return consigne-position;
}

float commande_to_PWM(float commande)
{
  return commande/vitesse_max*PWM_max;
}


float saturation_acc(float liste[2])
{
  float acceleration = calcul_derive(&liste[2]);
  if (acceleration>acceleration_max)
  {
    return acceleration_max;
  }
  else
  {
    return liste[1];
  }
}

void deplacement()
{
      float commande_mot_Dt;
      float commande_mot_Gt;

      float commande_PWM_Dt;
      float commande_PWM_Gt;



      // float commande_lin;
      // float commande_rot;

      float position_lin_cm[2];

      //position_lin_cm = tick_to_cm(compteur_ticks[2]);

      position_lin=calcul_position_lin(compteur);
      position_rot= calcul_position_rot(compteur);

      new_erreur_lin = calcul_erreur(consigne_lin, position_lin);
      new_erreur_rot = calcul_erreur(consigne_rot, position_rot);

      maj_data(erreur_lin, new_erreur_lin);
      maj_data(erreur_rot, new_erreur_rot);

      int_erreur_lin = calcul_integrale(erreur_lin);
      der_erreur_lin = calcul_derive(erreur_lin);

      int_erreur_rot = calcul_integrale(erreur_rot);
      der_erreur_rot = calcul_derive(erreur_rot);

      commande_lin = Kp_lin*erreur_lin[1] + Ki_lin*int_erreur_lin + Kd_lin*der_erreur_lin;
      commande_rot = Kp_rot*erreur_rot[1] + Ki_rot*int_erreur_rot + Kd_rot*der_erreur_rot;

      commande_mot_Dt = constrain(commande_lin + commande_rot,-vitesse_max,vitesse_max);
      commande_mot_Gt = constrain(commande_lin + commande_rot,-vitesse_max,vitesse_max);

      maj_data(commande_mot_D, commande_mot_Dt);
      maj_data(commande_mot_G, commande_mot_Gt);

      commande_mot_Dt = saturation_acc(commande_mot_D);
      commande_mot_Gt = saturation_acc(commande_mot_G);

      commande_PWM_Dt = commande_to_PWM(commande_mot_Dt);
      commande_PWM_Gt = commande_to_PWM(commande_mot_Gt);

      maj_data(commande_PWM_D, commande_PWM_Dt);
      maj_data(commande_PWM_G, commande_PWM_Gt);

      if(commande_PWM_D>0 && commande_PWM_G>0)
      {
        MD.avancer(commande_PWM_D[1]);
        MG.avancer(commande_PWM_G[1]);
      }
      else if(commande_PWM_D<=0 && commande_PWM_G<=0)
      {
        MD.reculer(commande_PWM_D[1]);
        MG.reculer(commande_PWM_G[1]);
      }
      else if (commande_PWM_D<=0 && commande_PWM_G>0)
      {
        MD.reculer(commande_PWM_D[1]);
        MG.avancer(commande_PWM_G[1]);
      }
      else
      {
        MD.avancer(commande_PWM_D[1]);
        MG.reculer(commande_PWM_G[1]);
      }

}
