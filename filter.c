#include <stdio.h>
#include <stdlib.h>

#define ORDER 5
#define NP 125
#define ARGU_NUM 4

int filter(int,float *,float *,int,float *,float *);

int filter(int ord, float *a, float *b, int np, float *x, float *y)
{
    int i, j;
    y[0] = b[0] * x[0];
    for (i = 1; i < ord; i++) {
        y[i] = 0.0;
        for (j = 0;j < i; j++)
            y[i] = y[i] + b[j] * x[i-j];
        for (j = 0;j < i-1; j++)
            y[i] = y[i] - a[j+1] * y[i-j-1];
    }
    // end of initial part
    for (i = ord; i < np+1; i++) {
        y[i] = 0.0;
        for (j = 0; j < ord; j++)
            y[i] = y[i] + b[j] * x[i-j];
        for (j = 0;j < ord-1; j++)
            y[i] = y[i] - a[j+1] * y[i-j-1];
    }

    return 0;
} /* end of filter */

int do_help()
{
    printf("usage: filter.exe [filter file] [input data] [output data]");
    return 0;
}


char const *fir_coef;
char const *inputfile_name;
char const *outputfile_name;

int main(int argc, char const *argv[])
{
    if (argc < ARGU_NUM) {
        do_help();
        exit(1);
    } else {
        fir_coef = argv[ARGU_NUM-3];
        inputfile_name = argv[ARGU_NUM-2];
        outputfile_name = argv[ARGU_NUM-1];
    }

    printf("%s, %s, %s\n", fir_coef, inputfile_name, outputfile_name);

    int i, j;
    FILE *fp;
    float x[NP], y[NP];
    float a[ORDER];
    float b[ORDER];
    fp = fopen(fir_coef, "r");
    if (fp == NULL) {
        printf("\n coefficient file not found! \n");
        exit(-1);
    }

    for (i = 0; i < ORDER; i++) {
        if(i)
            a[i] = 0;
        else
            a[i] = 1.0;
    }

    for (i = 0; i < ORDER; i++) {
        fscanf(fp, "%f", &b[i]);
        printf("index: %d, a: %f, b: %f\n", i, a[i], b[i]);
    }
    fclose(fp);

    /* printf("hello world \n"); */

    if((fp = fopen(inputfile_name, "r")) != NULL) {
        for(i = 0; i < NP; i++)
            fscanf(fp, "%f", &x[i]);
            /* printf("%f\n",x[i]); */
    } else {
        printf("\n file not found! \n");
        exit(-1);
    }
    fclose(fp);

    filter(ORDER,a,b,NP,x,y);

    if((fp=fopen(outputfile_name,"w+"))!=NULL) {
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
