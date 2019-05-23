#ifndef CONF_H
#define CONF_H

#include<Arduino.h>
#include <pins.h>


/*paramètres PID linéique */
float Kp_lin=1;
float Ki_lin=0;
float Kd_lin=0;

/*paramètres PID rotation */
float Kp_rot=1;
float Ki_rot=0;
float Kd_rot=0;

/* paramètre trapèze */
float Afrein=30;
float Vmax=100;
float Amax=50;


float Afrein_rot=20;
float Vmax_rot=300;
float Amax_rot=250;
float Dfrein_rot=(Vmax_rot*Vmax_rot)/(2*Afrein_rot);



float acceleration_max =10;  //en cm/s^2
float vit_rot_max=500;
float PWM_max = 100;
float PWM_min=80;
float periode = 5; //période de calcul asservissement en ms


float resolution = 0.00109; // en ticks/cm
float entraxe = 12.3; // cm



//entraxe gros robot = 15.2
const float pi = 3.14159265359;

#endif
