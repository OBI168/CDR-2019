#include<Arduino.h>

/*paramètres PID linéique */
float Kp_lin;
float Ki_lin;
float Kd_lin;

/*paramètres PID rotation */
float Kp_rot;
float Ki_rot;
float Kd_rot;

float resolution = 0.00109;  // en ticks/cm
float vitesse_max = 1;
float PWM_max = 255;
