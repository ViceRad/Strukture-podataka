#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definiranje strukture osobe s typedef za pokazivac
typedef struct _person* position;
typedef struct _person {
    char fname[50];
    char lname[50];
    int birth_year;
    position next;
} person;

// Deklaracije funkcija
position createPerson(char* fname, char* lname, int birth_year);
position addToBeginning(position head, char* fname, char* lname, int birth_year);
int printList(position head);
position addToEnd(position head, char* fname, char* lname, int birth_year);
position findByLastName(position head, char* lname);
position deleteElement(position head, char* lname);
position deleteList(position head);

// Glavna funkcija programa
int main() {
    position head = NULL;

    head = addToEnd(head, "Ana", "Anic", 1990);
    head = addToBeginning(head, "Ivan", "Ivic", 1985);
    head = addToEnd(head, "Marko", "Markic", 1992);

    printf("Lista nakon dodavanja:\n");
    printList(head);

    printf("\nTrazenje osobe s prezimenom 'Anic':\n");
    position found = findByLastName(head, "Anic");
    if (found != NULL) {
        printf("Pronadjeno: %s %s\n", found->fname, found->lname);
    }
    else {
        printf("Osoba nije pronadjena.\n");
    }

    printf("\nBrisanje osobe s prezimenom 'Ivic':\n");
    head = deleteElement(head, "Ivic");
    printList(head);

    printf("\nBrisanje cijele liste:\n");
    head = deleteList(head);
    printList(head);

    return 0;
}

position createPerson(char* fname, char* lname, int birth_year) {
    position newPerson = (position)malloc(sizeof(person));
    if (newPerson == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        return NULL;
    }
    strcpy(newPerson->fname, fname);
    strcpy(newPerson->lname, lname);
    newPerson->birth_year = birth_year;
    newPerson->next = NULL;
    return newPerson;
}

// A. Funkcija za dodavanje novog elementa na pocetak liste
position addToBeginning(position head, char* fname, char* lname, int birth_year) {
    position newPerson = createPerson(fname, lname, birth_year);
    if (newPerson == NULL) {
        return head;
    }

    newPerson->next = head;
    return newPerson;
}

// B. Funkcija za ispis svih elemenata liste
int printList(position head) {
    position current = head;

    if (current == NULL) {
        printf("Lista je prazna.\n");
        return 0; // Vracamo 0 jer je ocekivano stanje (nije greska)
    }

    while (current != NULL) {
        printf("%s %s (%d)\n", current->fname, current->lname, current->birth_year);
        current = current->next;
    }
}

// C. Funkcija za dodavanje novog elementa na kraj liste
position addToEnd(position head, char* fname, char* lname, int birth_year) {
    position newPerson = createPerson(fname, lname, birth_year);
    if (newPerson == NULL) {
        return head;
    }

    if (head == NULL) {
        return newPerson;
    }

    position current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = newPerson;
    return head;
}

// D. Funkcija za pronalazenje elementa u listi po prezimenu
position findByLastName(position head, char* lname) {
    position current = head;

    while (current != NULL) {
        if (strcmp(current->lname, lname) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

// E. Funkcija za brisanje odredjenog elementa iz liste
position deleteElement(position head, char* lname) {
    if (head == NULL) {
        return NULL;
    }

    position current = head;
    position previous = NULL;

    while (current != NULL && strcmp(current->lname, lname) != 0) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        return head;
    }

    if (previous == NULL) {
        position newHead = head->next;
        free(head);
        return newHead;
    }
    else {
        previous->next = current->next;
        free(current);
        return head;
    }
}

// Funkcija za brisanje cijele liste (oslobadjanje memorije)
position deleteList(position head) {

    position current = head;
    while (current != NULL) {
        position temp = current;
        current = current->next;
        free(temp);
    }

    return NULL;
}