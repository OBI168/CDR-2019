#include <Arduino.h>
#include <odometrie.h>
#include <moteur.h>
#include <math.h>
#include <utility.h>

Moteur MD(MoteurDroit_1,MoteurDroit_2,PWMMoteurDroit);
Moteur MG(MoteurGauche_1,MoteurGauche_2,PWMMoteurGauche);




float new_erreur_lin;  //nouvelle erreur linéaire
float new_erreur_rot; //nouvelle ereeur linéaire
float int_erreur_rot; //intégrale de l'erreur de rotation
float int_erreur_lin; //intégrale de l'erreur linéaire
float der_erreur_rot; //dérivé de l'erreur de rotation
float der_erreur_lin; //dérivé de l'erreur linéaire

float commande_mot_D[2]={100,0}; //commande du moteur droit
float commande_mot_G[2]={100,0}; //commande du moteur gauche
float commande_PWM_D[2]={0,0};  // commande du moteur droit en PWM
float commande_PWM_G[2]={0,0};

float cons_lin[2];
float cons_lin_t;
float cons_rot[2];
float cons_rot_t;

float erreur_lin[2];


float angle=0; // cosigne d'angle en degré
float distance = 8000;  // ne pas mettre 0 car on divise par distance

float commande_mot_Dt;
float commande_mot_Gt;

float norme_com = 6200;//(distance+angle)*(Kp_lin*(1-1/3)+Kp_rot*(1-1/3));


float calcul_position_lin(float liste[2])
{
  return (liste[1]+liste[0])/2;
}

