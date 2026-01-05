#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(int value);
Node* insertCustom();
int inorder(Node* root, FILE* f);
int replace(Node* root);
int freeTree(Node* root);

int main() {
    Node* root = insertCustom();

    FILE* f = fopen("output.txt", "w");
    inorder(root, f);
    fprintf(f, "\n");

    replace(root);

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

Node* insertCustom() {
    Node* root = createNode(2);
    root->left = createNode(5);
    root->right = createNode(1);

    root->left->left = createNode(7);
    root->left->right = createNode(4);

    root->left->left->left = createNode(8);
    root->left->left->left->left = createNode(11);
    root->left->left->left->right = createNode(7);

    root->left->right->right = createNode(2);
    root->left->right->right->left = createNode(3);

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
