#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int count;
    int i;
    FILE *file;
    count = atoi(argv[1]);
    file = fopen("dataset.txt", "w+");
    srand(time(NULL));
    for (i = 0; i < count; i++)
        fprintf(file, "%d %d\n", rand() % 101, rand() % 256);

    fclose(file);
    return 0;
}
