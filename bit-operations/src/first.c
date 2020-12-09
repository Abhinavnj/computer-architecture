#include <stdio.h>
#include <string.h>

int set(int x, int n, int v);
int get(int x, int n);
int comp(int x, int n);

int main(int argc, char* argv[])
{
    // Read input
    FILE *fp;
    fp = fopen(argv[1], "r");

    if (fp == NULL) {
        return 0;
    }

    int x, n, v;
    char operation[5];
    fscanf(fp, "%u\n", &x);
    while (fscanf(fp, "%s\t%u\t%u\n", operation, &n, &v) == 3) {
        if (strcmp(operation, "set") == 0) {
            x = set(x, n, v);
            printf("%d\n", x);
            // printf("set %dth bit of x to %d: %d\n", n, v, x);
        }
        else if (strcmp(operation, "get") == 0) {
            int bit = get(x, n);
            printf("%d\n", bit);
            // printf("%dth bit: %d\n", n, bit);
        }
        else {
            x = comp(x, n);
            printf("%d\n", x);
            // printf("complement %dth bit of x: %d\n", n, x);
        }
    }
}

int set(int x, int n, int v) {
    int changedNum;
    if (v == 1) {
        changedNum = (1 << n) | x;
    }
    else {
        changedNum = ~(1 << n) & x;
    }
    
    return changedNum;
}

int get(int x, int n) {
    int bit = (x >> n) & 1;
    return bit;
}

int comp(int x, int n) {
    int changedNum = (1 << n) ^ x;
    return changedNum;
}