#include <stdio.h>
#include <stdlib.h>
#include "findpeaks.h"

void cumsum(int *, int *, size_t);
void ones(int *, size_t);
void copy_array(int *, int *, size_t);


void cumsum(int *input, int *output, size_t length)
{
    int i;
    int sum = input[0];
    output[0] = sum;
    for (i = 1; i < length; i++) {
        sum += input[i];
        output[i] = sum;
    }
}

void ones(int *input, size_t length)
{
    int i;
    for (i = 0; i < length; i++)
        input[i] = 1;
}

void copy_array(int *input, int *output, size_t length)
{
    int i;
    for (i = 0; i < length; i++)
        output[i] = input[i];
}

#define ARRAY_INITIAL_TORRENT 100

int findpeaks(float *input, int *output, int count, int *output_length,
    int width, float height)
{
    int i = 0;
    int dx_length = count - 1;
    float dx[dx_length];
    for (i = 0; i < dx_length; i++) {
        dx[i] = input[i+1] - input[i];
        // printf("index: %i, dx: %lf\n", i, dx[i]);
    }

    int r_length = 0;
    int f_length = 0;
    int *r = malloc(ARRAY_INITIAL_TORRENT * sizeof(int));
    int *f = malloc(ARRAY_INITIAL_TORRENT * sizeof(int));
    for (i = 0; i < dx_length; i++) {
        if (dx[i] > 0)
            r[r_length++] = i;
        else
            f[f_length++] = i;
    }
    // printf("r_length: %d, f_length: %d\n", r_length, f_length);

    if (r_length == 0 && f_length == 0)
        exit(1);

    /* time since the last rise */
    int dr[r_length], rc[count], rs[count];
    copy_array(r, dr, r_length);
    for (i = 0; i < r_length-1; i++) {
        dr[i+1] = r[i+1] - r[i];
    }

    ones(rc, count);
    for (i = 0; i < r_length; i++) {
        if (i == 0)
            rc[i] = 0;
        else
            rc[r[i]+1] = 1 - dr[i];
    }
    cumsum(rc, rs, count);

    /*for (i = 0; i < count; i++)
        printf("index: %d, rc %d\n", i, rc[i]);*/

    /* time since the last fall*/
    int df[f_length], fc[count], fs[count];
    copy_array(f, df, f_length);
    for (i = 1; i < f_length; i++) {
        df[i] = f[i] - f[i-1];
    }
    ones(fc, count);
    for (i = 0; i < f_length; i++) {
        if (i == 0)
            fc[i] = 0;
        else
            fc[f[i]+1] = 1 - df[i];
    }
    cumsum(fc, fs, count);

    /*for (i = 0; i < count; i++)
        printf("index: %d, fc %d\n", i, fc[i]);*/

    /* time to the nexy rise */
    int rp[count], rq[count];
    for (i = 0; i < count; i++)
        rp[i] = -1;
    rp[0] = dr[0];

    for (i = 0; i < r_length-1; i++)
        rp[r[i]+1] = dr[i+1] -1;
    cumsum(rp, rq, count);

    // dump_array(rp, count);

    /* time to the next fall */

    int fp[count], fq[count];
    for (i = 0; i < count; i++)
        fp[i] = -1;
    fp[0] = df[0];
    for (i = 0; i < f_length; i++)
        fp[f[i]+1] = df[i+1] -1;
    cumsum(fp, fq, count);

    int interval;
    int last_interval = 0;
    int k_index = 0;
    for (i = 0; i < count; i++) {
        if ((rs[i] < fs[i]) && (fq[i] < rq[i]) && ((fq[i] - rs[i])/2 ==0))
            if (input[i] > height) output[k_index++] = i;
    }
    for (i = 1; i < k_index; i++) {
        interval = output[i] - output[i-1];
        last_interval = last_interval + interval;
        if (last_interval > width)
            last_interval = 0;
        else
            output[i] = -1;
    }
    (*output_length) = k_index;

    free(r);
    free(f);
    return 0;
}

float raw_data[50] = {
    0,
    0.852321569719618,
    0.891559230411004,
    0.0802816748428135,
    -0.807581690968336,
    -0.925041371738203,
    -0.160045086043251,
    0.757628415392720,
    0.952551847531460,
    0.238775315644031,
    -0.702784219655085,
    -0.973913062101399,
    -0.315964115459776,
    0.643403154075158,
    0.988987116719693,
    0.391113188106663,
    -0.579868557034903,
    -0.997676699833400,
    -0.463737403794016,
    0.512590580312337,
    0.999925715266099,
    0.533367932111660,
    -0.442003541319696,
    -0.995719644350459,
    -0.599555268595492,
    0.368563119338771,
    0.985085639654449,
    0.661872136533216,
    -0.292743413853263,
    -0.968092349696147,
    -0.719916245277281,
    0.215033883968319,
    0.944849475778709,
    0.773312887258672,
    -0.135936188675039,
    -0.915507063806374,
    -0.821717356936336,
    0.0559609483578025,
    0.880254535653247,
    0.864817176066493,
    0.0243755515492913,
    -0.839319466337903,
    -0.902334110926544,
    -0.104554693477050,
    0.792966114897856,
    0.934025968471188,
    0.184058875691088,
    -0.741493718447948,
    -0.959688159825614,
    -0.262374853703929
};

int main(int argc, char const *argv[])
{
    int i, ret_length = 0;
    int *ret = (int *) malloc(sizeof(int)*50);
    findpeaks(raw_data, ret, 50, &ret_length, 10, 0.5);
    for (i = 0; i < ret_length; i++)
        printf("%d\n", ret[i]);

    free(ret);
    return 0;
}
