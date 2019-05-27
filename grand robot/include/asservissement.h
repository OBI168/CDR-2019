#include <Arduino.h>
#include <odometrie.h>
#include <moteur.h>
#include <math.h>
#include <utility.h>
#include <sharp.h>

    /*définition des objets moteurs */
Moteur MD(MoteurDroit_1,MoteurDroit_2,PWMMoteurDroit);
Moteur MG(MoteurGauche_1,MoteurGauche_2,PWMMoteurGauche);


// liste qui contient l'erreur de rotation à l'instant n et n-1
float compteur[2]={0,0};  // liste de la position gauche/droite en cm

float cons_lin[2];
float cons_lin_t;
float cons_rot[2];
float cons_rot_t;

float position_lin_t;
float position_lin[2];
float position_rot_t;
float position_rot[2];

float erreur_lin[2];
float erreur_rot[2];

float cons_distance = 50;
float distance = cm_to_ticks(cons_distance);

float cons_ang = 0;
float angle = calcul_cons_rot(cons_ang);


float commande_mot_Dt;
float commande_mot_Gt;


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


float distance_freinage(float vit)
{
  return (vit*vit)/(2*Afrein);
}


float trapeze_lin(float liste[2], float Dfrein, float erreur)
{
  if(abs(liste[0])<Dfrein) //phase de freinage
  {
    if(erreur<0)
    {
      //Serial.println(" FREIN");
      return  -abs(cons_lin[1]-Afrein);
    }
    else
    {
      //Serial.println(" FREIN");
      return  abs(cons_lin[1]-Afrein);
    }

  }
  else if (cons_lin[1]<Vmax && liste[0]>Dfrein) //phase d'acceleration
  {
    if(erreur<0)
    {
    //Serial.println(" ACCELERATION");
    return abs(cons_lin[1]+Amax);
  }
  else
  {
  //Serial.println(" ACCELERATION");
  return abs(cons_lin[1]+Amax);
  }

  }
  else if (cons_lin[1]>=Vmax && liste[0]>Dfrein) //phase de plateau
  {
    if(erreur<0)
    {
    //Serial.println("CONSTANT");
    return -Vmax;
    }
    else
    //Serial.println("CONSTANT");
    return Vmax;
    }
  else if (abs(erreur)<precision_lin)
  {
    return 0;
  }
}

float trapeze_rot(float liste[2], float Dfrein, float erreur)
{

  if(abs(liste[0])<Dfrein) //phase de freinage
  {
    if(erreur<0)
    {
      Serial.println("FREIN");
      return -abs(cons_rot[1]-Afrein_rot);
    }
    else
    {
      Serial.println(" FREIN");
      return abs(cons_rot[1]-Afrein_rot);
    }

  }
  else if (cons_rot[1]<Vmax && liste[0]>Dfrein) //phase d'acceleration
  {
    if(erreur<0)
    {
      Serial.println(" ACCELERATION");
      return -abs(cons_rot[1]+Amax_rot);
    }
    else
    {
      Serial.println(" ACCELERATION");
      return abs(cons_rot[1]+Amax_rot);
    }
  }
  else if (cons_rot[1]>=Vmax && liste[0]>Dfrein) //phase de plateau
  {
    if(erreur<0)
    {
      Serial.println("CONSTANT");
      return -Vmax_rot;
    }
      else
      {
      Serial.println("CONSTANT");
      return Vmax_rot;
      }
    }
  else if (abs(erreur)<precision_rot)
  {
    //Serial.println("OOOKKKKK");
    return 0;
  }
}

void deplacement(boolean aD, float comPWM_D, boolean aG, float comPWM_G, float comRot, float erreurRot)
{
  {
  if ((abs(erreurRot)>precision_rot) || (abs(erreur_lin[0]>precision_lin)))
  {
      Serial.println("GOOOOOOOO");
      if(aD)
      {
        if (Detection_obstacle_AV())
        {
          MD.arret();
          MG.arret();
          Serial.println("Stop1");
        }
        else
        {
          MD.avancer(abs(comPWM_D));
        }
      }
      else
      {
        MD.reculer(abs(comPWM_D));
      }

      if (aG)
      {
        if (Detection_obstacle_AV())
        {
          MD.arret();
          MG.arret();
          Serial.println("Stop2");
        }
        else
        {
          MG.avancer(abs(comPWM_G));
        }
      }
      else
      {
        MG.reculer(abs(comPWM_G));
      }
    }
    else if((abs(erreurRot)<precision_rot) && (abs(erreur_lin[0]<precision_lin)))
  {
    Serial.println("STOP");
    MD.arret();
    MG.arret();
  }
}
}

