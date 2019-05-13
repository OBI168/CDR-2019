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


float new_erreur_lin;  //nouvelle erreur linéaire
float new_erreur_rot; //nouvelle ereeur linéaire
float int_erreur_rot; //intégrale de l'erreur de rotation
float int_erreur_lin; //intégrale de l'erreur linéaire
float der_erreur_rot; //dérivé de l'erreur de rotation
float der_erreur_lin; //dérivé de l'erreur linéaire
float position_lin;   //position linéaire (en cm)
float position_rot;   //angle de rotation (en degré)



float compteur[2]={0,0};  // liste de la position gauche/droite en cm
float commande_mot_D[2]={100,0}; //commande du moteur droit
float commande_mot_G[2]={100,0}; //commande du moteur gauche
float commande_PWM_D[2]={0,0};  // commande du moteur droit en PWM
float commande_PWM_G[2]={0,0};  // commande du moteur gauche en PWM
float cm[0];

float erreur_lin[2]={0,0}; // liste qui contient l'erreur linéaire à l'instant n et n-1
float erreur_rot[2]={0,0};  // liste qui contient l'erreur de rotation à l'instant n et n-1

float consigne_lin=5; // consigne de la distance rectiligne en cm
float consigne_rot=0; // cosigne d'angle en degré

float consigne_ticks = 4000;

float commande_mot_Dt;
float commande_mot_Gt;
/*
convertit les ticks en cm
*/
void tick_to_cm(float ticks[2], float cm[2])
{
  for(int i=0; i>2; i++)
  {
    cm[i]=resolution*ticks[i];
  }
}

/*
calcul l'angle de rotation du robot en
*/
float calcul_position_rot(float liste[2])
{

  return (liste[1]-liste[0]);
  //180*(atan(liste[2]-liste[1])/entraxe)/pi;
}

/*
calcul la position du robot
*/
float calcul_position_lin(float liste[2])
{
  return (liste[1]+liste[0])/2;
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
  float acceleration = calcul_derive(liste);
  if (acceleration>acceleration_max)
  {
    return liste[0];
  }
  else
  {
    return liste[1];
  }
}

void deplacement()

{

    float commande_mot_Dt=0;
    float commande_mot_Gt=0;

    float commande_PWM_Dt=0;
    float commande_PWM_Gt=0;

    float commande_lin=0;
    float commande_rot=0;

      Serial.print("consLin: ");
      Serial.print(consigne_ticks);
      Serial.print(" posLin: "),
      Serial.print(position_lin);
      Serial.print(" errLin: ");
      Serial.print(new_erreur_lin);
      Serial.print(" consRot: ");
      Serial.print(consigne_rot);
      Serial.print("  posRot: "),
      Serial.print(position_rot);
      Serial.print("  errRot: ");
      Serial.print(new_erreur_rot);

      // float commande_lin;
      // float commande_rot;

      //tick_to_cm(compteur_ticks, cm);

      position_lin=calcul_position_lin(compteur_ticks);
      position_rot=calcul_position_rot(compteur_ticks);

      new_erreur_lin = calcul_erreur(consigne_ticks, position_lin);
      new_erreur_rot = calcul_erreur(consigne_rot, position_rot);


      maj_data(erreur_lin, new_erreur_lin);
      maj_data(erreur_rot, new_erreur_rot);

      int_erreur_lin = calcul_integrale(erreur_lin);
      der_erreur_lin = calcul_derive(erreur_lin);

      int_erreur_rot = calcul_integrale(erreur_rot);
      der_erreur_rot = calcul_derive(erreur_rot);

      commande_lin = Kp_lin*erreur_lin[1] + Ki_lin*int_erreur_lin + Kd_lin*der_erreur_lin;
      commande_rot = Kp_rot*erreur_rot[1] + Ki_rot*int_erreur_rot + Kd_rot*der_erreur_rot;

      Serial.print(" cmdRot: ");
      Serial.print(commande_rot);


      commande_mot_Dt = commande_lin + commande_rot/2;
      commande_mot_Gt = commande_lin - commande_rot/2;


      maj_data(commande_mot_D, commande_mot_Dt);
      maj_data(commande_mot_G, commande_mot_Gt);

      // Serial.print("  cmdNNsat; ");
      // Serial.print(commande_mot_D[1]);

      commande_mot_Dt = saturation_acc(commande_mot_D);
      commande_mot_Gt = saturation_acc(commande_mot_G);

      // Serial.print("  cmdSat: ");
      // Serial.print(commande_mot_Dt);

      commande_PWM_Dt = constrain(commande_to_PWM(commande_mot_Dt), -PWM_max, PWM_max);
      commande_PWM_Gt = constrain(commande_to_PWM(commande_mot_Gt), -PWM_max, PWM_max);


      Serial.print(" cmdG: ");
      Serial.print(commande_mot_Gt);
      Serial.print(" cmdD: ");
      Serial.println(commande_mot_Dt);



      maj_data(commande_PWM_D, commande_PWM_Dt);
      maj_data(commande_PWM_G, commande_PWM_Gt);

      if(commande_PWM_D[1]>0)
      {
        MD.avancer(abs(commande_PWM_D[1]));
      }
      else
      {
        MD.reculer(abs(commande_PWM_D[1]));
      }

      if (commande_PWM_G[1]>0)
      {
        MG.avancer(abs(commande_PWM_G[1]));
      }
      else
      {
        MG.reculer(abs(commande_PWM_G[1]));
      }

      Serial.println(" ");
}
