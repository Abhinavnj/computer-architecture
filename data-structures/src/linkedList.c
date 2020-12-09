#include <stdio.h>
#include <stdlib.h>

struct Node {
    int value;
    struct Node* next;
};

void freeLL(struct Node* head) {
    struct Node* tempNode = head;
    while (head != NULL) {
        tempNode = head;
        head = head->next;
        free(tempNode);
    }
}

void printLL(struct Node* head, int count) {
    printf("%d\n", count);
    struct Node* ptr = head;
    struct Node* prev = NULL;
    while (ptr != NULL) {
        if (ptr == head || ptr->value != prev->value) {
            printf("%d\t", ptr->value);
        }
        prev = ptr;
        ptr = ptr->next;
    }
    printf("\n");
}

int insertNode(struct Node** head, int val) {
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
    newNode->value = val;
    newNode->next = NULL; // the next will always be null by default until changed

    if ((*head)->next == NULL && (*head)->value < val) {
        // set newNode as second node
        (*head)->next = newNode;
        return 0;
    }

    if ((*head)->value >= val) {
        // insert node at beginning (make it the head)
        newNode->next = *head;
        *head = newNode;
        return 0;
    }

    struct Node* ptr = *head;
    while (ptr->next != NULL && ptr->next->value < val) { // traverse until you get to a node whose next node's value is >= val
        // if the next->val is greater >= val, then insert node before next node
        ptr = ptr->next;
    }
    
    newNode->next = ptr->next;
    ptr->next = newNode;

    return 0;
}

int deleteNode(struct Node** head, int val, int* count) {
    if ((*head)->value == val) {
        struct Node* tempNode = *head;
        *head = (*head)->next;
        free(tempNode);
        (*count)--;
        return 0;
    }

    struct Node* ptr = *head;
    struct Node* prev = NULL;
    while (ptr != NULL && ptr->value != val) {
        prev = ptr;
        ptr = ptr->next;
    }

    if (ptr == NULL) {
        return 0;
    }
    if (ptr->value == val) {
        prev->next = ptr->next;
        free(ptr);
        (*count)--;
    }

    return 0;
}

int main(int argc, char* argv[])
{
    FILE *fp;
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("error\n");
        return 0;
    }
    
    // get first line
    int val, count = 0;
    char operation; // 'i' od 'd'
    int initial_items = fscanf(fp, "%c\t%d\n", &operation, &val);
    // check if first line is empty
    if (initial_items != 2) {
        printf("%d\n", 0); // 0 nodes
        return 0;
    }

    // CREATE HEAD

    struct Node* head = NULL;
    head = (struct Node*) malloc(sizeof(struct Node));
    head->next = NULL;

    if (operation == 'i') { // if first node is to be inserted then set the value of that to head
        count += 1;
        head->value = val;
    }
    else { // find the first node to insert
        while(operation != 'i') { // don't care about deletes until I hit an 'i'
            if (fscanf(fp, "%c\t%d\n", &operation, &val) != 2) { // reached end
                printf("%d\n", 0);
                return 0; // first node was delete and no other node was inserted
            }
        }
        count += 1; // found a node to insert
        //make node here with new first values
        head->value = val;
    }

    // POPULATE LINKED LIST

    while (fscanf(fp, "%c\t%d\n", &operation, &val) == 2) {
        if (operation == 'i') {
            if (head == NULL) {
                head = (struct Node*) malloc(sizeof(struct Node));
                head->next = NULL;
                head->value = val;
            }
            else {
                insertNode(&head, val);
            }
            count++;
        }
        else {
            if (head == NULL) {
                continue;
            }
            deleteNode(&head, val, &count);
        }
    }

    printLL(head, count);
    freeLL(head);

    return 0;
}