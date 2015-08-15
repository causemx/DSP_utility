#include <math.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#define DEBUG_SAVGOL
#define NR_END 1
#define FREE_ARG char*
#define TINY 1.0e-20;


static float minarg1,minarg2;
#define FMIN(a,b) (minarg1=(a),minarg2=(b),(minarg1) < (minarg2) ?\
        (minarg1) : (minarg2))

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

void savgol(float *c, int np, int nl, int nr, int ld, int m)
{
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
                c[kk]=sum;
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


int convolve1D(float* in, float* out, int dataSize, float* kernel, int kernelSize)
{
    int i, j, k;

    // check validity of params
    if(!in || !out || !kernel) return -1;
    if(dataSize <=0 || kernelSize <= 0) return -1;

    // start convolution from out[kernelSize-1] to out[dataSize-1] (last)
    for(i = kernelSize-1; i < dataSize; ++i)
    {
        out[i] = 0;                             // init to 0 before accumulate

        for(j = i, k = 0; k < kernelSize; --j, ++k)
            out[i] += in[j] * kernel[k];
    }

    // convolution from out[0] to out[kernelSize-2]
    for(i = 0; i < kernelSize - 1; ++i)
    {
        out[i] = 0;                             // init to 0 before sum

        for(j = i, k = 0; j >= 0; --j, ++k)
            out[i] += in[j] * kernel[k];
    }

    return 1;
}

void do_help()
{
    printf("\nusage: savgol.exe [input data]\n");
}

int main(int argc, char const *argv[])
{
    int i;

    int input_index = 0;
    char line[32];
    char const *input_file;
    float *input_data = (float *) malloc(sizeof(float)*256);

    FILE *fp;

    if (argc < 2) {
        do_help();
        exit(1);
    } else {
        input_file = argv[1];
    }


    if((fp = fopen(input_file, "r")) != NULL) {
        while (fgets(line, sizeof line, fp) != NULL) {
            input_data[input_index++] = atof(line);
            input_index++;
        }
    } else {
        printf("\n file not found! \n");
        exit(-1);
    }

    float *kernel = (float *) malloc(sizeof(float)*11);
    savgol(kernel, 11, 5, 5, 0, 2);

    float *output_data = (float *) malloc(sizeof(float)*150);
    filter(10, a, c, 150, input_data, output_data);
    for (i = 0; i < input_index; i++)
        printf("%f\n", output_data[i]);

#if 0
    float *output_data = (float *) malloc(sizeof(float)*150);
    convolve1D(input_data, output_data, input_index, kernel, 11);
    for (i = 0; i < input_index; i++)
        printf("%f\n", output_data[i]);
#endif

    free(input_data);
    free(output_data);
    free(kernel);

    return 0;
}
