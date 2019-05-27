#ifndef CONF_H
#define CONF_H

#include<Arduino.h>
#include <pins.h>

float precision_lin=90;
float precision_rot=30;
/*paramètres PID linéique */
float Kp_lin=10;
float Ki_lin=0;
float Kd_lin=0;

/*paramètres PID rotation */
float Kp_rot=120;
float Ki_rot=0;
float Kd_rot=10;

/* paramètre trapèze */
float Afrein=70;
float Vmax=400;
float Amax=80;


float Afrein_rot=20;
float Vmax_rot=400;
float Amax_rot=7;
float Dfrein_rot=(Vmax_rot*Vmax_rot)/(2*Afrein_rot);




float PWM_max = 190;
float PWM_min=140;
float periode = 5; //période de calcul asservissement en ms


float resolution =0.00792; // en ticks/cm
float entraxe = 12.3; // cm



//entraxe gros robot = 15.2
const float pi = 3.14159265359;

// config SHARP
//Résolution des Sharp en
double res = 4.8828;
// seuil de detection
int seuil = 100;


#endif
