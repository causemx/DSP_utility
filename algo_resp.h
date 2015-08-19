#ifndef ALGO_RESP_H
#define ALGO_RESP_H

float *vector(long, long);
int *ivector(long, long);
float **matrix(long, long, long, long);

void free_vector();
void free_ivector();
void free_matrix();

void lubksb(float **a, int n, int *indx, float b[]);
void ludcmp(float **a, int n, int *indx, float *d);

void savgol(float *c, int np, int nl, int nr, int ld, int m);
float *convolve(float *A, float *B, int lenA, int lenB, int *lenC);

float * fir_coef(unsigned, float, int);
int filter(int, float *, float *, int, float *, float *);

#endif
