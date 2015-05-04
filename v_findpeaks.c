#include <stdio.h>
#include <stdlib.h>


double * repmat(double src, size_t length, size_t duplicate_t)
{
    int i;
    double *ret = malloc((length*duplicate_t) * sizeof(double));;
    for (i = 0; i < length*duplicate_t; i++)
        ret[i] = src;
    return ret;
}

int find_peak(double *src, size_t count)
{
    int i;
    double r[26];
    double dr[26];
    double rp[50];
    for (i = 0; i < 50; i++) {
        if (i = 0)
            rp[0] = (dr[i] - 1);
        else
            rp[r[i]] = dr[i] - 1;
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    int i;
    double *myarr;
    myarr = repmat(-1, 50, 1);
    for (i = 0; i < 50; i++)
        printf("%lf\n", myarr[i]);

    free(myarr);
    return 0;
}
