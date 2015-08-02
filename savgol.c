#include <math.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#define NR_END 1
#define FREE_ARG char*
#define TINY 1.0e-20;


static float minarg1,minarg2;
#define FMIN(a,b) (minarg1=(a),minarg2=(b),(minarg1) < (minarg2) ?\
        (minarg1) : (minarg2))

float samples[150] = {
1.0000,
0.9442,
0.7831,
0.5346,
0.2265,
-0.1069,
-0.4283,
-0.7020,
-0.8974,
-0.9926,
-0.9772,
-0.8527,
-0.6331,
-0.3429,
-0.0144,
0.3157,
0.6105,
0.8373,
0.9706,
0.9957,
0.9097,
0.7222,
0.4542,
0.1354,
-0.1984,
-0.5101,
-0.7649,
-0.9343,
-0.9996,
-0.9533,
-0.8007,
-0.5588,
-0.2545,
0.0782,
0.4021,
0.6812,
0.8843,
0.9887,
0.9829,
0.8674,
0.6551,
0.3698,
0.0432,
-0.2882,
-0.5875,
-0.8212,
-0.9633,
-0.9980,
-0.9213,
-0.7418,
-0.4796,
-0.1639,
0.1701,
0.4851,
0.7460,
0.9237,
0.9983,
0.9616,
0.8176,
0.5824,
0.2822,
-0.0494,
-0.3756,
-0.6598,
-0.8705,
-0.9840,
-0.9878,
-0.8814,
-0.6766,
-0.3964,
-0.0719,
0.2605,
0.5639,
0.8044,
0.9552,
0.9994,
0.9321,
0.7608,
0.5047,
0.1923,
-0.1416,
-0.4597,
-0.7265,
-0.9123,
-0.9963,
-0.9691,
-0.8339,
-0.6056,
-0.3097,
0.0206,
0.3487,
0.6379,
0.8559,
0.9785,
0.9919,
0.8946,
0.6975,
0.4227,
0.1006,
-0.2326,
-0.5399,
-0.7870,
-0.9463,
-1.0000,
-0.9421,
-0.7792,
-0.5294,
-0.2204,
0.1131,
0.4339,
0.7064,
0.9001,
0.9934,
0.9758,
0.8494,
0.6283,
0.3370,
0.0082,
-0.3216,
-0.6155,
-0.8407,
-0.9721,
-0.9951,
-0.9071,
-0.7179,
-0.4486,
-0.1293,
0.2045,
0.5154,
0.7689,
0.9366,
0.9997,
0.9514,
0.7969,
0.5536,
0.2485,
-0.0844,
-0.4078,
-0.6857,
-0.8872,
-0.9896,
-0.9817,
-0.8643,
-0.6504,
-0.3640,
-0.0370,
0.2942,
0.5925,
0.8247,
0.9650
};

float *vector();
int *ivector();
float **matrix();

void free_vector();
void free_ivector();
void free_matrix();

void lubksb(float **a, int n, int *indx, float b[]);
void ludcmp(float **a, int n, int *indx, float *d);

float *vector(long nl, long nh)
/* allocate a float vector with subscript range v[nl..nh] */
{
    float *v;

    v=(float *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(float)));
    if (!v) printf("allocation failure in vector()");
    return v-nl+NR_END;
}

int *ivector(long nl, long nh)
/* allocate an int vector with subscript range v[nl..nh] */
{
    int *v;

    v=(int *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(int)));
    if (!v) printf("allocation failure in ivector()");
    return v-nl+NR_END;
}

float **matrix(long nrl, long nrh, long ncl, long nch)
/* allocate a float matrix with subscript range m[nrl..nrh][ncl..nch] */
{
    long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
    float **m;

    /* allocate pointers to rows */
    m=(float **) malloc((size_t)((nrow+NR_END)*sizeof(float*)));
    if (!m) printf("allocation failure 1 in matrix()");
    m += NR_END;
    m -= nrl;

    /* allocate rows and set pointers to them */
    m[nrl]=(float *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(float)));
    if (!m[nrl]) printf("allocation failure 2 in matrix()");
    m[nrl] += NR_END;
    m[nrl] -= ncl;

    for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

    /* return pointer to array of pointers to rows */
    return m;
}

void free_vector(float *v, long nl, long nh)
/* free a float vector allocated with vector() */
{
    free((FREE_ARG) (v+nl-NR_END));
}

void free_ivector(int *v, long nl, long nh)
/* free an int vector allocated with ivector() */
{
    free((FREE_ARG) (v+nl-NR_END));
}

