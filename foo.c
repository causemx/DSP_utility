#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "algo_resp.h"

int do_help()
{
    printf("usage: ./foo [input_file]\n");
    return 0;
}

int main(int argc, char const *argv[])
{
    FILE *fp;
    const char *input_file;
    char line[32];
    int i, index = 0;
    float *input_data = (float *) malloc(sizeof(float)*256);

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

    for (i = 0; i < index; i++) {
        printf("%f\n", input_data[i]);
    }

    float *my_fir_coef = (float *) malloc(sizeof(float)*128);
    my_fir_coef = fir_coef((128-1), 0.8, 128);

    fclose(fp);
    free(input_data);
    free(my_fir_coef);

    return 0;
}
