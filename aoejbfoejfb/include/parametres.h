#include<Arduino.h>

/*paramètres PID linéique */
float Kp_lin=1;
float Ki_lin=0;
float Kd_lin=0;

/*paramètres PID rotation */
float Kp_rot=1;
float Ki_rot=0;
float Kd_rot=0;

float vitesse_max = 80;     // en cm/s
float acceleration_max =10;  //en cm/s^2
float PWM_max = 100;
float periode = 5; //période de calcul asservissement en ms


float resolution = 0.00109; // en ticks/cm
float entraxe = 12.3; // cm



//entraxe gros robot = 15.2
int const pi = 3.14159265359;
