#include <Arduino.h>

#include <codeurs.h>
#include <moteur.h>
#include <pins.h>
#include <variables.h>

///timer
//#include <TimerThree.h>

Moteur MD(MoteurDroit_1,MoteurDroit_2,PWMMoteurDroit);
Moteur MG(MoteurGauche_1,MoteurGauche_2,PWMMoteurGauche);


void calcul_vitesse()
{
  vit[0]= dmes[0]-dmes_moins_1[0];
  vit[1]= dmes[1]-dmes_moins_1[1];
}


void av(int avanceourecule, float consigne)
{
  //construction consigne
  if (consigne>200)
    dist_trap=600; //longeur du trapèze ?
  else pwm_max=pwm_min;

  while(ok[0]==0||ok[1]==0)
  {
    for(int i=0; i<2; i++)
    {
      pwm_old[i]=pwm[i];
      dmes_moins_1[0]=dmes[0];
      dmes_moins_1[1]=dmes[1];
      dmes[0]=CompteurDroit;
      dmes[1]=CompteurGauche;
      eps_moins_1[i]=eps[i];
      eps[i]=consigne-dmes[i];
      somme_eps[i]+=eps[i];

      calcul_vitesse();

        if (abs(eps[i])>200&&ok[i]==0)
        //commande[i]= Kp*dmes[i]+ Ki*somme_eps[i]+Kd*vit[i];


      if(constrain((consigne-dmes[i])*pwm_max/dist_trap,pwm_min,constrain(dmes[i]*pwm_max/dist_trap,pwm_min,pwm_max)) !=200)
        {
          Vcomm[i]=abs(eps[i]);
          pwm[i]=kppwm*(Vcomm[i]+kpv*(vit[i]-Vcomm[i]));
        }
        if (eps[i]>0.02)
        commande[i]=pwm[i]+kpav*((dmes[!i]-dmes[i])+(dmes_moins_1[!i]-dmes_moins_1[i])*((pas/Tiav)-1)); //correction PI

    }
  }
}
void setup()
{}

void loop()
{}
