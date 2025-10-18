#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_BR_BODOVA 50
#define MAX_IME 50
#define MAX_PREZIME 50

typedef struct {
    char ime[MAX_IME];
    char prezime[MAX_PREZIME];
    int bodovi;
} Student;

int main() {
    // KORAK 1: Brojanje redaka u datoteci
    FILE* datoteka = NULL;

    if (fopen_s(&datoteka, "Vjezba1.txt", "r") != 0 || datoteka == NULL) {
        printf("Greska pri otvaranju datoteke 'studenti.txt'.\n");
        printf("Provjerite da li datoteka postoji u istom folderu kao .exe file.\n");
        return 1;
    }

    int brojStudenata = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), datoteka)) {
        brojStudenata++;
    }
    fclose(datoteka);

    printf("Pronadjeno %d studenata u datoteci.\n\n", brojStudenata);

    // KORAK 2: Alokacija memorije
    Student* studenti = (Student*)malloc(brojStudenata * sizeof(Student));
    if (!studenti) {
        printf("Greska pri alokaciji memorije.\n");
        return 1;
    }

    // KORAK 3: Ponovno otvaranje datoteke za ucitavanje podataka
    if (fopen_s(&datoteka, "Vjezba1.txt", "r") != 0 || datoteka == NULL) {
        printf("Greska pri ponovnom otvaranju datoteke.\n");
        free(studenti);
        return 1;
    }

    // Ucitavanje podataka
    int ucitaniStudenti = 0;
    for (int i = 0; i < brojStudenata; i++) {
        if (fscanf_s(datoteka, "%49s %49s %d",
            studenti[i].ime, (unsigned)sizeof(studenti[i].ime),
            studenti[i].prezime, (unsigned)sizeof(studenti[i].prezime),
            &studenti[i].bodovi) == 3) {
            ucitaniStudenti++;
        }
        else {
            printf("Greska pri citanju podataka za studenta %d\n", i + 1);
            break;
        }
    }
    fclose(datoteka);

    // KORAK 4: Ispis rezultata
    printf("Ime\t\tPrezime\t\tBodovi\tRelativni\n");

    for (int i = 0; i < ucitaniStudenti; i++) {
        float relativni = (float)studenti[i].bodovi / MAX_BR_BODOVA * 100;
        printf("%-10s\t%-10s\t%d\t%.2f%%\n",
            studenti[i].ime,
            studenti[i].prezime,
            studenti[i].bodovi,
            relativni);
    }

    // KORAK 5: Oslobadjanje memorije
    free(studenti);

    // Cekanje na korisnicki unos prije zatvaranja
    printf("\nProgram uspjesno zavrsio. Pritisnite Enter za izlaz...");
    while (getchar() != '\n');
    getchar();

    return 0;
}