#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DirNode {
    char name[100];
    struct DirNode* parent;
    struct DirNode* firstChild;
    struct DirNode* nextSibling;
} DirNode;

DirNode* currentDir = NULL;
DirNode* root = NULL;

DirNode* createDir(const char* name, DirNode* parent) {
    DirNode* newDir = (DirNode*)malloc(sizeof(DirNode));
    if (!newDir) {
        printf("Greska pri alokaciji memorije!\n");
        return NULL;
    }

    strcpy(newDir->name, name);
    newDir->parent = parent;
    newDir->firstChild = NULL;
    newDir->nextSibling = NULL;

    return newDir;
}

void md(const char* dirName) {
    DirNode* temp = currentDir->firstChild;
    while (temp != NULL) {
        if (strcmp(temp->name, dirName) == 0) {
            printf("Direktorij '%s' vec postoji!\n", dirName);
            return;
        }
        temp = temp->nextSibling;
    }

    DirNode* newDir = createDir(dirName, currentDir);

    if (currentDir->firstChild == NULL) {
        currentDir->firstChild = newDir;
    }
    else {
        newDir->nextSibling = currentDir->firstChild;
        currentDir->firstChild = newDir;
    }

    printf("Direktorij '%s' kreiran.\n", dirName);
}

void cd(const char* dirName) {
    if (strcmp(dirName, "..") == 0) {
        return;
    }

    DirNode* temp = currentDir->firstChild;
    while (temp != NULL) {
        if (strcmp(temp->name, dirName) == 0) {
            currentDir = temp;
            printf("Trenutni direktorij: %s\n", currentDir->name);
            return;
        }
        temp = temp->nextSibling;
    }

    printf("Direktorij '%s' ne postoji!\n", dirName);
}

void cdParent() {
    if (currentDir->parent != NULL) {
        currentDir = currentDir->parent;
        printf("Trenutni direktorij: %s\n", currentDir->name);
    }
    else {
        printf("Vec ste u root direktoriju!\n");
    }
}

void dir() {
    printf("\nSadrzaj direktorija '%s':\n", currentDir->name);
    printf("--------------------------------\n");

    if (currentDir->firstChild == NULL) {
        printf("Direktorij je prazan.\n");
        return;
    }

    DirNode* temp = currentDir->firstChild;
    int count = 1;

    while (temp != NULL) {
        printf("%d. %s <DIR>\n", count, temp->name);
        temp = temp->nextSibling;
        count++;
    }
    printf("--------------------------------\n");
    printf("Ukupno: %d direktorij(a)\n", count - 1);
}

void printPath() {
    if (currentDir == NULL) return;

    char path[1000] = "";
    DirNode* temp = currentDir;

    while (temp != NULL) {
        char tempPath[1000];
        strcpy(tempPath, temp->name);
        strcat(tempPath, "\\");
        strcat(tempPath, path);
        strcpy(path, tempPath);
        temp = temp->parent;
    }

    printf("\nTrenutna putanja: %s\n", path);
}

void freeTree(DirNode* node) {
    if (node == NULL) return;

    DirNode* child = node->firstChild;
    while (child != NULL) {
        DirNode* nextChild = child->nextSibling;
        freeTree(child);
        child = nextChild;
    }

    free(node);
}

void printMenu() {
    printf("\n========== DOS SIMULATOR ==========\n");
    printPath();
    printf("\n1. md  (kreiraj direktorij)\n");
    printf("2. cd dir  (promijeni direktorij)\n");
    printf("3. cd..  (idi na roditeljski direktorij)\n");
    printf("4. dir  (prikazi sadrzaj direktorija)\n");
    printf("5. Izlaz\n");
    printf("===================================\n");
    printf("Izbor: ");
}

int main() {
    int choice;
    char dirName[100];

    root = createDir("C:", NULL);
    currentDir = root;

    printf("DOS Simulator - Struktura direktorija pomocu vezanih listi\n");

    do {
        printMenu();
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Unesite ime direktorija: ");
            scanf("%s", dirName);
            md(dirName);
            break;

        case 2:
            printf("Unesite ime direktorija: ");
            scanf("%s", dirName);
            cd(dirName);
            break;

        case 3:
            cdParent();
            break;

        case 4:
            dir();
            break;

        case 5:
            printf("Izlaz iz programa...\n");
            break;

        default:
            printf("Nepoznata naredba! Pokusajte ponovno.\n");
        }

    } while (choice != 5);

    freeTree(root);

    return 0;
}