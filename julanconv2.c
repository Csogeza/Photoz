#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define refyear 1858.0
#define refday 321.0
#define modjul 2400000.5


double absd(double szam){
    double absvalue;
     if(szam<0){
         absvalue=-szam;
     }
     else{
         absvalue=szam;
     }
    return absvalue;
 }


int main(int argc, char* argv[]){

// Midnight 1858 Nov 17. is 2 400 000,5
//31+28+31+30+31+30+31+31+30+31+30+31
//365+365+365+366
//Gergely naptar: minden 1900 nem szokoev viszont 2000 az, ez a 100 eves korrekcio skipelese

FILE *file=fopen(argv[1],"r");
char buffer[40];

double* julmag;
julmag=(double*)malloc(2*sizeof(double));

int year,month;
float day,hour,minute,sec;
double magnitude;
double juliandate;
int diffyear,delta,yearnum,ind;
int ycorr;
int korr1900;
int i,q;
q=0;

while(fgets(buffer,40,file)!=NULL){

  if(buffer[0]!='\n'){
    if(q>0){
        julmag=(double*)realloc(julmag,2*(q+1)*sizeof(double));
    }

    sscanf(buffer,"%d %d %f %f %f %f  %lf",&year,&month,&day,&hour,&minute,&sec,&magnitude);

//    printf("%d %d %lf %lf %lf %lf , %lf\n",year,month,day,hour,minute,sec,magnitude);


//year=2017;
//month=3;
//day=31.0;


    diffyear=year-2000;
    delta=absd(diffyear);

    if(delta%4==0 && year!=1900){
        switch(month){
            case 1 : break;
            case 2 : day=day+31.0; break;
            case 3 : day=day+60.0; break;
            case 4 : day=day+91.0; break;
            case 5 : day=day+121.0; break;
            case 6 : day=day+152.0; break;
            case 7 : day=day+182.0; break;
            case 8 : day=day+213.0; break;
            case 9 : day=day+244.0; break;
            case 10 : day=day+274.0; break;
            case 11 : day=day+305.0; break;
            case 12 : day=day+335.0; break;
            default : printf("WTF"); break;
        }
    yearnum=year;
    ind=0;
    }

    else{
        switch(month){
            case 1 : break;
            case 2 : day=day+31.0; break;
            case 3 : day=day+59.0; break;
            case 4 : day=day+90.0; break;
            case 5 : day=day+120.0; break;
            case 6 : day=day+151.0; break;
            case 7 : day=day+181.0; break;
            case 8 : day=day+212.0; break;
            case 9 : day=day+243.0; break;
            case 10 : day=day+273.0; break;
            case 11 : day=day+304.0; break;
            case 12 : day=day+334.0; break;
            default : printf("WTF"); break;
        }
    if((year-1)%4==0){
        yearnum=year-1;
        ind=1;
    }
    if((year-2)%4==0){
        yearnum=year-2;
        ind=2;
    }
    if((year-3)%4==0){
        yearnum=year-3;
        ind=3;
    }

    }


    ycorr=0;

    if(ind==1){
            ycorr=366;
    }
    if(ind==2){
        ycorr=731;
    }
    if(ind==3){
        ycorr=1096;
    }


    korr1900=0;

    if(year>1900){
        korr1900=1;
    }

//hour=20.0;
//minute=32.0;
//sec=54.0;

    day=day+hour/24.0+minute/1440.0+sec/86400.0;


    juliandate=(365.0-refday)+365.0+(double)(((yearnum-refyear-2)/4)*1461)+(double)(ycorr)+day+modjul-korr1900;

//    printf("%10f\n\n",juliandate);

    julmag[2*q]=juliandate;
    julmag[2*q+1]=magnitude;
    q+=1;

  }
}
fclose(file);
file=fopen(argv[2],"w");

for(i=0;i<q;i++){
    fprintf(file,"%lf %lf\n",julmag[2*i],julmag[2*i+1]);
}

return 0;
}