float calcul_position_rot(float liste[2])
{
  return liste[1]-liste[0];
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


float calcul_erreur(float consigne, float position)
{
  return consigne-position;
}



float commande_to_PWM(float commande)
{
  return commande/norme_com*PWM_max +60;
}

float distance_freinage(float vit)
{
  return (vit*vit)/(2*Afrein);
}


float trapeze_lin(float liste[2], float Dfrein)
{

  if(abs(liste[0])<Dfrein) //phase de freinage
  {
    Serial.println("FREIN");
    return cons_lin[1]-Afrein;

  }
  else if (cons_lin[1]<Vmax && liste[0]>Dfrein) //phase d'acceleration
  {
    Serial.println("ACCELERATION");
    return cons_lin[1]+Amax;

  }
  else if (cons_lin[1]>=Vmax && liste[0] >Dfrein) //phase de plateau
  {
    Serial.println("CONSTANT");
    return Vmax;
  }
  else
  {
    return 0;
  }
}

float trapeze_rot(float liste[2], float Dfrein)
{
  if(abs(liste[0])<Dfrein) //phase de freinage
  {
    return cons_rot[1]-Afrein_rot;
  }
  else if (cons_rot[1]<Vmax_rot && liste[0]>Dfrein) //phase d'acceleration
  {
    return cons_lin[1]+Amax_rot;
  }
  else if (cons_rot[1]>=Vmax_rot && liste[0]>Dfrein) //phase de plateau
  {
    return Vmax_rot;
  }

}


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



float calcul_commande_lin(float ticks[2], float consigne)
{
  float position_lin_t;
  float position_lin[2];



  float vit_consigne_lin_t;
  float vit_consige_lin[2];
  float dist_frein_lin;

  position_lin_t=calcul_position_lin(compteur_ticks);
  maj_data(position_lin, position_lin_t);
  new_erreur_lin = calcul_erreur(distance, position_lin[0]); //consine finale
  maj_data(erreur_lin, new_erreur_lin);
  Serial.print("erreur: ");
  Serial.print(erreur_lin[0]);
  Serial.print("  ");
  Serial.print(erreur_lin[1]);
  vit_consigne_lin_t=calcul_derive(erreur_lin);
  Serial.print(" der-erreur: ");
  Serial.print(vit_consigne_lin_t);
  dist_frein_lin=distance_freinage(vit_consigne_lin_t);

  return (trapeze_lin(erreur_lin, dist_frein_lin))/2;
}


float erreur_pos_rot(float liste[2], float consigne)
{
  float position_rot_t;
  float position_rot[2];

  position_rot_t=calcul_position_rot(compteur_ticks);
  maj_data(position_rot, position_rot_t);
  return calcul_erreur(angle, position_rot[0]);
}

float pid_lin(float consigne[2])
{
  float commande;
  float der_cons;
  float int_cons;

  der_cons= calcul_derive(consigne);
  int_cons=calcul_integrale(consigne);
  commande = Kp_lin*consigne[0]+ Ki_lin*int_cons + Kd_lin*der_cons;
  return commande;
}

float pid_rot(float consigne[2])
{
  float commande;
  float der_cons;
  float int_cons;

  der_cons= calcul_derive(consigne);
  int_cons=calcul_integrale(consigne);
  commande = Kp_lin*consigne[0]/2 + Ki_rot*int_cons + Kd_rot*der_cons;
  return commande;
}


void deplacement()
{

    float consigne_lin_t;
    float consigne_lin[2];

    float consigne_rot_t;
    float consigne_rot[2];

    float commande_lin;
    float commande_rot;

    float commande_mot_Gt=0;

    float commande_PWM_Dt=0;
    float commande_PWM_Gt=0;

    float erreur_rot;

    boolean avD, avG;


    erreur_rot=calcul_erreur(distance, calcul_position_rot(compteur_ticks));

    consigne_lin_t=calcul_commande_lin(compteur_ticks, distance);
    maj_data(consigne_lin, consigne_lin_t);

    consigne_rot_t=erreur_pos_rot(compteur_ticks, angle);
    maj_data(consigne_rot, consigne_rot_t);

    // true si la roue doit avancer


      // if(distance==0)
      // {
      //   consigne_lin[0]=consigne_lin[0]*PWM_max;
      // }
      // else
      // {
      //   consigne_lin[0]=consigne_lin[0]*PWM_max/(distance);
      // }
      //
      // if(angle==0)
      // {
      //   consigne_rot[0]=consigne_rot[0]*PWM_max;
      // }
      // else
      // {
      //   consigne_rot[0]= consigne_rot[0]*PWM_max/(angle);
      // }


      commande_lin=pid_lin(consigne_lin);
      commande_rot=pid_rot(consigne_rot);

      Serial.print("comLin: ");
      Serial.print(commande_lin);
      Serial.print("comRot: ");
      Serial.print(commande_lin);

      commande_mot_Dt = commande_lin + commande_rot/2;
      commande_mot_Gt = commande_lin - commande_rot/2;

      maj_data(commande_mot_D, commande_mot_Dt);
      maj_data(commande_mot_G, commande_mot_Gt);

      avD=positif(commande_mot_Dt);
      avG=positif(commande_mot_Gt);

      commande_PWM_Dt= commande_to_PWM(commande_mot_Dt);
      commande_PWM_Gt= commande_to_PWM(commande_mot_Gt);

      float commande_sat_D = constrain(commande_PWM_Dt, PWM_min, PWM_max);
      float commande_sat_G = constrain(commande_PWM_Gt, PWM_min, PWM_max);

      maj_data(commande_PWM_D, commande_PWM_Dt);
      maj_data(commande_PWM_G, commande_PWM_Gt);

      Serial.print(" comMTD: ");
      Serial.print(commande_PWM_Dt);
      Serial.print(" comMTG: ");
      Serial.print(commande_PWM_Gt);



      if (abs(erreur_lin)>30 && abs(erreur_rot)>10)
      {
        if(avD)
        {
          MD.avancer(abs(commande_sat_D));
        }
        else
        {
          MD.reculer(abs(commande_sat_D));
        }

        if (avG)
        {
          MG.avancer(abs(commande_sat_G));
        }
        else
        {
          MG.reculer(abs(commande_sat_G));
        }
      }
      else if((abs(erreur_lin)<30 && abs(erreur_rot)<10) || commande_mot_Dt==0)
      {
        MD.arret();
        MG.arret();
      }

      Serial.println(" ");
}
