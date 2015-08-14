#ifndef ALGO_RESP_H
#define ALGO_RESP_H

typedef struct sma_obj {
    double sma;
    double sum;
    int period;
    double *values;
    int index;
} sma_obj_t;

typedef struct sma_result {
    sma_obj_t *handle;
    double sma;
    double *values;
} sma_result_t;

enum Action {
    SMA_NEW,
    SMA_FREE,
    SMA_VALUES,
    SMA_ADD,
    SMA_MEAN
};

double * fir1(unsigned, double, int);
int filter(int, float *, float *, int, float *, float *);
sma_result_t sma(enum, ...);

#endif
