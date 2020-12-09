#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    char vowels[] = "aeiouAEIOU";

    for (int i = 1; i < argc; i++) {
        for (int j = 0; j < strlen(argv[i]); j++) {
            for (int k = 0; k < 10; k++) {
                if (argv[i][j] == vowels[k]) {
                    printf("%c", vowels[k]);
                    break;
                }
            }
        }
    }

    printf("\n");
    return 0;
}