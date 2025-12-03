#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 100
#define MAX_FILENAME 50

typedef struct ArticleStruct* ArticlePtr;
typedef struct ReceiptStruct* ReceiptPtr;

typedef struct ArticleStruct {
    char name[MAX_NAME];
    int quantity;
    double price;
    ArticlePtr next;
} Article;

typedef struct ReceiptStruct {
    char date[11];
    char filename[MAX_FILENAME];
    ArticlePtr articles;
    ReceiptPtr next;
} Receipt;

ArticlePtr createArticle(const char* name, int quantity, double price) {
    ArticlePtr new_article = (ArticlePtr)malloc(sizeof(Article));
    if (new_article == NULL) return NULL;
    strcpy(new_article->name, name);
    new_article->quantity = quantity;
    new_article->price = price;
    new_article->next = NULL;
    return new_article;
}

ReceiptPtr createReceipt(const char* date, const char* filename) {
    ReceiptPtr new_receipt = (ReceiptPtr)malloc(sizeof(Receipt));
    if (new_receipt == NULL) return NULL;
    strcpy(new_receipt->date, date);
    strcpy(new_receipt->filename, filename);
    new_receipt->articles = NULL;
    new_receipt->next = NULL;
    return new_receipt;
}

ArticlePtr addArticleSorted(ArticlePtr head, ArticlePtr new_article) {
    ArticlePtr current = head;
    while (current->next != NULL && strcmp(new_article->name, current->next->name) >= 0) {
        current = current->next;
    }
    new_article->next = current->next;
    current->next = new_article;
    return head;
}

ReceiptPtr addReceiptSorted(ReceiptPtr head, ReceiptPtr new_receipt) {
    ReceiptPtr current = head;
    while (current->next != NULL && strcmp(new_receipt->date, current->next->date) >= 0) {
        current = current->next;
    }
    new_receipt->next = current->next;
    current->next = new_receipt;
    return head;
}

ArticlePtr loadArticlesFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) return NULL;

    ArticlePtr head = (ArticlePtr)malloc(sizeof(Article));
    head->next = NULL;

    char line[256];
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        free(head);
        return NULL;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = 0;
        char name[MAX_NAME];
        int quantity;
        double price;

        char* token = strtok(line, ",");
        if (token == NULL) continue;
        strcpy(name, token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        quantity = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        price = atof(token);

        ArticlePtr new_article = createArticle(name, quantity, price);
        if (new_article != NULL) {
            head = addArticleSorted(head, new_article);
        }
    }

    fclose(file);
    return head;
}

char* loadDateFromFile(const char* filename, char* date) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) return NULL;
    if (fgets(date, 11, file) != NULL) {
        date[strcspn(date, "\n")] = 0;
        fclose(file);
        return date;
    }
    fclose(file);
    return NULL;
}

ReceiptPtr loadAllReceipts(const char* receipt_list_filename) {
    FILE* file = fopen(receipt_list_filename, "r");
    if (file == NULL) return NULL;

    ReceiptPtr head = (ReceiptPtr)malloc(sizeof(Receipt));
    head->next = NULL;

    char receipt_filename[MAX_FILENAME];
    while (fscanf(file, "%s", receipt_filename) == 1) {
        char date[11];
        if (loadDateFromFile(receipt_filename, date) == NULL) continue;
        ReceiptPtr new_receipt = createReceipt(date, receipt_filename);
        if (new_receipt == NULL) continue;
        new_receipt->articles = loadArticlesFromFile(receipt_filename);
        head = addReceiptSorted(head, new_receipt);
    }

    fclose(file);
    return head;
}

double calculateSpendingForArticle(ReceiptPtr head, const char* article_name,
    const char* date_from, const char* date_to,
    int* total_quantity) {
    double total_spending = 0.0;
    *total_quantity = 0;
    ReceiptPtr receipt = head->next;
    while (receipt != NULL) {
        if (strcmp(receipt->date, date_from) >= 0 && strcmp(receipt->date, date_to) <= 0) {
            ArticlePtr article = receipt->articles->next;
            while (article != NULL) {
                if (strcmp(article->name, article_name) == 0) {
                    total_spending += article->quantity * article->price;
                    *total_quantity += article->quantity;
                    break;
                }
                article = article->next;
            }
        }
        receipt = receipt->next;
    }
    return total_spending;
}

int calculateMonthlyExpenditure(ReceiptPtr head, int year) {
    double monthly_total[12] = { 0 };
    int months_with_data = 0;
    ReceiptPtr receipt = head->next;
    while (receipt != NULL) {
        int receipt_year, receipt_month;
        if (sscanf(receipt->date, "%d-%d", &receipt_year, &receipt_month) == 2) {
            if (receipt_year == year && receipt_month >= 1 && receipt_month <= 12) {
                ArticlePtr article = receipt->articles->next;
                while (article != NULL) {
                    monthly_total[receipt_month - 1] += article->quantity * article->price;
                    article = article->next;
                }
            }
        }
        receipt = receipt->next;
    }

    printf("Potrosnja po mjesecima za %d. godinu:\n", year);
    char* months[] = { "Sijecanj", "Veljaca", "Ozujak", "Travanj",
                     "Svibanj", "Lipanj", "Srpanj", "Kolovoz",
                     "Rujan", "Listopad", "Studeni", "Prosinac" };
    for (int i = 0; i < 12; i++) {
        if (monthly_total[i] > 0) {
            printf("%s: %.2f\n", months[i], monthly_total[i]);
            months_with_data++;
        }
    }
    return months_with_data;
}

