#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

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

int main(int argc, char const *argv[])
{
    int i;
    double val[] = {1,2,3,4,5,6,7,8,9,10};
    sma_obj_t *h3 = sma(SMA_NEW, 3).handle;
    sma_obj_t *h5 = sma(SMA_NEW, 5).handle;

    for (i = 0; i < sizeof(val)/sizeof(double); i++) {
        printf(
            "next number %lf, SMA_3 = %lf, SMA_5 = %lf\n",
            val[i],
            sma(SMA_ADD, h3, val[i]).sma,
            sma(SMA_ADD, h5, val[i]).sma
            );
    }
    sma(SMA_FREE, h3);
    sma(SMA_FREE, h5);
    return 0;
}
