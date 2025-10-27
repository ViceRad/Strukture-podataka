#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Person* Position;
typedef struct Person {
    char name[50];
    char surname[50];
    int year;
    Position next;
} Person;

Position createPerson(char* name, char* surname, int year);
Position addBeginning(Position head, char* name, char* surname, int year);
Position addEnd(Position head, char* name, char* surname, int year);
int printList(Position head);
Position find(Position head, char* surname);
Position deleteElement(Position head, char* surname);
Position clear(Position head);
Position insertAfter(Position head, char* target, char* name, char* surname, int year);
Position insertBefore(Position head, char* target, char* name, char* surname, int year);
Position sort(Position head);
int save(Position head, char* filename);
Position load(Position head, char* filename);
int clearInput();
int inputData(char* name, char* surname, int* year);

int main() {
    Position head = NULL;
    int choice, result, year;
    char name[50], surname[50], target[50], filename[100];

    do {
        printf("\n1. Dodaj na pocetak\n2. Dodaj na kraj\n3. Ispisi\n4. Pronadi\n5. Obrisi element\n6. Obrisi sve\n7. Dodaj iza\n8. Dodaj ispred\n9. Sortiraj\n10. Spasi\n11. Ucitaj\n0. Izlaz\nOdabir: ");
        scanf("%d", &choice);
        clearInput();

        switch (choice) {
        case 1:
            if (inputData(name, surname, &year) == 0) {
                head = addBeginning(head, name, surname, year);
            }
            break;
        case 2:
            if (inputData(name, surname, &year) == 0) {
                head = addEnd(head, name, surname, year);
            }
            break;
        case 3:
            result = printList(head);
            if (result == -1) {
                printf("Lista je prazna.\n");
            }
            break;
        case 4:
            printf("Prezime: ");
            scanf("%49s", surname);
            clearInput();
            Position found = find(head, surname);
            if (found != NULL) {
                printf("Pronadjeno: %s %s (%d)\n", found->name, found->surname, found->year);
            }
            else {
                printf("Nije pronadjeno.\n");
            }
            break;
        case 5:
            printf("Obriši osobu s prezimenom: ");
            scanf("%49s", surname);
            clearInput();
            head = deleteElement(head, surname);
            break;
        case 6:
            head = clear(head);
            printf("Lista obrisana.\n");
            break;
        case 7:
            if (inputData(name, surname, &year) == 0) {
                printf("Dodaj nakon osobe s prezimenom: ");
                scanf("%49s", target);
                clearInput();
                head = insertAfter(head, target, name, surname, year);
            }
            break;
        case 8:
            if (inputData(name, surname, &year) == 0) {
                printf("Dodaj prije osobe s prezimenom: ");
                scanf("%49s", target);
                clearInput();
                head = insertBefore(head, target, name, surname, year);
            }
            break;
        case 9:
            head = sort(head);
            printf("Lista sortirana.\n");
            break;
        case 10:
            printf("Datoteka: ");
            scanf("%99s", filename);
            clearInput();
            result = save(head, filename);
            if (result == 0) {
                printf("Spremljeno.\n");
            }
            else {
                printf("Greska pri spremanju.\n");
            }
            break;
        case 11:
            printf("Datoteka: ");
            scanf("%99s", filename);
            clearInput();
            head = load(head, filename);
            break;
        case 0:
            printf("Izlaz.\n");
            break;
        default:
            printf("Neispravan odabir.\n");
        }
    } while (choice != 0);

    head = clear(head);
    return 0;
}

int clearInput() {
    int c, count = 0;
    while ((c = getchar()) != '\n' && c != EOF) {
        count++;
    }
    return count;
}

int inputData(char* name, char* surname, int* year) {
    printf("Ime: ");
    if (scanf("%49s", name) != 1) {
        return -1;
    }
    clearInput();
    printf("Prezime: ");
    if (scanf("%49s", surname) != 1) {
        return -1;
    }
    clearInput();
    printf("Godina: ");
    if (scanf("%d", year) != 1) {
        return -1;
    }
    clearInput();
    return 0;
}

Position createPerson(char* name, char* surname, int year) {
    Position p = (Position)malloc(sizeof(Person));
    if (p == NULL) {
        return NULL;
    }
    strcpy(p->name, name);
    strcpy(p->surname, surname);
    p->year = year;
    p->next = NULL;
    return p;
}

