#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct City {
    char name[100];
    int population;
    struct City* next;
    struct City* left;
    struct City* right;
} City;

typedef struct CountryList {
    char name[100];
    char filename[100];
    City* cityTree;
    struct CountryList* next;
} CountryList;

typedef struct CountryTree {
    char name[100];
    char filename[100];
    City* cityList;
    struct CountryTree* left;
    struct CountryTree* right;
} CountryTree;

City* createCity(const char* name, int population);
City* addCityToTree(City* root, City* newCity);
City* addCityToList(City* head, City* newCity);
City* inorderPrintTree(City* root);
City* printCityList(City* head);
City* freeCityTree(City* root);
City* freeCityList(City* head);

CountryList* createCountryList(const char* name, const char* filename);
CountryList* addCountryToList(CountryList* head, CountryList* newCountry);
CountryList* loadCountriesFromFile(const char* filename);
CountryList* loadCitiesForCountryList(CountryList* country);
CountryList* printCountriesAndCitiesList(CountryList* head);
CountryList* searchCitiesList(CountryList* head, const char* countryName, int minPopulation);
CountryList* freeCountryList(CountryList* head);

CountryTree* createCountryTree(const char* name, const char* filename);
CountryTree* addCountryToTree(CountryTree* root, CountryTree* newCountry);
CountryTree* loadCitiesForCountryTree(CountryTree* country);
CountryTree* inorderPrintCountryTree(CountryTree* root);
CountryTree* searchCitiesTree(CountryTree* root, const char* countryName, int minPopulation);
CountryTree* freeCountryTree(CountryTree* root);

int compareCities(const City* a, const City* b);

int main() {
    CountryList* countryList = NULL;
    CountryTree* countryTree = NULL;
    char inputFilename[] = "drzave.txt";
    int choice;
    char countryName[100];
    int minPopulation;
    CountryList* tempList;

    countryList = loadCountriesFromFile(inputFilename);
    if (countryList == NULL) {
        printf("Greska pri ucitavanju datoteke drzava.\n");
        return 1;
    }

    tempList = countryList;
    while (tempList != NULL) {
        loadCitiesForCountryList(tempList);
        tempList = tempList->next;
    }

    tempList = countryList;
    while (tempList != NULL) {
        CountryTree* newCountryTree = createCountryTree(tempList->name, tempList->filename);
        loadCitiesForCountryTree(newCountryTree);
        countryTree = addCountryToTree(countryTree, newCountryTree);
        tempList = tempList->next;
    }

    printf("Podaci su uspjesno ucitani.\n\n");

    do {
        printf("\n=== MENI ===\n");
        printf("1. Ispisi drzave i gradove (lista drzava)\n");
        printf("2. Ispisi drzave i gradove (stablo drzava)\n");
        printf("3. Pretrazi gradove po drzavi i broju stanovnika\n");
        printf("4. Izlaz\n");
        printf("Odabir: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1:
            printf("\n--- Drzave i gradovi (lista drzava) ---\n");
            printCountriesAndCitiesList(countryList);
            break;

        case 2:
            printf("\n--- Drzave i gradovi (stablo drzava) ---\n");
            inorderPrintCountryTree(countryTree);
            break;

        case 3:
            printf("Unesite naziv drzave: ");
            fgets(countryName, sizeof(countryName), stdin);
            countryName[strcspn(countryName, "\n")] = 0;

            printf("Unesite minimalni broj stanovnika: ");
            scanf("%d", &minPopulation);
            getchar();

            printf("\n--- Rezultati pretrage (iz liste) ---\n");
            searchCitiesList(countryList, countryName, minPopulation);

            printf("\n--- Rezultati pretrage (iz stabla) ---\n");
            searchCitiesTree(countryTree, countryName, minPopulation);
            break;

        case 4:
            printf("Izlaz iz programa.\n");
            break;

        default:
            printf("Neispravan odabir.\n");
        }
    } while (choice != 4);

    freeCountryList(countryList);
    freeCountryTree(countryTree);

    return 0;
}

City* createCity(const char* name, int population) {
    City* newCity = (City*)malloc(sizeof(City));
    strcpy(newCity->name, name);
    newCity->population = population;
    newCity->left = NULL;
    newCity->right = NULL;
    newCity->next = NULL;
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

City* addCityToList(City* head, City* newCity) {
    if (head == NULL) {
        return newCity;
    }

    City* current = head;
    City* previous = NULL;

    while (current != NULL && compareCities(newCity, current) > 0) {
        previous = current;
        current = current->next;
    }

    newCity->next = current;
    if (previous == NULL) {
        return newCity;
    }
    else {
        previous->next = newCity;
        return head;
    }
}

City* inorderPrintTree(City* root) {
    if (root != NULL) {
        inorderPrintTree(root->left);
        printf("  %s, %d stanovnika\n", root->name, root->population);
        inorderPrintTree(root->right);
    }
    return root;
}

City* printCityList(City* head) {
    City* current = head;
    while (current != NULL) {
        printf("  %s, %d stanovnika\n", current->name, current->population);
        current = current->next;
    }
    return head;
}

City* freeCityTree(City* root) {
    if (root != NULL) {
        freeCityTree(root->left);
        freeCityTree(root->right);
        free(root);
    }
    return NULL;
}

City* freeCityList(City* head) {
    City* current = head;
    City* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    return NULL;
}

CountryList* createCountryList(const char* name, const char* filename) {
    CountryList* newCountry = (CountryList*)malloc(sizeof(CountryList));
    strcpy(newCountry->name, name);
    strcpy(newCountry->filename, filename);
    newCountry->cityTree = NULL;
    newCountry->next = NULL;
    return newCountry;
}

CountryList* addCountryToList(CountryList* head, CountryList* newCountry) {
    if (head == NULL) {
        return newCountry;
    }

    CountryList* current = head;
    CountryList* previous = NULL;

    while (current != NULL && strcmp(newCountry->name, current->name) > 0) {
        previous = current;
        current = current->next;
    }

    if (previous == NULL) {
        newCountry->next = head;
        return newCountry;
    }
    else {
        previous->next = newCountry;
        newCountry->next = current;
        return head;
    }
}

CountryList* loadCountriesFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ne mogu otvoriti datoteku %s\n", filename);
        return NULL;
    }

    CountryList* head = NULL;
    char countryName[100];
    char cityFilename[100];

    while (fscanf(file, "%s %s", countryName, cityFilename) == 2) {
        CountryList* newCountry = createCountryList(countryName, cityFilename);
        head = addCountryToList(head, newCountry);
    }

    fclose(file);
    return head;
}

