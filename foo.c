#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "algo_resp.h"
#include "findpeaks.h"

// #define DEBUG_FOO

int do_help()
{
    printf("usage: ./foo [input_file]\n");
    return 0;
}

int main(int argc, char const *argv[])
{
    FILE *fp;
    int i, index = 0;
    char line[32];
    const char *input_file;
    float *input_data = (float *) malloc(sizeof(float)*2520);

    if (argc > 1) {
        input_file = argv[1];
    } else {
        do_help();
        exit(1);
    }

    if((fp = fopen(input_file, "r")) == NULL) {
        perror(input_file);
    }

    while (fgets(line, sizeof line, fp) != NULL) {
        input_data[index++] = atof(line);
    }

#ifdef DEBUG_FOO
    for (i = 0; i < index; i++)
        printf("%f\n", input_data[i]);
#endif

    int order_fir_coef = 128;
    // retrive the FIR coefficients(parameter b in 1d-filter)
    float *my_fir_coef = (float *) malloc(sizeof(float)*order_fir_coef);
    my_fir_coef = fir_coef((order_fir_coef-1), 0.8, order_fir_coef);

#ifdef DEBUG_FOO
    printf("dump fir_coef\n");
    for (i = 0; i < order_fir_coef; i++)
        printf("%f\n", my_fir_coef[i]);
#endif

    // retrive the parameter a in 1-d filter
    float my_fir_coef_a[order_fir_coef];
    for (i = 0; i < order_fir_coef; i++) {
        if (i) my_fir_coef_a[i] = 0;
        else my_fir_coef_a[i] = 1;
    }

    int len_sample_data = 2520;
    float *fir_filter_output = (float *) malloc(sizeof(float)*2520);
    filter(order_fir_coef, my_fir_coef_a, my_fir_coef, len_sample_data, input_data, fir_filter_output);

#ifdef DEBUG_FOO
    printf("dump fir_output\n");
    for (i = 0; i < len_sample_data; i++)
        printf("%d, %f\n", i, fir_filter_output[i]);
#endif

    float *kernel = (float *) malloc(sizeof(float)*41);
    savgol(kernel, 41, 20, 20, 0, 3);
#ifdef DEBUG_FOO
    printf("dump savgol kernel\n");
    for (i = 0; i < 41; i++)
        printf("%d, %f\n", i, kernel[i]);
#endif

    int len_savgol_output = 0;
    float *out_savgol_filter;
    out_savgol_filter = convolve(fir_filter_output, kernel, 2520, 41, &len_savgol_output);

#ifdef DEBUG_FOO
    printf("dump savgol result\n");
    for (i = 0; i < len_savgol_output; i++)
        printf("%f\n", out_savgol_filter[i]);
#endif

    int len_output_peaks = 0;
    // int* output_peaks = (int *) malloc(sizeof(int)*len_savgol_output);
    Peak* output_peaks = (Peak *) malloc(sizeof(Peak)*len_savgol_output);
    findpeaks(out_savgol_filter, output_peaks, len_savgol_output, &len_output_peaks, 100, 100);

    printf("len_output: %d\n", len_output_peaks);

#if 0
    for (i = 0; i < len_output_peaks; i++) {
        Peak peak = output_peaks[i];
        printf("is_peak: %d, location: %d\n", peak.is_peak, peak.location);
    }
#endif

    fclose(fp);
    free(kernel);
    free(input_data);
    free(my_fir_coef);
    free(fir_filter_output);
    free(out_savgol_filter);
    free(output_peaks);

    return 0;
}
