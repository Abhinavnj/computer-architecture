#include <stdio.h>
#include <stdlib.h>

struct Node {
    int value;
    struct Node* leftChild;
    struct Node* rightChild;
};

int inorderPrint(struct Node* root) {
    // inorder traversal tp print ascending
    if (root == NULL) {
        return 0;
    }
    inorderPrint(root->leftChild);
    printf("%d\t", root->value);
    inorderPrint(root->rightChild);

    return 0;
}

int freeBST(struct Node* root) {
    if (root == NULL) {
        return 0;
    }

    freeBST(root->leftChild);
    freeBST(root->rightChild);

    free(root);
    return 0;
}

struct Node* insertNode(struct Node* root, int newVal) {
    if (root == NULL) {
        struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
        newNode->value = newVal;
        newNode->leftChild = NULL;
        newNode->rightChild = NULL;
        return newNode;
    }

    if (newVal < root->value) {
        root->leftChild = insertNode(root->leftChild, newVal);
    }
    else if (newVal > root->value) {
        root->rightChild = insertNode(root->rightChild, newVal);
    }

    return root;
}

int main(int argc, char* argv[])
{
    FILE *fp;
    fp = fopen(argv[1], "r");

    if (fp == NULL) {
        printf("error");
        return 0;
    }

    int newVal;
    char op;
    int num_items_scanned = fscanf(fp, "%c\t%d\n", &op, &newVal);

    if (num_items_scanned != 2) { // file is empty
        return 0;
    }

    struct Node* root = NULL;
    root = (struct Node*) malloc(sizeof(struct Node));

    // initialize root
    root->value = newVal;
    root->leftChild = NULL;
    root->rightChild = NULL;

    while (fscanf(fp, "%c\t%d\n", &op, &newVal) == 2) {
        root = insertNode(root, newVal);
    }

    inorderPrint(root);
    printf("\n");

    freeBST(root);

    return 0;
}
