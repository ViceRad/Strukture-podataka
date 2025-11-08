#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _poly* position;
typedef struct _poly {
    int coeff;
    int exp;
    position next;
} poly;

position createElement(int coeff, int exp);
position insertSorted(position head, int coeff, int exp);
position readPolynomialFromFile(position head, const char* filename);
position printList(position head);
position addPolynomials(position headA, position headB, position result);
position multiplyPolynomials(position headA, position headB, position result);
position freeList(position head);

int main() {
    poly headA = { .coeff = 0, .exp = 0, .next = NULL };
    poly headB = { .coeff = 0, .exp = 0, .next = NULL };
    poly result = { .coeff = 0, .exp = 0, .next = NULL };

    readPolynomialFromFile(&headA, "poly1.txt");
    printf("Prvi polinom: ");
    printList(&headA);

    readPolynomialFromFile(&headB, "poly2.txt");
    printf("Drugi polinom: ");
    printList(&headB);

    addPolynomials(&headA, &headB, &result);
    printf("Zbroj polinoma: ");
    printList(&result);
    freeList(&result);

    result.next = NULL;
    multiplyPolynomials(&headA, &headB, &result);
    printf("Umnozak polinoma: ");
    printList(&result);

    freeList(&headA);
    freeList(&headB);
    freeList(&result);

    return 0;
}

position createElement(int coeff, int exp) {
    position newE = (position)malloc(sizeof(poly));
    if (newE == NULL) {
        printf("Greska pri alokaciji memorije\n");
        return NULL;
    }
    newE->coeff = coeff;
    newE->exp = exp;
    newE->next = NULL;
    return newE;
}

position insertSorted(position head, int coeff, int exp) {
    if (coeff == 0) return head;

    position newElement = createElement(coeff, exp);
    position current = head;

    while (current->next != NULL && current->next->exp > exp) {
        current = current->next;
    }

    if (current->next != NULL && current->next->exp == exp) {
        current->next->coeff += coeff;
        free(newElement);
        return head;
    }
    else {
        newElement->next = current->next;
        current->next = newElement;
        return head;
    }
}

position readPolynomialFromFile(position head, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Greska pri otvaranju datoteke %s!\n", filename);
        return head;
    }

    char line[256];
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Greska pri citanju datoteke %s!\n", filename);
        fclose(file);
        return head;
    }

    fclose(file);

    line[strcspn(line, "\n")] = 0;

    char* ptr = line;
    int coeff, exp;
    int numBytes;

    while (sscanf(ptr, "%d %d %n", &coeff, &exp, &numBytes) == 2) {
        insertSorted(head, coeff, exp);
        ptr += numBytes;
    }

    return head;
}

position printList(position head) {
    position temp = head->next;
    int first = 1;

    while (temp != NULL) {
        if (!first) {
            printf(" + ");
        }

        if (temp->exp == 0) {
            printf("%d", temp->coeff);
        }
        else if (temp->exp == 1) {
            if (temp->coeff == 1) {
                printf("x");
            }
            else {
                printf("%dx", temp->coeff);
            }
        }
        else {
            if (temp->coeff == 1) {
                printf("x^%d", temp->exp);
            }
            else {
                printf("%dx^%d", temp->coeff, temp->exp);
            }
        }

        temp = temp->next;
        first = 0;
    }

    if (first) {
        printf("0");
    }
    printf("\n");
    return head;
}

position addPolynomials(position headA, position headB, position result) {
    position tempA = headA->next;
    position tempB = headB->next;

    while (tempA != NULL && tempB != NULL) {
        if (tempA->exp == tempB->exp) {
            int sum = tempA->coeff + tempB->coeff;
            insertSorted(result, sum, tempA->exp);
            tempA = tempA->next;
            tempB = tempB->next;
        }
        else if (tempA->exp > tempB->exp) {
            insertSorted(result, tempA->coeff, tempA->exp);
            tempA = tempA->next;
        }
        else {
            insertSorted(result, tempB->coeff, tempB->exp);
            tempB = tempB->next;
        }
    }

    while (tempA != NULL) {
        insertSorted(result, tempA->coeff, tempA->exp);
        tempA = tempA->next;
    }

    while (tempB != NULL) {
        insertSorted(result, tempB->coeff, tempB->exp);
        tempB = tempB->next;
    }

    return result;
}

position multiplyPolynomials(position headA, position headB, position result) {
    position tempA = headA->next;

    while (tempA != NULL) {
        position tempB = headB->next;
        while (tempB != NULL) {
            int productCoeff = tempA->coeff * tempB->coeff;
            int productExp = tempA->exp + tempB->exp;
            insertSorted(result, productCoeff, productExp);
            tempB = tempB->next;
        }
        tempA = tempA->next;
    }

    return result;
}

position freeList(position head) {
    position temp = head->next;
    while (temp != NULL) {
        position toDelete = temp;
        temp = temp->next;
        toDelete->next = NULL;
        free(toDelete);
    }
    head->next = NULL;
    return head;
}