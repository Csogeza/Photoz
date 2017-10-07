#include<stdio.h>
#include<stdlib.h>

#define rows 18537
#define deltat 0.6


int main(int argc, char* argv[]){
    FILE *file=fopen(argv[1],"r");

    double* data; double* peak;
    data=(double*)malloc(rows*2*sizeof(double));
    peak=(double*)malloc(2*sizeof(double));
    int i,k;
    while(fscanf(file,"%lf",&data[i])==1){        //Matrixba valo beolvasas, soronkent reallocol
            i+=1;
    }
    double estmax;
    estmax=atof(argv[3]);
    //estmax=estmax-2400000.5;
    k=0;
    for(i=0;i<rows;i++){
            if(data[2*i]>estmax-deltat && data[2*i]<estmax+deltat){
                peak[2*k]=data[2*i];
                peak[2*k+1]=data[2*i+1];
                k+=1;
                peak=(double*)realloc(peak,2*(k+1)*sizeof(double));
            }
    }
    fclose(file);
    file=fopen(argv[2],"w");
    for(i=0;i<k;i++){
        fprintf(file,"%10f %10f\n",peak[2*i],peak[2*i+1]);
    }
return 0;
}
