#include <stdio.h>
#include <stdlib.h>

struct Node {
    int key;
    struct Node* next;
};

struct Node* hashTable[10000];

int hash(int newVal) {
    int bucket = newVal % 10000;
    if (newVal < 0) {
        bucket += 10000;
    }
    return bucket;
}

int insertNode(int newVal, int* collisionCount) {
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
    newNode->key = newVal;
    newNode->next = NULL;
    int bucket = hash(newVal);
    if (hashTable[bucket] == NULL) {
        hashTable[bucket] = newNode;
        return bucket;
    }
    else {
        struct Node* ptr = hashTable[bucket];
        while (ptr->next != NULL) { // get last node in chain
            ptr = ptr->next;
        }

        ptr->next = newNode;
        (*collisionCount)++;
    }
    
    return bucket;
}

int searchTable(int val, int* searchCount) {
    int bucket = hash(val);
    struct Node* ptr = hashTable[bucket];

    if (ptr == NULL) {
        return -1;
    }

    while (ptr != NULL && ptr->key != val) {
        ptr = ptr->next;
    }

    if (ptr != NULL) {
        (*searchCount)++;
    }

    return 0;
}

int main(int argc, char* argv[])
{
    FILE *fp;
    fp = fopen(argv[1], "r");

    int newVal;
    char operation; // 'i' or 's'
    int num_items_scanned = fscanf(fp, "%c\t%d\n", &operation, &newVal);

    if (num_items_scanned != 2) { // file is empty
        printf("%d\n%d\n", 0, 0); // 0 collisions, 0 searched
        return 0;
    }

    // initialize hashTable to NULL
    for (int i = 0; i < 10000; i++) {
        hashTable[i] = NULL;
    }

    int collisionCount = 0;
    int searchCount = 0;
    while (num_items_scanned == 2) {
        if (operation == 'i') {
            insertNode(newVal, &collisionCount);
        }
        else {
            searchTable(newVal, &searchCount);
        }
        num_items_scanned = fscanf(fp, "%c\t%d\n", &operation, &newVal);
    }

    printf("%d\n", collisionCount);
    printf("%d\n", searchCount);

    return 0;
}
