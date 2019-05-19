#include <Arduino.h>
#include <odometrie.h>
#include <moteur.h>
#include <math.h>

Moteur MD(MoteurDroit_1,MoteurDroit_2,PWMMoteurDroit);
Moteur MG(MoteurGauche_1,MoteurGauche_2,PWMMoteurGauche);




float new_erreur_lin;  //nouvelle erreur linéaire
float new_erreur_rot; //nouvelle ereeur linéaire
float int_erreur_rot; //intégrale de l'erreur de rotation
float int_erreur_lin; //intégrale de l'erreur linéaire
float der_erreur_rot; //dérivé de l'erreur de rotation
float der_erreur_lin; //dérivé de l'erreur linéaire



float compteur[2]={0,0};  // liste de la position gauche/droite en cm
float commande_mot_D[2]={100,0}; //commande du moteur droit
float commande_mot_G[2]={100,0}; //commande du moteur gauche
float commande_PWM_D[2]={0,0};  // commande du moteur droit en PWM
float commande_PWM_G[2]={0,0};  // commande du moteur gauche en PWM

float vit[2];
float position[2];


float erreur_lin[2]={0,0}; // liste qui contient l'erreur linéaire à l'instant n et n-1
float erreur_rot[2]={0,0};  // liste qui contient l'erreur de rotation à l'instant n et n-1

float consigne_lin=5; // consigne de la distance rectiligne en cm
float consigne_rot=0; // cosigne d'angle en degré

float consigne_ticks = 4000;

float commande_mot_Dt;
float commande_mot_Gt;

float norme_com = 4000;//(consigne_ticks+consigne_rot)*(Kp_lin*(1-1/3)+Kp_rot*(1-1/3));

float position_lin_t;   //position linéaire (en cm)
float position_rot_t;   //angle de rotation (en degré)
float vit_lin_t;
float vit_rot_t;
float vit_lin[2];
float vit_rot[2];
float position_lin[2];
float position_rot[2];



float calcul_position_lin(float liste[2])
{
  return (liste[1]+liste[0])/2;
}
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
retourne True si le nombre est positive ou nul
*/
boolean positif(float x)
{
  if(x>=0)
    return true;
  else
    return false;
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


/*
mets à jour une liste avec une nouvelle valeur
@liste : liste à modifier
@nouveau : nouvelle valeur à mettre dans la liste
*/
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

float calcul_erreur(float consigne, float position)
{
  return consigne-position;
}



float commande_to_PWM(float commande)
{
  return commande/norme_com*PWM_max;
}


float trapeze(float liste[2])
{
  float vit=calcul_integrale(liste);
  if(liste[0]<Dfrein);
  {
    return vit-Afrein;
  }
  if (vit<Vmax && liste[0]>Dfrein)
  {
    return vit+Amax*Vmax;
  }
  if (vit>Vmax && liste[0] >Dfrein)
  {
    return vit;
  }
}

vit_t =trapeze(position);
maj_data(vit, vit_t);

float saturation(float liste[2], float saturation)
{
  float derive = calcul_derive(liste);
  if (derive>saturation)
  {
    return liste[1];
  }
  else
  {
    return liste[0];
  }
}

position_lin_t = calcul_position_lin(compteur_ticks);
position_rot_t = calcul_position_rot(compteur_ticks);

maj_data(position_lin, position_lin_t);
maj_data(position_rot, position_rot_t);

vit_lin_t=calcul_integrale(position_lin);
vit_rot_t=calcul_integrale(position_rot);

maj_data(vit_lin, vit_lin_t);
maj_data(vit_rot, vit_rot_t);
void deplacement()

{

    float commande_mot_Dt=0;
    float commande_mot_Gt=0;

    float commande_PWM_Dt=0;
    float commande_PWM_Gt=0;

    float commande_lin=0;
    float commande_rot_t=0;
    float commande_rot[2];



    boolean avD, avG;  // true si la roue doit avancer

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



      new_erreur_lin = calcul_erreur(consigne_ticks, position_lin);
      new_erreur_rot = calcul_erreur(consigne_rot, position_rot);


      maj_data(erreur_lin, new_erreur_lin);
      maj_data(erreur_rot, new_erreur_rot);

      int_erreur_lin = calcul_integrale(erreur_lin);
      der_erreur_lin = calcul_derive(erreur_lin);

      int_erreur_rot = calcul_integrale(erreur_rot);
      der_erreur_rot = calcul_derive(erreur_rot);

      commande_lin = Kp_lin*erreur_lin[0] + Ki_lin*int_erreur_lin + Kd_lin*der_erreur_lin;
      commande_rot_t = Kp_rot*erreur_rot[0] + Ki_rot*int_erreur_rot + Kd_rot*der_erreur_rot;


      maj_data(commande_rot,commande_rot_t);

      commande_rot_t=saturation(commande_rot, vit_rot_max);


      Serial.print(" cmdRot: ");
      Serial.print(commande_rot_t);


      commande_mot_Dt = commande_lin + commande_rot_t/2;
      commande_mot_Gt = commande_lin - commande_rot_t/2;


      maj_data(commande_mot_D, commande_mot_Dt);
      maj_data(commande_mot_G, commande_mot_Gt);

      // Serial.print("  cmdNNsat; ");
      // Serial.print(commande_mot_D[1]);

      commande_mot_Dt = saturation(commande_mot_D, acceleration_max);
      commande_mot_Gt = saturation(commande_mot_G, acceleration_max);

      // Serial.print("  cmdSat: ");
      // Serial.print(commande_mot_Dt);

      avD=positif(commande_mot_Dt);
      avG=positif(commande_mot_Gt);

      commande_PWM_Dt = constrain(commande_to_PWM(commande_mot_Dt), PWM_min, PWM_max);
      commande_PWM_Gt = constrain(commande_to_PWM(commande_mot_Gt), PWM_min, PWM_max);


      Serial.print(" cmdG: ");
      Serial.print(commande_PWM_Dt);
      Serial.print(" cmdD: ");
      Serial.println(commande_PWM_Gt);

      maj_data(commande_PWM_D, commande_PWM_Dt);
      maj_data(commande_PWM_G, commande_PWM_Gt);

      if(new_erreur_lin<500 && new_erreur_lin > -500)
      {
        commande_PWM_D[0]=60;
        commande_PWM_G[0]=60;
        Serial.println("coucou");
      }

      if(avD)
      {
        MD.avancer(abs(commande_PWM_D[0]));
      }
      else
      {
        MD.reculer(abs(commande_PWM_D[0]));
      }

      if (avG)
      {
        MG.avancer(abs(commande_PWM_G[0]));
      }
      else
      {
        MG.reculer(abs(commande_PWM_G[0]));
      }

      Serial.println(" ");
}
