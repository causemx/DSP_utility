#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

#define ARRAY_INITIAL_TORRENT 2600

int findpeaks(float *input, Peak *output, int count, int *output_length,
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
        if ((rs[i] < fs[i]) && (fq[i] < rq[i])) {
            if (input[i] > height) {
                // output[k_index++] = i;
                Peak *peak = &output[k_index];
                peak->location = i;
                peak->is_peak = 1;
                k_index++;
            }
        }
    }

    for (i = 1; i < k_index; i++) {
        interval = output[i].location - output[i-1].location;
        last_interval += interval;
        // printf("last_interval: %d\n", last_interval);
        if (last_interval > width) {
            last_interval = 0;
        } else {
            Peak *peak = &output[i];
            peak->is_peak = 0;
            // output[i] = -1;
        }
    }

    (*output_length) = k_index;

    free(r);
    free(f);
    return 0;
}
