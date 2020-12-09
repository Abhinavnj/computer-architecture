#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int num = atoi(argv[1]);
    int oneBitCount = 0;
    int onePairCount = 0;

    while (num != 0) {
        int bit1 = num & 1; // right most bit
        if (bit1 == 1) {
            oneBitCount++;
        }
        num = num >> 1; // shift number right
        if (bit1 == 1) {
            int bit2 = num & 1; // next right most bit
            if (bit2 == 1) {
                oneBitCount++;
                onePairCount++;
                num = num >> 1;
            }
        }
    }

    if (oneBitCount % 2 == 0) {
        printf("Even-Parity\t%d\n", onePairCount);
    }
    else {
        printf("Odd-Parity\t%d\n", onePairCount);
    }

    return 0;
}