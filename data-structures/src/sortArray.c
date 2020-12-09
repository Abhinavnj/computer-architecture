#include <stdio.h>

void printArray(int *arrPointer, int arrLen) {
    for (int i = 0; i < arrLen; i++) {
        printf("%d\t", arrPointer[i]);
    }
    printf("\n");
}

int sortAsc(int *arrPointer, int arrLen) {
    int i, j;
    // bubble sort
    for (i = 0; i < arrLen - 1; i++) {
        for (j = 0; j < arrLen - i - 1; j++) {
            if (arrPointer[j] > arrPointer[j+1]) {
                // swap
                int temp = arrPointer[j];
                arrPointer[j] = arrPointer[j+1];
                arrPointer[j+1] = temp;
            }
        }
    }

    return 0;
}

int sortDesc(int *arrPointer, int arrLen) {
    int i, j;
    // bubble sort
    for (i = 0; i < arrLen - 1; i++) {
        for (j = 0; j < arrLen - i - 1; j++) {
            if (arrPointer[j] < arrPointer[j+1]) {
                // swap
                int temp = arrPointer[j];
                arrPointer[j] = arrPointer[j+1];
                arrPointer[j+1] = temp;
            }
        }
    }

    return 0;
}

int main(int argc, char* argv[])
{
    FILE *fp;
    fp = fopen(argv[1], "r");
    int len;
    int numItems = fscanf(fp, "%d", &len);

    if (numItems == 0) {
        printf("File is empty\n");
        return 0;
    }

    int numbers[len], num;
    int oddAmount = 0; // store amount of odd numbers given
    int evenAmount = 0; // store amount of even numbers given
    int i = 0;
    while(fscanf(fp, "%d", &num) == 1) {
        if (num % 2 == 0) {
            evenAmount++;
        }
        else {
            oddAmount++;
        }
        numbers[i] = num;
        i++;
    }

    int evenArr[evenAmount]; // array to store even numbers
    int oddArr[oddAmount]; // array to store odd numbers

    int evenCounter = 0; // track index where to put evens from array
    int oddCounter = 0; // track index where to put odds from array
    for (i = 0; i < len; i++) {
        if (numbers[i] % 2 == 0) {
            evenArr[evenCounter] = numbers[i];
            evenCounter++;
        }
        else {
            oddArr[oddCounter] = numbers[i];
            oddCounter++;
        }
    }

    sortAsc(evenArr, evenAmount);
    sortDesc(oddArr, oddAmount);

    // merge odd/even arrays to numbers array
    for (i = 0; i < len; i++) {
        if (i < evenAmount) {
            numbers[i] = evenArr[i];
        }
        else {
            numbers[i] = oddArr[i - evenAmount];
        }
    }

    printArray(numbers, len);

    return 0;
}