int findReceiptsAboveAmount(ReceiptPtr head, double min_amount,
    const char* date_from, const char* date_to) {
    int found_count = 0;
    ReceiptPtr receipt = head->next;
    while (receipt != NULL) {
        if (strcmp(receipt->date, date_from) >= 0 && strcmp(receipt->date, date_to) <= 0) {
            double receipt_total = 0;
            ArticlePtr article = receipt->articles->next;
            while (article != NULL) {
                receipt_total += article->quantity * article->price;
                article = article->next;
            }
            if (receipt_total > min_amount) {
                if (found_count == 0) {
                    printf("Racuni iznad %.2f:\n", min_amount);
                }
                printf("Datum: %s, Datoteka: %s, Ukupno: %.2f\n",
                    receipt->date, receipt->filename, receipt_total);
                found_count++;
            }
        }
        receipt = receipt->next;
    }
    if (found_count == 0) {
        printf("Nema racuna iznad %.2f u navedenom periodu.\n", min_amount);
    }
    return found_count;
}

ArticlePtr freeArticles(ArticlePtr head) {
    ArticlePtr current = head->next;
    while (current != NULL) {
        ArticlePtr temp = current;
        current = current->next;
        free(temp);
    }
    free(head);
    return NULL;
}

ReceiptPtr freeReceipts(ReceiptPtr head) {
    ReceiptPtr current = head->next;
    while (current != NULL) {
        ReceiptPtr temp = current;
        current->articles = freeArticles(current->articles);
        current = current->next;
        free(temp);
    }
    free(head);
    return NULL;
}

int displayMenu() {
    printf("\n=== MENI ===\n");
    printf("1. Prikazi sve racune\n");
    printf("2. Upit za potrosnju na artikl\n");
    printf("3. Ukupna potrosnja po mjesecima\n");
    printf("4. Pretraga racuna iznad odredene vrijednosti\n");
    printf("5. Izlaz\n");
    printf("Izbor: ");
    return 1;
}

int displayReceipts(ReceiptPtr head) {
    printf("\n=== SVI RACUNI (sortirani po datumu) ===\n");
    ReceiptPtr receipt = head->next;
    int counter = 0;
    while (receipt != NULL) {
        printf("\n%d. Racun: %s (Datum: %s)\n", ++counter,
            receipt->filename, receipt->date);
        printf("   Artikli (sortirani po nazivu):\n");
        ArticlePtr article = receipt->articles->next;
        while (article != NULL) {
            printf("   - %s: %d x %.2f\n",
                article->name, article->quantity, article->price);
            article = article->next;
        }
        receipt = receipt->next;
    }
    return counter;
}

int main() {
    ReceiptPtr head = loadAllReceipts("racuni.txt");
    if (head == NULL || head->next == NULL) {
        printf("Greska pri ucitavanju racuna ili nema racuna!\n");
        if (head != NULL) free(head);
        return 1;
    }

    int choice;
    char article_name[MAX_NAME];
    char date_from[11], date_to[11];

    do {
        displayMenu();
        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1:
            displayReceipts(head);
            break;
        case 2:
            printf("Unesite naziv artikla: ");
            fgets(article_name, sizeof(article_name), stdin);
            article_name[strcspn(article_name, "\n")] = 0;
            printf("Unesite datum od (YYYY-MM-DD): ");
            scanf("%s", date_from);
            printf("Unesite datum do (YYYY-MM-DD): ");
            scanf("%s", date_to);
            int total_quantity;
            double spending = calculateSpendingForArticle(head,
                article_name, date_from, date_to, &total_quantity);
            printf("\nArtikl: %s\n", article_name);
            printf("Ukupna kolicina: %d\n", total_quantity);
            printf("Ukupna potrosnja: %.2f\n", spending);
            break;
        case 3:
            printf("Unesite godinu za analizu: ");
            int year;
            scanf("%d", &year);
            calculateMonthlyExpenditure(head, year);
            break;
        case 4:
            printf("Unesite minimalni iznos racuna: ");
            double min_amount;
            scanf("%lf", &min_amount);
            printf("Unesite datum od (YYYY-MM-DD): ");
            scanf("%s", date_from);
            printf("Unesite datum do (YYYY-MM-DD): ");
            scanf("%s", date_to);
            findReceiptsAboveAmount(head, min_amount, date_from, date_to);
            break;
        case 5:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Nepoznat izbor!\n");
            break;
        }
    } while (choice != 5);

    head = freeReceipts(head);
    return 0;
}