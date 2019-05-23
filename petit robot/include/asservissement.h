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


float compteur[2]={0,0};  // liste de la position gauche/droite en cm
float commande_mot_D[2]={100,0}; //commande du moteur droit
float commande_mot_G[2]={100,0}; //commande du moteur gauche
float commande_PWM_D[2]={0,0};  // commande du moteur droit en PWM
float commande_PWM_G[2]={0,0};



float vit_trap;
float vit_trapeze[2]={0,0};
float vit[2]={0,0};
float position[2];
float cons_lin[2];
float cons_lin_t;
float cons_rot[2];
float cons_rot_t;




float erreur_lin[2]={0,0}; // liste qui contient l'erreur linéaire à l'instant n et n-1
float erreur_rot[2]={0,0};  // liste qui contient l'erreur de rotation à l'instant n et n-1

float consigne_lin=5; // consigne de la distance rectiligne en cm
float consigne_rot=600; // cosigne d'angle en degré

float consigne_ticks = 0;  // ne pas mettre 0 car on divise par consigne_ticks

float commande_mot_Dt;
float commande_mot_Gt;

float norme_com = 6200;//(consigne_ticks+consigne_rot)*(Kp_lin*(1-1/3)+Kp_rot*(1-1/3));




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





void deplacement()

{
    float position_lin_t;
    float position_lin[2];

    float position_rot_t;
    float position_rot[2];

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
    new_erreur_lin = calcul_erreur(consigne_ticks, position_lin[0]); //consine finale
    maj_data(erreur_lin, new_erreur_lin);

    vit_consigne_lin_t=calcul_derive(erreur_lin);
    dist_frein_lin=distance_freinage(vit_consigne_lin_t);

    cons_lin_t=(trapeze_lin(erreur_lin, dist_frein_lin))/2;
    maj_data(cons_lin, cons_lin_t);



    position_rot_t=calcul_position_rot(compteur_ticks);
    maj_data(position_rot, position_rot_t);
    new_erreur_rot = calcul_erreur(consigne_rot, position_rot[0]); //consine finale
    maj_data(erreur_rot, new_erreur_rot);

    vit_consigne_rot_t=calcul_derive(erreur_rot);
    dist_frein_rot=distance_freinage(vit_consigne_rot_t);

    //cons_rot_t=(trapeze_rot(erreur_rot, dist_frein_rot));
    maj_data(cons_rot, cons_rot_t);


      Serial.print("consLin: ");
      Serial.print(consigne_ticks);
      Serial.print(" erreurLin: ");
      Serial.print(new_erreur_lin);
      Serial.print(" positionRot: ");
      Serial.print(position_rot[0]);
      Serial.print(" erreur_rot: ");
      Serial.print(new_erreur_rot);

      int_erreur_lin = calcul_integrale(cons_lin);
      der_erreur_lin = calcul_derive(cons_lin);

      int_erreur_rot = calcul_integrale(cons_rot);
      der_erreur_rot = calcul_derive(cons_rot);

      if(consigne_ticks==0)
      {
        cons_lin[0]= cons_lin[0]*PWM_max;
      }
      else
      {
        cons_lin[0]= cons_lin[0]*PWM_max/(consigne_ticks);
      }

      if(consigne_rot==0)
      {
        erreur_rot[0]=erreur_rot[0]*PWM_max;
      }
      else
      {
      erreur_rot[0]= erreur_rot[0]*PWM_max/(consigne_rot);
      }


      commande_lin = Kp_lin*cons_lin[0]/2 + Ki_lin*int_erreur_lin + Kd_lin*der_erreur_lin;
      commande_rot = Kp_rot*erreur_rot[0] + Ki_rot*int_erreur_rot + Kd_rot*der_erreur_rot;


      Serial.print(" comRot: ");
      Serial.print(commande_rot);

      commande_mot_Dt = commande_lin + commande_rot/2;
      commande_mot_Gt = commande_lin - commande_rot/2;


      maj_data(commande_mot_D, commande_mot_Dt);
      maj_data(commande_mot_G, commande_mot_Gt);

      // Serial.print("  cmdNNsat; ");
      // Serial.print(commande_mot_D[1]);

      // Serial.print("  cmdSat: ");
      // Serial.print(commande_mot_Dt);

      avD=positif(commande_mot_Dt);
      avG=positif(commande_mot_Gt);

      commande_PWM_Dt= commande_to_PWM(commande_mot_Dt);
      commande_PWM_Gt= commande_to_PWM(commande_mot_Gt);

      float commande_sat_D = constrain(commande_PWM_Dt, PWM_min, PWM_max);
      float commande_sat_G = constrain(commande_PWM_Gt, PWM_min, PWM_max);

      Serial.print(" cmdG: ");
      Serial.print(commande_mot_Dt);
      Serial.print(" cmdD: ");
      Serial.println(commande_mot_Gt);

      maj_data(commande_PWM_D, commande_PWM_Dt);
      maj_data(commande_PWM_G, commande_PWM_Gt);

      // if(new_erreur_lin<500 && new_erreur_lin > -500)
      // {
      //   commande_PWM_D[0]=60;
      //   commande_PWM_G[0]=60;
      //   Serial.println("coucou");
      // }



      if (abs(erreur_lin[0])>30 && abs(erreur_rot[0])>10)
      {
        Serial.println("GOOOOO");
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
      else if((abs(erreur_lin[0])<30 && abs(erreur_rot[0])<10) || commande_mot_Dt==0)
      {
        MD.arret();
        MG.arret();
        Serial.println("PROUT");
      }

      Serial.println(" ");
}
