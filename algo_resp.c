#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

/**
    Get the FIR filter coefficients.
    Pr_L default value: 256
    coeff_size default: 256-1
*/
double * fir1(unsigned coeff_size, double window, int Pr_L)
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

sma_result_t sma(enum Action action, ...)
{
    va_list vl;
    sma_result_t ret;
    sma_obj_t *obj;
    double val;
    va_start(vl, action);

    switch (action) {
    case SMA_NEW:
        // initialize a new result
        ret.handle = malloc(sizeof(sma_obj_t));
        ret.handle->sma = 0.0;
        ret.handle->sum = 0.0;
        ret.handle->period = va_arg(vl, int);
        ret.handle->values = malloc(ret.handle->period * sizeof(double));
        ret.handle->index = 0;
        break;
    case SMA_FREE:
        ret.handle = va_arg(vl, sma_obj_t *);
        free(ret.handle->values);
        free(ret.handle);
        ret.handle = NULL;
        break;
    case SMA_VALUES:
        obj = va_arg(vl, sma_obj_t *);
        ret.values = obj->values;
        break;
    case SMA_MEAN:
        obj = va_arg(vl, sma_obj_t *);
        ret.sma = obj->sma;
        break;
    case SMA_ADD:
        obj = va_arg(vl, sma_obj_t *);
        val = va_arg(vl, double);
        if (obj->index < obj->period) {
            obj->values[obj->index++] = val;
            obj->sum += val;
            obj->sma = obj->sum / obj->index;
        } else {
            obj->sum -= obj->values[obj->index % obj->period];
            obj->sum += val;
            obj->sma = obj->sum / obj->period;
            obj->values[obj->index % obj->period] = val;
            obj->index++;
        }
        ret.sma = obj->sma;
        break;
    }
    va_end(vl);
    return ret;
}
