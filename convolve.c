#include <stdio.h>
#include <stdlib.h>

float *conv(float *A, float *B, int lenA, int lenB, int *lenC)
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

int main(int argc, char const *argv[])
{
    int i;
    int len_c = 0;
    float *out;
    float a[7] = {-1.0, 2.0, 3.0, -2.0, 0, 1.0, 2.0};
    float k[4] = {2.0, 4.0, -1.0, 1.0};

    out = conv(a, k, 7, 4, &len_c);
    for (i = 0; i < len_c; i++) {
        printf("%f\n", out[i]);
    }
    /* code */
    return 0;
}
