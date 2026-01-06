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
Node* insertRandom();
int inorder(Node* root, FILE* f);
int replace(Node* root);
int freeTree(Node* root);

int main() {
    srand(time(NULL));

    Node* root = insertRandom();

    FILE* f = fopen("inorder_stablo.txt", "w");
    fprintf(f, "Inorder prolazak nakon kreiranja stabla:\n");
    inorder(root, f);
    fprintf(f, "\n\n");

    replace(root);

    fprintf(f, "Inorder prolazak nakon replace() funkcije:\n");
    inorder(root, f);

    fclose(f);
    freeTree(root);
    return 0;
}

Node* createNode(int value) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->value = value;
    n->left = NULL;
    n->right = NULL;
    return n;
}

Node* insertRandom() {
    Node* root = createNode(rand() % 81 + 10);
    root->left = createNode(rand() % 81 + 10);
    root->right = createNode(rand() % 81 + 10);

    root->left->left = createNode(rand() % 81 + 10);
    root->left->right = createNode(rand() % 81 + 10);

    root->left->left->left = createNode(rand() % 81 + 10);
    root->left->left->left->left = createNode(rand() % 81 + 10);
    root->left->left->left->right = createNode(rand() % 81 + 10);

    root->left->right->right = createNode(rand() % 81 + 10);
    root->left->right->right->left = createNode(rand() % 81 + 10);

    return root;
}

int inorder(Node* root, FILE* f) {
    if (!root) return 0;
    inorder(root->left, f);
    fprintf(f, "%d ", root->value);
    inorder(root->right, f);
    return 1;
}

int replace(Node* root) {
    if (!root) return 0;
    int l = replace(root->left);
    int r = replace(root->right);
    int old = root->value;
    root->value = l + r;
    return root->value + old;
}

int freeTree(Node* root) {
    if (!root) return 0;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
    return 1;
}