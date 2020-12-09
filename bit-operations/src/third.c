#include <stdio.h>
#include <stdlib.h>

int get(int x, int n);

int main(int argc, char* argv[])
{
    int num = atoi(argv[1]);
    int i = 0, j = 15;
    while (i < 16 && j >= 0) {
        if (get(num, i) != get(num, j)) {
            printf("Not-Palindrome\n");
            return 0;
        }
        i++;
        j--;
    }

    printf("Is-Palindrome\n");
    return 0;
}

int get(int x, int n) {
    int bit = (x >> n) & 1;
    return bit;
}