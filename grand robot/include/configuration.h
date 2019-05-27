#ifndef CONF_H
#define CONF_H

#include<Arduino.h>
#include <pins.h>

float precision_lin=50;
float precision_rot=10;
/*paramètres PID linéique */
float Kp_lin=1;
float Ki_lin=0;
float Kd_lin=0;

/*paramètres PID rotation */
float Kp_rot=35;
float Ki_rot=0;
float Kd_rot=13;

/* paramètre trapèze */
float Afrein=80;
float Vmax=400;
float Amax=80;


float Afrein_rot=30;
float Vmax_rot=1000;
float Amax_rot=7;
float Dfrein_rot=(Vmax_rot*Vmax_rot)/(2*Afrein_rot);




float PWM_max = 110;
float PWM_min=90;
float periode = 5; //période de calcul asservissement en ms


float resolution =0.00792; // en ticks/cm
float entraxe = 12.3; // cm



//entraxe gros robot = 15.2
const float pi = 3.14159265359;

#endif
