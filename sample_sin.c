#include <stdio.h>
#include <math.h>

#define PI 3.14159265

int main(int argc, char const *argv[])
{
    double i, ret, val;
    val = PI/180;
    FILE *file;
    file = fopen("sample_data.log", "w");
    for (i = 0; i < 500; i += 5)
    {
        ret = sin(i*val);
        // printf("the sine of %f is: %lf\n", i, ret);
        fprintf(file, "%f,\n", ret);
    }
    fclose(file);
    return 0;
}
