#ifndef CONF_H
#define CONF_H

#include<Arduino.h>
#include <pins.h>


/*paramètres PID linéique */
float Kp_lin=1;
float Ki_lin=0;
float Kd_lin=0;

/*paramètres PID rotation */
float Kp_rot=50;
float Ki_rot=0;
float Kd_rot=0;

/* paramètre trapèze */
float Afrein=100;
float Vmax=200;
float Amax=100;


float Afrein_rot=200;
float Vmax_rot=30;
float Amax_rot=25;
float Dfrein_rot=(Vmax_rot*Vmax_rot)/(2*Afrein_rot);



float acceleration_max =10;  //en cm/s^2
float vit_rot_max=500;
float PWM_max = 90;
float PWM_min=60;
float periode = 5; //période de calcul asservissement en ms


float resolution = 0.00109; // en ticks/cm
float entraxe = 12.3; // cm



//entraxe gros robot = 15.2
const float pi = 3.14159265359;

#endif
