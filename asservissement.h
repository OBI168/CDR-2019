#include <Arduino.h>
#include <codeurs.h>
#include <moteur.h>


Moteur MD(MoteurDroit_1,MoteurDroit_2,PWMMoteurDroit);
Moteur MG(MoteurGauche_1,MoteurGauche_2,PWMMoteurGauche);

/*
l'indice [n] d'une liste de taille n indque la valeur échantilloné à l'instant t
l'indice n-1 indique l'échantillon à l'instant n-1
*/
float commande_lin;
float commande_rot;
float new_erreur_lin;
float new_erreur_rot;
float int_erreur_rot;
float int_erreur_lin;
float der_erreur_rot;
float der_erreur_lin;
float position_lin;
float position_rot;
float somme_erreur_lin;
float somme_erreur_rot;

float compteur[2]={0,0};
float distance_D[2] ={0,0};  // nombre de clicks compteur droit
float distance_G[2]={0,0};   // nombre de clicks compteur gauche
float commande_D[2]={0,0};
float commande_G[2]={0,0};
float erreur_lin[2]={0,0};
float erreur_rot[2]={0,0};

float consigne_lin=0;
float consigne_rot=0;


float commande_mot_D;
float commande_mot_G;
float commande_PWM_D;
float commande_PWM_G;


/*
convertit les ticks en cm
*/
float tick_to_meter(float liste[2])
{
  for(int i=0; i>2; i++)
  {
    compteur[i]=resolution*liste[i];
  }
  return liste[2];
}

/*
calcul l'angle de rotation du robot
*/
float calcul_position_rot(float compteur[2])
{
  return compteur[2]-compteur[1];
}

/*
calcul la position du robot
*/
float calcul_position_lin(float compteur[2])
{
  return (compteur[2]-compteur[1])/2;
}

/*
mets à jour uen liste avec une nouvelle valeur
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

void name(/* arguments */) {
  /* code */
} asser(float consigne_lin, float consigne_rot)
{

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

  commande_mot_D = commande_lin + commande_rot;
  commande_mot_G = commande_lin + commande_rot;

  commande_PWM_D = commande_to_PWM(commande_mot_D);
  commande_PWM_G = commande_to_PWM(commande_mot_G);

  if(commande_PWM_D>0 && commande_PWM_G>0)
  {
    MD.avancer(commande_PWM_D);
    MG.avancer(commande_PWM_G);
  }
  else if(commande_PWM_D<=0 && commande_PWM_G<=0)
  {
    MD.reculer(commande_PWM_D);
    MG.reculer(commande_PWM_G);
  }
  else if (commande_PWM_D<=0 && commande_PWM_G>0)
  {
    MD.reculer(commande_PWM_D);
    MG.avancer(commande_PWM_G);
  }
  else
  {
    MD.avancer(commande_PWM_D);
    MG.reculer(commande_PWM_G);
  }

}
