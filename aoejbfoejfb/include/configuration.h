

#include<Arduino.h>


/*paramètres PID linéique */
float Kp_lin=300;
float Ki_lin=100;
float Kd_lin=10;

/*paramètres PID rotation */
float Kp_rot=70;
float Ki_rot=0;
float Kd_rot=0;

/* paramètre trapèze */
float Afrein;
float Vmax;
float Amax;
float Dfrein=(Vmax*Vmax)/(2*Afrein);

float vit_max ;     // en cm/s
float acceleration_max =10;  //en cm/s^2
float vit_rot_max=500;
float PWM_max = 90;
float PWM_min=60;
float periode = 5; //période de calcul asservissement en ms


float resolution = 0.00109; // en ticks/cm
float entraxe = 12.3; // cm



//entraxe gros robot = 15.2
int const pi = 3.14159265359;