void asservissement()
{
    // while(!start)
    // {

  /*défiition des variables pour les PID*/
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



  float commande_mot_Dt=0;
  float commande_mot_Gt=0;

      float commande_PWM_Dt=0;
      float commande_PWM_Gt=0;

      float commande_lin=0;
      float commande_rot=0;
      float vit_consigne_lin_t=0;
      float vit_consigne_rot_t=0;
      float dist_frein_lin;
      float dist_frein_rot;


      boolean avD, avG;  // true si la roue doit avancer


      position_lin_t=calcul_position_lin(compteur_ticks);
      maj_data(position_lin, position_lin_t);
      new_erreur_lin = calcul_erreur(distance, position_lin[0]); //consine finale
      maj_data(erreur_lin, new_erreur_lin);

      vit_consigne_lin_t=calcul_derive(erreur_lin);
      dist_frein_lin=distance_freinage(vit_consigne_lin_t);

      cons_lin_t =trapeze_lin(erreur_lin, dist_frein_lin, erreur_lin[0]);
      maj_data(cons_lin, cons_lin_t);


      // Serial.print(" consTrap: ");
      // Serial.print(cons_lin[0]);
      // Serial.print(" conLinTap1: ");
      // Serial.print(cons_lin[1]);

        position_rot_t=calcul_position_rot(compteur_ticks);
        maj_data(position_rot, position_rot_t);
        new_erreur_rot = calcul_erreur(angle, position_rot[0]); //consine finale
        maj_data(erreur_rot, new_erreur_rot);

        vit_consigne_rot_t=calcul_derive(erreur_rot);
        dist_frein_rot=distance_freinage(vit_consigne_rot_t);

        cons_rot_t=trapeze_rot(erreur_rot, dist_frein_rot, erreur_rot[0]);
        maj_data(cons_rot, cons_rot_t);

        // Serial.print("consLin: ");
        // Serial.print(distance);
        Serial.print(" erreurLin: ");
        Serial.print(erreur_lin[0]);
        Serial.print(" erreurRot: ");
        Serial.print(erreur_rot[0]);

        Serial.print(" consTROT: ");
        Serial.print(cons_rot[0]);

        int_erreur_lin = calcul_integrale(cons_lin);
        der_erreur_lin = calcul_derive(cons_lin);

        int_erreur_rot = calcul_integrale(cons_rot);
        der_erreur_rot = calcul_derive(cons_rot);

        commande_lin = Kp_lin*cons_lin[0]/2 + Ki_lin*int_erreur_lin + Kd_lin*der_erreur_lin;
        commande_rot = Kp_rot*cons_rot[0] + Ki_rot*int_erreur_rot + Kd_rot*der_erreur_rot;

        // Serial.print(" comLin: ");
        // Serial.print(commande_lin);
        Serial.print(" angle: ");
        Serial.print(angle);
        Serial.print(" comRot: ");
        Serial.print(commande_rot);

        commande_mot_Dt = commande_lin -  commande_rot/2;
        commande_mot_Gt = commande_lin + commande_rot/2;

        maj_data(commande_mot_D, commande_mot_Dt);
        maj_data(commande_mot_G, commande_mot_Gt);

        avD=positif(commande_mot_Dt);
        avG=positif(commande_mot_Gt);

        commande_PWM_Dt= map(commande_mot_D[0], 0, (Vmax*Kp_lin+Vmax_rot*Kp_rot), PWM_min, PWM_max);
        commande_PWM_Gt= map(commande_mot_G[0], 0, (Vmax*Kp_lin+Vmax_rot*Kp_rot), PWM_min, PWM_max);

        Serial.print(" cmdD: ");
        Serial.print(commande_PWM_Dt);
        Serial.print(" cmdG: ");
        Serial.println(commande_PWM_Gt);

        maj_data(commande_PWM_D, commande_PWM_Dt);
        maj_data(commande_PWM_G, commande_PWM_Gt);

        deplacement(avD,commande_PWM_D[0], avG, commande_PWM_G[0], commande_rot, erreur_rot[0]);
        Serial.println(" ");
      //}
}
