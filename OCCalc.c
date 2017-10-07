#include<stdio.h>
#include<math.h>
#include<stdlib.h>

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
    double refepoch,period,freq,date,mindate,maxdate;
    int mpoints;
    double* input;
    double* output;

    FILE *file=fopen(argv[1],"r");
    refepoch=atof(argv[2]);
    freq=atof(argv[3]);
    period=1.0/freq;
    mindate=atof(argv[4]);
    maxdate=atof(argv[5]);
    mpoints=atoi(argv[6]);

    input=(double*)malloc(2*mpoints*sizeof(double));
    output=(double*)malloc(2*mpoints*sizeof(double));

    int i;
    for(i=0;i<2*mpoints;i++){
        fscanf(file,"%lf",&input[i]);
    }
    fclose(file);

    date=refepoch;
    while(date>mindate){
        date=date-period;
    }

    printf("%lf\n",date);

    i=0;
    while(date<maxdate){
        if(absd(input[2*i]-date)<1.0){
            output[2*i]=input[2*i];
            output[2*i+1]=input[2*i]-date;
            i+=1;
        }
        date=date+period;
    }

    file=fopen(argv[7],"w");

    for(i=0;i<mpoints;i++){
        fprintf(file,"%10f %10f\n",output[2*i],output[2*i+1]);
    }

return 0;
}
