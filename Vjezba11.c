#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct City {
    char name[100];
    int population;
    struct City* left;
    struct City* right;
} City;

typedef struct Country {
    char name[100];
    char filename[100];
    City* cityTree;
    struct Country* next;
} Country;

#define HASH_TABLE_SIZE 11
Country* hashTable[HASH_TABLE_SIZE];

City* createCity(const char* name, int population);
City* addCityToTree(City* root, City* newCity);
City* inorderPrintTree(City* root);
City* freeCityTree(City* root);
int compareCities(const City* a, const City* b);

int hashFunction(const char* countryName);
int initializeHashTable();
Country* createCountry(const char* name, const char* filename);
Country* insertCountryToHashTable(Country* country);
Country* findCountryInHashTable(const char* countryName);

Country* loadCountriesFromFile(const char* filename);
Country* loadCitiesForCountry(Country* country);

Country* printHashTable();
Country* printCountryAndCities(Country* country);
Country* searchCitiesInCountry(const char* countryName, int minPopulation);

Country* freeHashTable();

int main() {
    char inputFilename[] = "drzave.txt";
    int choice;
    char countryName[100];
    int minPopulation;

    initializeHashTable();

    loadCountriesFromFile(inputFilename);
    printf("Podaci su uspjesno ucitani.\n\n");

    do {
        printf("\n=== MENI ===\n");
        printf("1. Ispisi sve drzave i gradove\n");
        printf("2. Pretrazi gradove po drzavi i broju stanovnika\n");
        printf("3. Izlaz\n");
        printf("Odabir: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1:
            printf("\n--- Sve drzave i gradove ---\n");
            printHashTable();
            break;

        case 2:
            printf("Unesite naziv drzave: ");
            fgets(countryName, sizeof(countryName), stdin);
            countryName[strcspn(countryName, "\n")] = 0;

            printf("Unesite minimalni broj stanovnika: ");
            scanf("%d", &minPopulation);
            getchar();

            printf("\n--- Rezultati pretrage ---\n");
            searchCitiesInCountry(countryName, minPopulation);
            break;

        case 3:
            printf("Izlaz iz programa.\n");
            break;

        default:
            printf("Neispravan odabir.\n");
        }
    } while (choice != 3);

    freeHashTable();

    return 0;
}

City* createCity(const char* name, int population) {
    City* newCity = (City*)malloc(sizeof(City));
    strcpy(newCity->name, name);
    newCity->population = population;
    newCity->left = NULL;
    newCity->right = NULL;
    return newCity;
}

int compareCities(const City* a, const City* b) {
    if (a->population != b->population) {
        return b->population - a->population;
    }
    return strcmp(a->name, b->name);
}

City* addCityToTree(City* root, City* newCity) {
    if (root == NULL) {
        return newCity;
    }

    if (compareCities(newCity, root) < 0) {
        root->left = addCityToTree(root->left, newCity);
    }
    else {
        root->right = addCityToTree(root->right, newCity);
    }

    return root;
}

City* inorderPrintTree(City* root) {
    if (root != NULL) {
        inorderPrintTree(root->left);
        printf("  %s, %d stanovnika\n", root->name, root->population);
        inorderPrintTree(root->right);
    }
    return root;
}

City* freeCityTree(City* root) {
    if (root != NULL) {
        freeCityTree(root->left);
        freeCityTree(root->right);
        free(root);
    }
    return NULL;
}

int hashFunction(const char* countryName) {
    int sum = 0;
    int length = strlen(countryName);
    int charsToSum = (length < 5) ? length : 5;

    for (int i = 0; i < charsToSum; i++) {
        sum += (int)countryName[i];
    }

    return sum % HASH_TABLE_SIZE;
}

int initializeHashTable() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
    return 0;
}

Country* createCountry(const char* name, const char* filename) {
    Country* newCountry = (Country*)malloc(sizeof(Country));
    strcpy(newCountry->name, name);
    strcpy(newCountry->filename, filename);
    newCountry->cityTree = NULL;
    newCountry->next = NULL;
    return newCountry;
}

Country* insertCountryToHashTable(Country* country) {
    int index = hashFunction(country->name);

    if (hashTable[index] == NULL) {
        hashTable[index] = country;
    }
    else {
        Country* current = hashTable[index];
        Country* previous = NULL;

        while (current != NULL && strcmp(country->name, current->name) > 0) {
            previous = current;
            current = current->next;
        }

        if (previous == NULL) {
            country->next = hashTable[index];
            hashTable[index] = country;
        }
        else {
            country->next = current;
            previous->next = country;
        }
    }

    return country;
}

Country* findCountryInHashTable(const char* countryName) {
    int index = hashFunction(countryName);
    Country* current = hashTable[index];

    while (current != NULL) {
        if (strcmp(current->name, countryName) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

Country* loadCountriesFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ne mogu otvoriti datoteku %s\n", filename);
        return NULL;
    }

    char countryName[100];
    char cityFilename[100];

    while (fscanf(file, "%s %s", countryName, cityFilename) == 2) {
        Country* newCountry = createCountry(countryName, cityFilename);
        insertCountryToHashTable(newCountry);
        loadCitiesForCountry(newCountry);
    }

    fclose(file);
    return NULL;
}

Country* loadCitiesForCountry(Country* country) {
    FILE* file = fopen(country->filename, "r");
    if (file == NULL) {
        printf("Ne mogu otvoriti datoteku %s\n", country->filename);
        return country;
    }

    char cityName[100];
    int population;

    while (fscanf(file, "%[^,], %d\n", cityName, &population) == 2) {
        City* newCity = createCity(cityName, population);
        country->cityTree = addCityToTree(country->cityTree, newCity);
    }

    fclose(file);
    return country;
}

Country* printHashTable() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        printf("\n[Slot %d]:\n", i);
        Country* current = hashTable[i];

        if (current == NULL) {
            printf("  (prazno)\n");
        }
        else {
            while (current != NULL) {
                printCountryAndCities(current);
                current = current->next;
            }
        }
    }
    return NULL;
}

Country* printCountryAndCities(Country* country) {
    printf("\nDrzava: %s\n", country->name);
    printf("Gradovi:\n");
    inorderPrintTree(country->cityTree);
    return country;
}

Country* searchCitiesInCountry(const char* countryName, int minPopulation) {
    Country* country = findCountryInHashTable(countryName);

    if (country == NULL) {
        printf("Drzava %s nije pronadena.\n", countryName);
        return NULL;
    }

    printf("Gradovi u drzavi %s s vise od %d stanovnika:\n", countryName, minPopulation);

    int cityCount = 0;
    struct Stack {
        City* city;
        struct Stack* next;
    } *stack = NULL;

    City* temp = country->cityTree;

    while (temp != NULL || stack != NULL) {
        while (temp != NULL) {
            struct Stack* newStack = malloc(sizeof(struct Stack));
            newStack->city = temp;
            newStack->next = stack;
            stack = newStack;
            temp = temp->left;
        }

        temp = stack->city;
        struct Stack* toFree = stack;
        stack = stack->next;
        free(toFree);

        if (temp->population > minPopulation) {
            printf("  %s, %d stanovnika\n", temp->name, temp->population);
            cityCount++;
        }

        temp = temp->right;
    }

    if (cityCount == 0) {
        printf("  Nema gradova s vise od %d stanovnika.\n", minPopulation);
    }

    return country;
}

Country* freeHashTable() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Country* current = hashTable[i];
        while (current != NULL) {
            Country* next = current->next;
            freeCityTree(current->cityTree);
            free(current);
            current = next;
        }
        hashTable[i] = NULL;
    }
    return NULL;
}