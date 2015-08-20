#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265
#define L 256 /* Filter length */
#define Pr_L L
#define M 3.125

double * my_fir1(unsigned N, double Wn)
{
    /* FIR filter
    Return an array of 1 X 256
    */
    unsigned odd, i, j, nhlf, i1;
    double f1, gain, c1;
    double wind[Pr_L],xn[Pr_L/2],b[Pr_L/2],c[Pr_L/2],c3[Pr_L/2];
    double *bb;

    bb = (double *) malloc(sizeof(double) * Pr_L);

    gain = 0.0000000;
    N = N+1;
    odd = N - (N/2)*2; /* odd = rem(N,2) */

    /*wind = hamming(N);*/
    for (i=0; i < Pr_L; i++)
    {
        wind[i] = 0.54 - 0.46 * cos ((2 * PI * i) / (N-1));
    }

    f1 = Wn / 2.0;
    c1 = f1;
    nhlf = (N+1) / 2;
    i1 = odd + 1;

    /* Lowpass */

    if(odd)
        b[0] = 2 * c1;

    for (i=0; i < nhlf; i++)
        xn[i] = i + 0.5 * (1 - odd);

    for (i=0; i < nhlf; i++)
        c[i] = PI * xn[i];

    for (i=0; i < nhlf; i++)
        c3[i] = 2 * c1 * c[i];

    /* b(i1:nhlf)=(sin(c3)./c) */
    for (i=0; i < nhlf; i++)
        b[i] = sin(c3[i]) / c[i];

    /* bb = real([b(nhlf:-1:i1) b(1:nhlf)].*wind(:)') */
    for (i=0,j=nhlf-1; i < nhlf; i++, j--)
        bb[i] = b[j];

    for (i=nhlf,j=0; i < Pr_L; i++,j++)
        bb[i] = b[j];

    for (i=0; i < Pr_L; i++)
        bb[i] = bb[i] * wind[i];

    /* gain = abs(polyval(b,1)); */
    for (i=0; i < Pr_L; i++)
        gain += bb[i];
    /* b = b / gain */
    for (i=0; i < Pr_L; i++)
        bb[i] = bb[i] / gain;

    return bb;

}

int main (void)
{
    unsigned i;
    double *Prot_filt;
    Prot_filt = (double *) malloc(sizeof(double) * Pr_L);

    Prot_filt = my_fir1(Pr_L-1, 0.8); /* FIR filter */

/*
    for(i=0; i < Pr_L; i++)
        Prot_filt[i] = Prot_filt[i] * (M/2);*/

    printf("print coefficient: \n");
    for(i=0; i < Pr_L; i++)
        // printf("Prot_filt[%d]=%f\n",i,Prot_filt[i]); /*debug*/
        printf("%f\n", Prot_filt[i]);

}
