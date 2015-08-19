#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#define NR_END 1
#define FREE_ARG char*
#define TINY 1.0e-20;

static float minarg1,minarg2;
#define FMIN(a,b) (minarg1=(a),minarg2=(b),(minarg1) < (minarg2) ?\
        (minarg1) : (minarg2))

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

void savgol(float *c, int np, int nl, int nr, int ld, int m)
{
        int i = 0;
        int imj,ipj,j,k,kk,mm,*indx;
        float d,fac,sum,**a,*b;

        if (np < nl+nr+1 || nl < 0 || nr < 0 || ld > m || nl+nr < m)
                printf("bad args in savgol\n");

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
                // c[kk]=sum;
                c[i] = sum;
                i++;
        }

#ifndef DEBUG_SAVGOL
        int i;
        for (i = 1; i < np; i++) {
            printf("%f\n", c[i]);
        }
#endif

        free_vector(b,1,m+1);
        free_matrix(a,1,m+1,1,m+1);
        free_ivector(indx,1,m+1);
}

float *convolve(float *A, float *B, int lenA, int lenB, int *lenC)
{
    int nconv;
    int i, j, i1;
    float tmp;
    float *C;

    //allocated convolution array
    nconv = lenA+lenB-1;
    C = (float*) calloc(nconv, sizeof(float));

    //convolution process
    for (i=0; i<nconv; i++)
    {
        i1 = i;
        tmp = 0.0;
        for (j=0; j<lenB; j++)
        {
            if(i1>=0 && i1<lenA)
                tmp = tmp + (A[i1]*B[j]);

            i1 = i1-1;
            C[i] = tmp;
        }
    }

    //get length of convolution array
    (*lenC) = nconv;

    //return convolution array
    return(C);
}

/**
    Get the FIR filter coefficients.
    Pr_L default value: 256
    coeff_size default: 256-1
*/
double * fir_coef(unsigned coeff_size, double window, int Pr_L)
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
    coeff_size = coeff_size+1;
    odd = coeff_size - (coeff_size/2)*2; /* odd = rem(N,2) */

    /*wind = hamming(N);*/
    for (i=0; i < Pr_L; i++)
    {
        wind[i] = 0.54 - 0.46 * cos ((2 * PI * i) / (N-1));
    }

    f1 = Wn / 2.0;
    c1 = f1;
    nhlf = (coeff_size+1) / 2;
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

int filter(int order, float *a, float *b, int np, float *in, float *out)
{
    int i, j;
    out[0] = b[0] * in[0];
    for (i = 0; i < ord; i++) {
        out[i] = 0.0;
        for (j = 0;j < i; j++)
            out[i] = out[i] + b[j] * in[i-j];
        for (j = 0;j < i-1; j++)
            out[i] = out[i] - a[j+1] * out[i-j-1];
    }
    // end of initial part
    for (i = ord; i < np+1; i++) {
        out[i] = 0.0;
        for (j = 0; j < ord; j++)
            out[i] = out[i] + b[j] * in[i-j];
        for (j = 0;j < ord-1; j++)
            out[i] = out[i] - a[j+1] * out[i-j-1];
    }

    return 0;
} /* end of filter */
