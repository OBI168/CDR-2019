boolean ok[2]={0,0};
double ecart =0;  // ecart de la distance parcourue
int droite =0;
int gauche =1;

float pwm_max=250;
float pwm_min=70;
float dist_trap=600; // valeur au pif (nombre de ticks)

double kppwm=600;//gain distance-->pwm
int Kp=300,Ki=0,Kd=0;

double kpv=50; //correcteur PI en vitesse
double Tip=0.00005;

double kpav=50000;//gain proportionels et integ correcteur ligne droite
double Tiav=0.25;

float pas=1;//pas de calcul en millisecondes: c'est la periode d'echantillonage

double mot[2]={12,11}; //pin pwn moteur
double eps_moin_2[2]={0,0}; // erreur 2 pas en retard
double eps_moins_1[2]={0,0}; // erreur 1 pas en retard
double eps[2]={0,0}; // erreurs
double commande[2]={0,0};
double entree_moins_1[2]={0,0};
double entree[2] ={0,0};
double dmes[2]={0,0};
double dmes_moins_1[2]={0,0};
double pwm[2] = {100,100};
double pwm_old[2]={0,0};
double vit[2]={0,0};
double Vcomm[2]={0,0};
double somme_eps[2]={0,0};
