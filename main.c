#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Document {
    int id;
    char name[50];
    int totalRows;
    int remainingRows;
    struct Document* next;
} Document;

typedef struct Printer {
    int id;
    int timePerRow;
    Document* currentDoc;
    struct Printer* next;
} Printer;

Document* queueHead = NULL;
Printer* printersHead = NULL;

// Adaugă document în coadă
void addDocument(int id, const char* name, int rows) {
    Document* newDoc = (Document*)malloc(sizeof(Document));
    newDoc->id = id;
    strcpy(newDoc->name, name);
    newDoc->totalRows = rows;
    newDoc->remainingRows = rows;
    newDoc->next = NULL;

    if (!queueHead) {
        queueHead = newDoc;
    } else {
        Document* temp = queueHead;
        while (temp->next) temp = temp->next;
        temp->next = newDoc;
    }
}

// Inițializează imprimantele
void initPrinters(int n, int times[]) {
    for (int i = 0; i < n; i++) {
        Printer* newPrinter = (Printer*)malloc(sizeof(Printer));
        newPrinter->id = i + 1;
        newPrinter->timePerRow = times[i];
        newPrinter->currentDoc = NULL;
        newPrinter->next = printersHead;
        printersHead = newPrinter;
    }
}

// Distribuie documentele către imprimante libere
void distributeDocuments() {
    Printer* p = printersHead;
    while (p) {
        if (!p->currentDoc && queueHead) {
            p->currentDoc = queueHead;
            queueHead = queueHead->next;
            p->currentDoc->next = NULL;
            printf("Document %s atribuit imprimantei %d\n", p->currentDoc->name, p->id);
        }
        p = p->next;
    }
}

// Simulează tipărirea
void simulateStep() {
    Printer* p = printersHead;
    while (p) {
        if (p->currentDoc) {
            p->currentDoc->remainingRows--;
            printf("Imprimanta %d tipareste documentul %s, randuri ramase: %d\n",
                   p->id, p->currentDoc->name, p->currentDoc->remainingRows);

            if (p->currentDoc->remainingRows <= 0) {
                printf("Document %s finalizat de imprimanta %d\n", p->currentDoc->name, p->id);
                free(p->currentDoc);
                p->currentDoc = NULL;
            }
        }
        p = p->next;
    }
    distributeDocuments();
}

// Anulează document din coadă
void cancelDocument(int id) {
    Document* prev = NULL;
    Document* curr = queueHead;
    while (curr) {
        if (curr->id == id) {
            if (prev) prev->next = curr->next;
            else queueHead = curr->next;
            free(curr);
            printf("Document %d anulat din coada.\n", id);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    // verifică imprimantele
    Printer* p = printersHead;
    while (p) {
        if (p->currentDoc && p->currentDoc->id == id) {
            free(p->currentDoc);
            p->currentDoc = NULL;
            printf("Document %d anulat în curs de tiparire.\n", id);
            return;
        }
        p = p->next;
    }
    printf("Document %d nu a fost gasit.\n", id);
}

// Adaugă imprimantă
void addPrinter(int id, int timePerRow) {
    Printer* newPrinter = (Printer*)malloc(sizeof(Printer));
    newPrinter->id = id;
    newPrinter->timePerRow = timePerRow;
    newPrinter->currentDoc = NULL;
    newPrinter->next = printersHead;
    printersHead = newPrinter;
    printf("Imprimanta %d adaugata.\n", id);
}

// Scoate imprimantă
void removePrinter(int id) {
    Printer* prev = NULL;
    Printer* curr = printersHead;
    while (curr) {
        if (curr->id == id) {
            if (prev) prev->next = curr->next;
            else printersHead = curr->next;
            if (curr->currentDoc) {
                addDocument(curr->currentDoc->id, curr->currentDoc->name, curr->currentDoc->remainingRows);
                free(curr->currentDoc);
            }
            free(curr);
            printf("Imprimanta %d scoasa din functiune.\n", id);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("Imprimanta %d nu a fost gasita.\n", id);
}

// Eliberare memorie
void freeAll() {
    while (queueHead) {
        Document* temp = queueHead;
        queueHead = queueHead->next;
        free(temp);
    }
    while (printersHead) {
        Printer* temp = printersHead;
        printersHead = printersHead->next;
        if (temp->currentDoc) free(temp->currentDoc);
        free(temp);
    }
}

int main() {
    int times[] = {2, 3}; // timpi de tipărire pentru 2 imprimante
    initPrinters(2, times);

    addDocument(1, "Raport", 5);
    addDocument(2, "Contract", 3);
    addDocument(3, "Manual", 4);

    distributeDocuments();

    for (int i = 0; i < 10; i++) {
        printf("\n--- Pas %d ---\n", i + 1);
        simulateStep();
    }

    cancelDocument(2);
    addPrinter(3, 1);
    removePrinter(1);

    freeAll();
    return 0;
}
