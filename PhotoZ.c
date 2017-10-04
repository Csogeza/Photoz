#include<stdio.h>
#include<stdlib.h>
#include<math.h>

double interpol(double x1, double x2, double y1, double y2, double xk){

    double yk;

    // egyenes egyenlete a*x+b, van ket meresi pontom, x1,y1,x2,y2          y1=a*x1+b
    //                                                                      y2=a*x2+b           ------>             y1-y2=a*(x1-x2)         a=(y2-y1)/(x2-x1)
    //                                                                                                              b=y1-x1*(y2-y1)/(x2-x1)
    double a,b;
    a=(y2-y1)/(x2-x1);
    b=y1-x1*(y2-y1)/(x2-x1);

    yk=a*xk+b;

    return yk;
}

int main(int argc, char* argv[]){

FILE *file=fopen(argv[1],"r");
double* spektrum;
spektrum=(double*)malloc(2*sizeof(double));
int i,k,m;
k=1;
i=0;
while(fscanf(file,"%lf",&spektrum[i])==1){          // i lesz az elemek szama a spektrumban
    i+=1;                                           // k lesz az index a reallockolashoz, ennyi sora van a spektrumnak
    if(i%2==0){
        k+=1;
        spektrum=(double*)realloc(spektrum,2*k*sizeof(double));
    }
}
fclose(file);

//for(m=0;m<k;m++){
//    printf("%lf %lf\n",spektrum[2*m],spektrum[2*m+1]);
//}

double* szuro;
szuro=(double*)malloc(2*sizeof(double));
file=fopen(argv[2],"r");

int j,h;
j=0;
h=1;
while(fscanf(file,"%lf",&szuro[j])==1){          // j lesz az elemek szama a szuroben
    j+=1;                                        // h lesz az index a reallockolashoz FONTOS----> h eggyel tobb mint ami a ciklusirashoz kell, van egy ures sor a tomb vegen
    if(j%2==0){
        h+=1;
        szuro=(double*)realloc(szuro,2*h*sizeof(double));
    }
}
fclose(file);

printf("%d %d %d %d\n",i,k,j,h);

int q;
double z;               //ez lesz a redshift
z=0;                    // erre majd kell egy ciklus 0 es 0.70 kozott 0.01 es lepessel
double* spekz;

double xk,yk;
int w,v;
double* umspek;     //ujra-mintavetelezett-spektrum

int u;
u=0;
double flux,mab;
double szamlalo,nevezo;
double* magnik;
magnik=(double*)malloc(71*2*sizeof(double));


while(z<0.01){
    umspek=(double*)malloc(2*sizeof(double));
    spekz=(double*)malloc(2*k*sizeof(double));

    for(q=0;q<k-1;q++){
        spekz[2*q]=(1.0+z)*spektrum[2*q];
        spekz[2*q+1]=spektrum[2*q+1];
//    printf("%10f %10f\n",spekz[2*q],spekz[2*q+1]);
    }

//ujra kell mintavetelezni a spektrumot, veszek egy hullamhosszt a szurobol, majd megnezen melyik ket spektrum meresi pont koze esik, majd interpolalok es nezem a koztes pont erteket


    v=0;
    for(q=0;q<h-1;q++){                 //ujramintavetelezett spektrum: sorban veszi a hullámhosszertekeket a szuro adatsorabol
        xk=szuro[2*q];
        for(w=0;w<k-2;w++){
            if(xk>spekz[2*w] && xk<spekz[2*(w+1)]){         // ha a spektrum adatsoranak ket hullamhosszerteke koze esik, akkor a spektrumbol interpolal oda erteket
                if(v>0){
                    umspek=(double*)realloc(umspek,2*(v+1)*sizeof(double));
                }
                yk=interpol(spekz[2*w],spekz[2*(w+1)],spekz[2*w+1],spekz[2*(w+1)+1],xk);
                umspek[2*v]=xk;
                umspek[2*v+1]=yk;
                v+=1;
                break;
                }
            if(xk==spekz[2*w]){                             // ha pont meresi pontra, akkor az ahhoz tartozo erteket veszi
                if(v>0){
                    umspek=(double*)realloc(umspek,2*(v+1)*sizeof(double));
                }
                umspek[2*v]=xk;
                umspek[2*v+1]=spekz[2*w+1];
                v+=1;
                break;
            }
            if(xk==spekz[2*(w+1)]){                         // ez is, csak az utolso elemre a fentebbi nem fut le, ezért kell ez
                if(v>0){
                    umspek=(double*)realloc(umspek,2*(v+1)*sizeof(double));
                }
                umspek[2*v]=xk;
                umspek[2*v+1]=spekz[2*(w+1)+1];
                v+=1;
                break;
            }
        }
    }

for(q=0;q<v;q++){
    printf("%10f %10f\n",umspek[2*q],umspek[2*q+1]);
}
printf("%d %d\n \n ",h,v);
    szamlalo=0;
    nevezo=0;

    for(q=0;q<v;q++){                                       // keplet szerint szummazas az egyes elemekre, majd leosztasuk
        szamlalo+=umspek[2*q+1]*(pow(umspek[2*q]*pow(10,-10),2)*3*pow(10,8))*szuro[2*q+1]*umspek[2*q]*pow(10,-10);
        nevezo+=szuro[2*q+1]/(umspek[2*q]*pow(10,-10));
    }

    flux=szamlalo/(3*pow(10,8)*nevezo);
    mab=-2.5*log10(flux)-48.60;                         // valamiert nagyon negativ ertek jon ki...

    magnik[2*u]=z;
    magnik[2*u+1]=mab;

    u+=1;
    z+=0.01;
    free(umspek);
    free(spekz);
}

for(q=0;q<u;q++){
    printf("%10f %10f\n",magnik[2*q],magnik[2*q+1]);
}


return 0;
}