void free_matrix(float **m, long nrl, long nrh, long ncl, long nch)
/* free a float matrix allocated by matrix() */
{
    free((FREE_ARG) (m[nrl]+ncl-NR_END));
    free((FREE_ARG) (m+nrl-NR_END));
}

void ludcmp(float **a, int n, int *indx, float *d)
{
        int i,imax,j,k;
        float big,dum,sum,temp;
        float *vv;

        vv=vector(1,n);
        *d=1.0;
        for (i=1;i<=n;i++) {
                big=0.0;
                for (j=1;j<=n;j++)
                        if ((temp=fabs(a[i][j])) > big) big=temp;
                if (big == 0.0) printf("Singular matrix in routine ludcmp");
                vv[i]=1.0/big;
        }
        for (j=1;j<=n;j++) {
                for (i=1;i<j;i++) {
                        sum=a[i][j];
                        for (k=1;k<i;k++) sum -= a[i][k]*a[k][j];
                        a[i][j]=sum;
                }
                big=0.0;
                for (i=j;i<=n;i++) {
                        sum=a[i][j];
                        for (k=1;k<j;k++)
                                sum -= a[i][k]*a[k][j];
                        a[i][j]=sum;
                        if ( (dum=vv[i]*fabs(sum)) >= big) {
                                big=dum;
                                imax=i;
                        }
                }
                if (j != imax) {
                        for (k=1;k<=n;k++) {
                                dum=a[imax][k];
                                a[imax][k]=a[j][k];
                                a[j][k]=dum;
                        }
                        *d = -(*d);
                        vv[imax]=vv[j];
                }
                indx[j]=imax;
                if (a[j][j] == 0.0) a[j][j]=TINY;
                if (j != n) {
                        dum=1.0/(a[j][j]);
                        for (i=j+1;i<=n;i++) a[i][j] *= dum;
                }
        }
        free_vector(vv,1,n);
}

void lubksb(float **a, int n, int *indx, float b[])
{
        int i,ii=0,ip,j;
        float sum;

        for (i=1;i<=n;i++) {
                ip=indx[i];
                sum=b[ip];
                b[ip]=b[i];
                if (ii)
                        for (j=ii;j<=i-1;j++) sum -= a[i][j]*b[j];
                else if (sum) ii=i;
                b[i]=sum;
        }
        for (i=n;i>=1;i--) {
                sum=b[i];
                for (j=i+1;j<=n;j++) sum -= a[i][j]*b[j];
                b[i]=sum/a[i][i];
        }
}

/**
double  c[np]           # O:  The kernel.
int np                  # I:  Size of the smoothing kernel.
int nl                  # I:  Points to the left of center.
int nr                  # I:  Points to the right of center.
int ld                  # I:  Order of derivative to return.
int m                   # I:  Order of the smoothing polynomial.

example: Savitzky-Golay with 40, 2th degree polynomial:

savgol(data, size(data), int 20, int 20, int 0, int 2)
**/

void savgol(float c[], int np, int nl, int nr, int ld, int m)
{
        int imj,ipj,j,k,kk,mm,*indx;
        float d,fac,sum,**a,*b;

        if (np < nl+nr+1 || nl < 0 || nr < 0 || ld > m || nl+nr < m)
                printf("bad args in savgol");
        indx=ivector(1,m+1);
        a=matrix(1,m+1,1,m+1);
        b=vector(1,m+1);
        for (ipj=0;ipj<=(m << 1);ipj++) {
                sum=(ipj ? 0.0 : 1.0);
                for (k=1;k<=nr;k++) sum += pow((double)k,(double)ipj);
                for (k=1;k<=nl;k++) sum += pow((double)-k,(double)ipj);
                mm=FMIN(ipj,2*m-ipj);
                for (imj = -mm;imj<=mm;imj+=2) a[1+(ipj+imj)/2][1+(ipj-imj)/2]=sum;
        }
        ludcmp(a,m+1,indx,&d);
        for (j=1;j<=m+1;j++) b[j]=0.0;
        b[ld+1]=1.0;
        lubksb(a,m+1,indx,b);
        for (kk=1;kk<=np;kk++) c[kk]=0.0;
        for (k = -nl;k<=nr;k++) {
                sum=b[1];
                fac=1.0;
                for (mm=1;mm<=m;mm++) sum += b[mm+1]*(fac *= k);
                kk=((np-k) % np)+1;
                c[kk]=sum;
        }

        int i;
        for (i = 1; i < np; i++) {
            printf("%f\n", c[i]);
        }

        free_vector(b,1,m+1);
        free_matrix(a,1,m+1,1,m+1);
        free_ivector(indx,1,m+1);
}

int main(int argc, char const *argv[])
{
    float in[11];
    savgol(in, 11, 5, 5, 0, 2);
    return 0;
}
