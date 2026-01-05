#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(int value);
Node* insert(Node* root, int value);
int replace(Node* root);
int inorderTraversal(Node* root, FILE* file, int isFirst);
Node* buildTreeFromArray(int arr[], int n);
Node* buildTreeRandom(int n);
int writeInorderToFile(Node* root, const char* filename, const char* label);
int freeTree(Node* root);

int main() {
    int choice;
    Node* root = NULL;

    printf("Odaberite nacin rada:\n");
    printf("1 - Koristi zadani niz\n");
    printf("2 - Generiraj slucajne brojeve\n");
    printf("Vas izbor: ");
    scanf("%d", &choice);

    if (choice == 1) {
        int arr[] = { 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 };
        int n = 10;
        root = buildTreeFromArray(arr, n);
    }
    else {
        int n;
        printf("Unesite broj cvorova: ");
        scanf("%d", &n);
        root = buildTreeRandom(n);
    }

    remove("output.txt");

    writeInorderToFile(root, "output.txt", "Inorder nakon umetanja: ");

    replace(root);

    writeInorderToFile(root, "output.txt", "Inorder nakon replace: ");

    int freedNodes = freeTree(root);

    printf("Podaci su spremljeni u output.txt\n");
    printf("Oslobodjeno cvorova: %d\n", freedNodes);

    return 0;
}

Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

Node* insert(Node* root, int value) {
    if (root == NULL) {
        return createNode(value);
    }

    Node* queue[1000];
    int front = 0;
    int rear = 0;
    queue[rear] = root;
    rear = rear + 1;

    while (front < rear) {
        Node* current = queue[front];
        front = front + 1;

        if (current->left == NULL) {
            current->left = createNode(value);
            break;
        }
        else {
            queue[rear] = current->left;
            rear = rear + 1;
        }

        if (current->right == NULL) {
            current->right = createNode(value);
            break;
        }
        else {
            queue[rear] = current->right;
            rear = rear + 1;
        }
    }

    return root;
}

int replace(Node* root) {
    if (root == NULL) {
        return 0;
    }

    int leftSum = replace(root->left);
    int rightSum = replace(root->right);

    int oldValue = root->value;
    root->value = leftSum + rightSum;

    return oldValue + leftSum + rightSum;
}

int inorderTraversal(Node* root, FILE* file, int isFirst) {
    if (root == NULL) {
        return isFirst;
    }

    isFirst = inorderTraversal(root->left, file, isFirst);

    if (isFirst) {
        fprintf(file, "%d", root->value);
        isFirst = 0;
    }
    else {
        fprintf(file, " %d", root->value);
    }

    isFirst = inorderTraversal(root->right, file, isFirst);

    return isFirst;
}

Node* buildTreeFromArray(int arr[], int n) {
    Node* root = NULL;
    for (int i = 0; i < n; i++) {
        root = insert(root, arr[i]);
    }
    return root;
}

Node* buildTreeRandom(int n) {
    srand(time(NULL));
    Node* root = NULL;
    for (int i = 0; i < n; i++) {
        int randomValue = rand() % 81 + 10;
        root = insert(root, randomValue);
    }
    return root;
}

int writeInorderToFile(Node* root, const char* filename, const char* label) {
    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        return 0;
    }

    fprintf(file, "%s", label);
    inorderTraversal(root, file, 1);
    fprintf(file, "\n");

    fclose(file);
    return 1;
}

int freeTree(Node* root) {
    if (root == NULL) {
        return 0;
    }

    int leftFreed = freeTree(root->left);
    int rightFreed = freeTree(root->right);

    free(root);

    return 1 + leftFreed + rightFreed;
}