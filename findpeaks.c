#include <stdio.h>
#include "vector.h"

int * cumsum(double *src, size_t count)
{
    int i;
    double sum = src[0];
    double ret = malloc(count*sizeof(double));
    ret[0] = sum;
    printf("%lf\n", ret[0]);
    for (i = 1; i < count; i++) {
        sum += src[i];
        ret[i] = sum;
        printf("%lf\n", ret[i]);
    }
    return ret;
}

double * repmat(double src, size_t length, size_t duplicate_t)
{
    int i;
    double *ret = malloc((length*duplicate_t) * sizeof(double));;
    for (i = 0; i < length*duplicate_t; i++)
        ret[i] = src;
    return ret;
}

int findpeaks(double *val, int count)
{
    int i;
    int dx_length = count - 1;
    double dx[dx_length], rc[count];
    for (i = 0; i < dx_length; i++) {
        dx[i] = val[i+1] - val[i];
    }
    Vector vr, vf;
    vector_init(&vr);
    vector_init(&vf);
    for (i = 0; i < dx_length; i++) {
        if (dx[i] > 0)
            vector_append(&vr, i);
        else
            vector_append(&vf, i);
    }
    if (vr.size == 0 && vf.size == 0)
        exit(1);

    double dr[vr.size] = vr;
    for (i = 1; i < vr.size; i++) {
        dr[i] = vr[i+1] - vr[i];
    }
    double rc[count];
    double *rs;
    for (i = 0; i < count; i++)
        rc[i] = 1;
    for (i = 0; i < vr.size; i++) {
        rc[vr[i]+1] = 1- dr[i];
    }
    rc[0] = 0;
    rs = cumsum(rc);    // TODO need free

    double df[vf.size] = vf;
    for (i = 1; i < vf.size; i++) {
        df[i] = vf[i+1] - vf[i];
    }
    double fc[count];
    double *fs;
    for (i = 0; i < count; i++)
        fc[i] = 1;
    for (i = 0; i < vf.size; i++) {
        fc[vf[i]+1] = 1- df[i];
    }
    fc[0] = 0;
    fs = cumsum(fc);    // TODO need free

    double *rp = repmat(-1, count, 1);  // TODO need free
    double rc[count];
    double *rs;
    for (i = 0; i < count; i++) {
        rc[i] = 1;
    }

    for (i = 0; i < vr.size; i++) {
        rc[vector_get(&vr, i)+1] = 1 - dr[i];
    }
    rs = cumsum(rc, count);


}

int main(int argc, char const *argv[])
{
  int i;
  double val[] = {1, 2, 3, 4, 5, 4, 3, 2, 1};
  cumsum(val, 9);
  return 0;
}