Position addBeginning(Position head, char* name, char* surname, int year) {
    Position p = createPerson(name, surname, year);
    if (p == NULL) {
        return head;
    }
    p->next = head;
    return p;
}

Position addEnd(Position head, char* name, char* surname, int year) {
    Position p = createPerson(name, surname, year);
    if (p == NULL) {
        return head;
    }
    if (head == NULL) {
        return p;
    }
    Position cur = head;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = p;
    return head;
}

int printList(Position head) {
    Position cur = head;
    int count = 0;
    while (cur != NULL) {
        printf("%s %s (%d)\n", cur->name, cur->surname, cur->year);
        cur = cur->next;
        count++;
    }
    if (count > 0) {
        return count;
    }
    else {
        return -1;
    }
}

Position find(Position head, char* surname) {
    Position cur = head;
    while (cur != NULL) {
        if (strcmp(cur->surname, surname) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

Position deleteElement(Position head, char* surname) {
    if (head == NULL) {
        return NULL;
    }
    Position cur = head;
    Position prev = NULL;
    while (cur != NULL && strcmp(cur->surname, surname) != 0) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == NULL) {
        return head;
    }
    if (prev == NULL) {
        Position newHead = head->next;
        free(head);
        return newHead;
    }
    else {
        prev->next = cur->next;
        free(cur);
        return head;
    }
}

Position clear(Position head) {
    Position cur = head;
    while (cur != NULL) {
        Position temp = cur;
        cur = cur->next;
        free(temp);
    }
    return NULL;
}

Position insertAfter(Position head, char* target, char* name, char* surname, int year) {
    Position t = find(head, target);
    if (t == NULL) {
        printf("Osoba s prezimenom '%s' nije pronadjena.\n", target);
        return head;
    }
    Position p = createPerson(name, surname, year);
    if (p == NULL) {
        return head;
    }
    p->next = t->next;
    t->next = p;
    return head;
}

Position insertBefore(Position head, char* target, char* name, char* surname, int year) {
    if (head == NULL) {
        printf("Lista je prazna.\n");
        return head;
    }
    if (strcmp(head->surname, target) == 0) {
        return addBeginning(head, name, surname, year);
    }
    Position cur = head;
    while (cur->next != NULL && strcmp(cur->next->surname, target) != 0) {
        cur = cur->next;
    }
    if (cur->next == NULL) {
        printf("Osoba s prezimenom '%s' nije pronadjena.\n", target);
        return head;
    }
    Position p = createPerson(name, surname, year);
    if (p == NULL) {
        return head;
    }
    p->next = cur->next;
    cur->next = p;
    return head;
}

Position sort(Position head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }
    int swapped;
    Position ptr;
    Position last = NULL;
    do {
        swapped = 0;
        ptr = head;
        while (ptr->next != last) {
            if (strcmp(ptr->surname, ptr->next->surname) > 0) {
                char tempName[50], tempSurname[50];
                int tempYear;
                strcpy(tempName, ptr->name);
                strcpy(tempSurname, ptr->surname);
                tempYear = ptr->year;
                strcpy(ptr->name, ptr->next->name);
                strcpy(ptr->surname, ptr->next->surname);
                ptr->year = ptr->next->year;
                strcpy(ptr->next->name, tempName);
                strcpy(ptr->next->surname, tempSurname);
                ptr->next->year = tempYear;
                swapped = 1;
            }
            ptr = ptr->next;
        }
        last = ptr;
    } while (swapped);
    return head;
}

int save(Position head, char* filename) {
    FILE* f = fopen(filename, "w");
    if (f == NULL) {
        return -1;
    }
    Position cur = head;
    while (cur != NULL) {
        fprintf(f, "%s %s %d\n", cur->name, cur->surname, cur->year);
        cur = cur->next;
    }
    fclose(f);
    return 0;
}

Position load(Position head, char* filename) {
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("Greska pri otvaranju datoteke.\n");
        return head;
    }
    head = clear(head);
    char name[50], surname[50];
    int year;
    while (fscanf(f, "%49s %49s %d", name, surname, &year) == 3) {
        head = addEnd(head, name, surname, year);
    }
    fclose(f);
    printf("Ucitano.\n");
    return head;
}