CountryList* loadCitiesForCountryList(CountryList* country) {
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

CountryList* printCountriesAndCitiesList(CountryList* head) {
    CountryList* current = head;
    while (current != NULL) {
        printf("\nDrzava: %s\n", current->name);
        printf("Gradovi:\n");
        inorderPrintTree(current->cityTree);
        current = current->next;
    }
    return head;
}

CountryList* searchCitiesList(CountryList* head, const char* countryName, int minPopulation) {
    CountryList* current = head;
    int found = 0;

    while (current != NULL) {
        if (strcmp(current->name, countryName) == 0) {
            found = 1;
            printf("Gradovi u drzavi %s s vise od %d stanovnika:\n", countryName, minPopulation);

            int cityCount = 0;

            struct Stack {
                City* city;
                struct Stack* next;
            } *stack = NULL;

            City* temp = current->cityTree;

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

            return current;
        }
        current = current->next;
    }

    if (!found) {
        printf("Drzava %s nije pronadena.\n", countryName);
    }

    return head;
}

CountryList* freeCountryList(CountryList* head) {
    CountryList* current = head;
    CountryList* next;
    while (current != NULL) {
        next = current->next;
        freeCityTree(current->cityTree);
        free(current);
        current = next;
    }
    return NULL;
}

CountryTree* createCountryTree(const char* name, const char* filename) {
    CountryTree* newCountry = (CountryTree*)malloc(sizeof(CountryTree));
    strcpy(newCountry->name, name);
    strcpy(newCountry->filename, filename);
    newCountry->cityList = NULL;
    newCountry->left = NULL;
    newCountry->right = NULL;
    return newCountry;
}

CountryTree* addCountryToTree(CountryTree* root, CountryTree* newCountry) {
    if (root == NULL) {
        return newCountry;
    }

    if (strcmp(newCountry->name, root->name) < 0) {
        root->left = addCountryToTree(root->left, newCountry);
    }
    else {
        root->right = addCountryToTree(root->right, newCountry);
    }

    return root;
}

CountryTree* loadCitiesForCountryTree(CountryTree* country) {
    FILE* file = fopen(country->filename, "r");
    if (file == NULL) {
        printf("Ne mogu otvoriti datoteku %s\n", country->filename);
        return country;
    }

    char cityName[100];
    int population;

    while (fscanf(file, "%[^,], %d\n", cityName, &population) == 2) {
        City* newCity = createCity(cityName, population);
        country->cityList = addCityToList(country->cityList, newCity);
    }

    fclose(file);
    return country;
}

CountryTree* inorderPrintCountryTree(CountryTree* root) {
    if (root != NULL) {
        inorderPrintCountryTree(root->left);
        printf("\nDrzava: %s\n", root->name);
        printf("Gradovi:\n");
        printCityList(root->cityList);
        inorderPrintCountryTree(root->right);
    }
    return root;
}

CountryTree* searchCitiesTree(CountryTree* root, const char* countryName, int minPopulation) {
    if (root == NULL) {
        printf("Drzava %s nije pronadena.\n", countryName);
        return NULL;
    }

    int cmp = strcmp(countryName, root->name);

    if (cmp == 0) {
        printf("Gradovi u drzavi %s s vise od %d stanovnika:\n", countryName, minPopulation);

        City* current = root->cityList;
        int cityCount = 0;

        while (current != NULL) {
            if (current->population > minPopulation) {
                printf("  %s, %d stanovnika\n", current->name, current->population);
                cityCount++;
            }
            current = current->next;
        }

        if (cityCount == 0) {
            printf("  Nema gradova s vise od %d stanovnika.\n", minPopulation);
        }

        return root;
    }
    else if (cmp < 0) {
        return searchCitiesTree(root->left, countryName, minPopulation);
    }
    else {
        return searchCitiesTree(root->right, countryName, minPopulation);
    }
}

CountryTree* freeCountryTree(CountryTree* root) {
    if (root != NULL) {
        freeCountryTree(root->left);
        freeCountryTree(root->right);
        freeCityList(root->cityList);
        free(root);
    }
    return NULL;
}