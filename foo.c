#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int do_help()
{
    printf("usage: ./foo [input_file]\n");
    return 0;
}

int main(int argc, char const *argv[])
{
    FILE *file;
    const char *file_name;
    char line[100];
    int index = 0;
    int value_dec;
    if (argc > 1)
    {
        file_name = argv[1];
    }
    else
    {
        do_help();
        exit(1);
    }

    file = fopen(file_name, "r");
    while (fgets(line, sizeof line, file) != NULL)
    {
        value_dec = (int)strtol(line, NULL, 16);
        printf("%d\n", value_dec);
        index++;
    }

    return 0;
}
