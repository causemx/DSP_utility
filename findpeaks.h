#ifndef FINDPEAK_H
#define FINDPEAK_H

typedef struct
{
    int location;
    int is_peak;
} Peak;

int findpeaks(float *input, Peak *output, int count, int *output_length,
    int width, float height);

#endif
