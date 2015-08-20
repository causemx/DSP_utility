#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

int i, j;
int break_point = 1;
int break_points[2];
int length_bps = 0;

int * unique(int *src, size_t count)
{
    Vector vector;
    vector_init(&vector);
    int i, j, index, candidate;
    printf("the array length is: %d\n", count);
    for (i = 0; i < count; i++)
    {
        int hit_stat = 0;
        for (j = 0; j < i; j++)
        {
            if (src[j] == src[i])
                hit_stat = 1;
        }
        if (!hit_stat)
        {
            index++;
            vector_append(&vector, src[i]);
        }
    }

    int *ret;
    ret = malloc(sizeof(int) * index);
    for (i = 0; i < index; i++)
        ret[i] = vector_get(&vector, i);

    vector_free(&vector);
    return ret;
}

void dump_array(double src[][2], int m, int n)
{
    printf("dump array\n");
    int i, j;
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("%f\n", src[i][j]);
        }
    }
}

int detrend(double *src, size_t count)
{
    int N = count;
    printf("count: %d\n", count);
    int bp[2] = {1, N};
    length_bps = sizeof(bp)/sizeof(*bp);

    double a[N][2];
    for (i = 0; i < N; i++)
    {
        a[i][0] = i/(double)N;
        a[i][1] = 1;
    }
    dump_array(a, N, 2);
    // for (i = 0; i < N; i++)
    //    printf("%f\n", a[i][0]);
    return 0;
}

int main(int argc, char const *argv[])
{
    double value[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    detrend(value, sizeof(value)/sizeof(*value));
    // unique(value, sizeof(value)/sizeof(int));
    //int ret[] = unique(value);
    /* code */
    return 0;
}
