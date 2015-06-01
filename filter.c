#include <stdio.h>
#include <stdlib.h>

#define ORDER 49
#define NP 125

int filter(int,float *,float *,int,float *,float *);

int filter(int ord, float *a, float *b, int np, float *x, float *y)
{
    int i, j;
    y[0] = b[0] * x[0];
    for (i = 1; i < ord+1; i++) {
        y[i] = 0.0;
        for (j = 0;j < i+1; j++)
            y[i] = y[i] + b[j] * x[i-j];
        for (j = 0;j < i; j++)
            y[i] = y[i] - a[j+1] * y[i-j-1];
    }
    // end of initial part
    for (i = ord+1; i < np+1; i++) {
        y[i] = 0.0;
        for (j = 0; j < ord+1; j++)
            y[i] = y[i] + b[j] * x[i-j];
        for (j = 0;j < ord; j++)
            y[i] = y[i] - a[j+1] * y[i-j-1];
    }

    return 0;
} /* end of filter */

int main(int argc, char const *argv[])
{
    int i, j;
    FILE *fp;
    float x[NP], y[NP];
    float a[ORDER+1], b[ORDER+1];
    fp = fopen("fir.log", "r");
    if (fp == NULL) {
        printf("\n coefficient file not found! \n");
        exit(-1);
    }
    for (i = 0; i < ORDER+1; i++) {
        // a[i] = (i == 0) ? 1 : 0;
        a[i] = 1.0;
    }
    for (i = 0; i < ORDER+1; i++) {
        fscanf(fp, "%f", &b[i]);
        // printf("index: %d, a: %f, b: %f\n", i, a[i], b[i]);
    }
    fclose(fp);

    /* printf("hello world \n"); */

    if((fp = fopen("acc1.dat", "r")) != NULL) {
        for(i = 0; i < NP; i++) {
            fscanf(fp, "%f", &x[i]);
            /* printf("%f\n",x[i]); */
        }
    } else {
        printf("\n file not found! \n");
        exit(-1);
    }
    fclose(fp);

    filter(ORDER,a,b,NP,x,y);

    /* NOW y=filter(b,a,x);*/
#if 0
    /* reverse the series for FILTFILT */
    for (i=0;i<NP;i++)
        x[i]=y[NP-i-1];
    /* do FILTER again */
    filter(ORDER,a,b,NP,x,y);
    /* reverse the series back */
    for (i=0;i<NP;i++)
        x[i]=y[NP-i-1];
    for (i=0;i<NP;i++)
        y[i]=x[i];
    /* NOW y=filtfilt(b,a,x); boundary handling not included*/
#endif

    if((fp=fopen("acc10.dat","w+"))!=NULL) {
        for (i=0;i<NP;i++) {
            fprintf(fp,"%f\n",y[i]);
        }
    } else {
        printf("\n file cannot be created! \n");
        exit(-1);
    }


    fclose(fp);
    return 0;
}
