#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct tree {
    int value;
    struct tree* left;
    struct tree* right;
};

typedef struct tree* treenode;

treenode createNode(int value);
treenode insertTree(treenode root, int value);
treenode deleteTree(treenode root, int value);
treenode findMin(treenode root);
int findTree(treenode root, int value);
treenode inorder(treenode root);
treenode preorder(treenode root);
treenode postorder(treenode root);
treenode levelOrder(treenode root);
treenode freeTree(treenode root);
int treeHeight(treenode root);
treenode printCurrentLevel(treenode root, int level);

int main() {
    treenode root = NULL;
    int choice, value;

    do {
        printf("\n--- Binarno stablo pretrazivanja ---\n");
        printf("1. Ubaci element\n");
        printf("2. Obrisi element\n");
        printf("3. Pronadji element\n");
        printf("4. Inorder ispis\n");
        printf("5. Preorder ispis\n");
        printf("6. Postorder ispis\n");
        printf("7. Level order ispis\n");
        printf("8. Obrisi cijelo stablo i izlaz\n");
        printf("Odabir: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Unesi vrijednost za ubacivanje: ");
            scanf("%d", &value);
            root = insertTree(root, value);
            break;
        case 2:
            printf("Unesi vrijednost za brisanje: ");
            scanf("%d", &value);
            root = deleteTree(root, value);
            break;
        case 3:
            printf("Unesi vrijednost za pretragu: ");
            scanf("%d", &value);
            if (findTree(root, value))
                printf("Element %d je u stablu.\n", value);
            else
                printf("Element %d nije u stablu.\n", value);
            break;
        case 4:
            printf("Inorder: ");
            inorder(root);
            printf("\n");
            break;
        case 5:
            printf("Preorder: ");
            preorder(root);
            printf("\n");
            break;
        case 6:
            printf("Postorder: ");
            postorder(root);
            printf("\n");
            break;
        case 7:
            printf("Level order: ");
            levelOrder(root);
            printf("\n");
            break;
        case 8:
            root = freeTree(root);
            printf("Stablo obrisano, izlazim.\n");
            break;
        default:
            printf("Nepostojeci odabir.\n");
        }
    } while (choice != 8);

    return 0;
}

treenode createNode(int value) {
    treenode newNode = (treenode)malloc(sizeof(struct tree));
    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

treenode insertTree(treenode root, int value) {
    if (root == NULL) {
        return createNode(value);
    }
    if (value < root->value) {
        root->left = insertTree(root->left, value);
    }
    else if (value > root->value) {
        root->right = insertTree(root->right, value);
    }
    return root;
}

treenode findMin(treenode root) {
    if (root == NULL) return NULL;
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

treenode deleteTree(treenode root, int value) {
    if (root == NULL) {
        return NULL;
    }

    if (value < root->value) {
        root->left = deleteTree(root->left, value);
    }
    else if (value > root->value) {
        root->right = deleteTree(root->right, value);
    }
    else {
        if (root->left == NULL) {
            treenode temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            treenode temp = root->left;
            free(root);
            return temp;
        }
        else {
            treenode temp = findMin(root->right);
            root->value = temp->value;
            root->right = deleteTree(root->right, temp->value);
        }
    }
    return root;
}

int findTree(treenode root, int value) {
    if (root == NULL) {
        return 0;
    }
    if (value == root->value) {
        return 1;
    }
    else if (value < root->value) {
        return findTree(root->left, value);
    }
    else {
        return findTree(root->right, value);
    }
}

treenode inorder(treenode root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->value);
        inorder(root->right);
    }
    return root;
}

treenode preorder(treenode root) {
    if (root != NULL) {
        printf("%d ", root->value);
        preorder(root->left);
        preorder(root->right);
    }
    return root;
}

treenode postorder(treenode root) {
    if (root != NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->value);
    }
    return root;
}

int treeHeight(treenode root) {
    if (root == NULL) {
        return 0;
    }
    int leftHeight = treeHeight(root->left);
    int rightHeight = treeHeight(root->right);
    if (leftHeight > rightHeight) {
        return leftHeight + 1;
    }
    else {
        return rightHeight + 1;
    }
}

treenode printCurrentLevel(treenode root, int level) {
    if (root == NULL) {
        return NULL;
    }
    if (level == 1) {
        printf("%d ", root->value);
    }
    else if (level > 1) {
        printCurrentLevel(root->left, level - 1);
        printCurrentLevel(root->right, level - 1);
    }
    return root;
}

treenode levelOrder(treenode root) {
    int height = treeHeight(root);
    for (int i = 1; i <= height; i++) {
        printCurrentLevel(root, i);
    }
    return root;
}

treenode freeTree(treenode root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
    return NULL;
}