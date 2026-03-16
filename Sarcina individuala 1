#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 6000


typedef struct {
    int zi, luna, an;
    char nume[50];
    char categorie[50];
    char subcategorie[50];
    char tara[50];
    char oras[50];
    double pret;
    int cantitate;
    double venit;
} Vanzare;



typedef struct {
    int luna;
    double venit;
} VenitLunar;

typedef struct {
    char nume[50];
    double venit;
} Produs;

typedef struct {
    char tara[50];
    char oras[50];
    double venit;
} TaraOras;

typedef struct {
    char tara[50];
    char oras[50];
    double venitMax;
} TopTara;

int citesteCSV(const char *filename, Vanzare v[], int max_rows) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Eroare la deschiderea fisierului: %s\n", filename);
        return 0;
    }

    char linie[1024];
    int n = 0;

    // sari peste header
    if (!fgets(linie, sizeof(linie), f)) {
        fclose(f);
        return 0;
    }

    while (fgets(linie, sizeof(linie), f)) {
        // ignora linii goale
        if (linie[0] == '\n' || linie[0] == '\r' || linie[0] == '\0')
            continue;

        // daca CSV-ul foloseste ';' in loc de ',' -> convertim rapid
        for (char *p = linie; *p; p++) {
            if (*p == ';') *p = ',';
        }

        if (n >= max_rows) {
            printf("ATENTIE: S-au depasit %d randuri. Mareste MAX.\n", max_rows);
            break;
        }

        // initializeaza sigur (evita gunoi daca parse-ul esueaza)
        memset(&v[n], 0, sizeof(Vanzare));

        int scanned = sscanf(linie,
    "%d-%d-%d,%*d,%49[^,],%49[^,],%49[^,],%lf,%d,%49[^,],%49[^\r\n]",
    &v[n].an, &v[n].luna, &v[n].zi,
    v[n].nume,
    v[n].categorie,
    v[n].subcategorie,
    &v[n].pret,
    &v[n].cantitate,
    v[n].tara,
    v[n].oras
);

        if (scanned != 10) {
            // Afiseaza linia problematica (SUPER util)
            printf("Linie invalida (scanned=%d): %s\n", scanned, linie);
            continue; // sari peste linia stricata
        }

        v[n].venit = v[n].pret * v[n].cantitate;
        n++;
    }

    fclose(f);
    return n;
}


void venitPeLuni(Vanzare v[], int n) {
    double luni[13] = {0};

    for (int i = 0; i < n; i++)
        luni[v[i].luna] += v[i].venit;

    printf("\nVenit lunar:\n");
    for (int i = 1; i <= 12; i++)
        printf("Luna %d: %.2f\n", i, luni[i]);
}

void top5Produse(Vanzare v[], int n) {
    Produs p[MAX];
    int m = 0;

    for (int i = 0; i < n; i++) {
        int gasit = 0;
        for (int j = 0; j < m; j++) {
            if (strcmp(p[j].nume, v[i].nume) == 0) {
                p[j].venit += v[i].venit;
                gasit = 1;
                break;
            }
        }
        if (!gasit) {
            strcpy(p[m].nume, v[i].nume);
            p[m].venit = v[i].venit;
            m++;
        }
    }

    // sortare descrescatoare
    for (int i = 0; i < m - 1; i++)
        for (int j = i + 1; j < m; j++)
            if (p[i].venit < p[j].venit) {
                Produs aux = p[i];
                p[i] = p[j];
                p[j] = aux;
            }

    printf("\nTop 5 produse:\n");
    for (int i = 0; i < 5 && i < m; i++)
        printf("%s - %.2f\n", p[i].nume, p[i].venit);
}

void vanzariPeCategorii(Vanzare v[], int n) {
    Produs c[MAX];
    int m = 0;

    for (int i = 0; i < n; i++) {
        int gasit = 0;
        for (int j = 0; j < m; j++) {
            if (strcmp(c[j].nume, v[i].categorie) == 0) {
                c[j].venit += v[i].venit;
                gasit = 1;
                break;
            }
        }
        if (!gasit) {
            strcpy(c[m].nume, v[i].categorie);
            c[m].venit = v[i].venit;
            m++;
        }
    }

    printf("\nVanzari pe categorii:\n");
    for (int i = 0; i < m; i++)
        printf("%s: %.2f\n", c[i].nume, c[i].venit);
}

void topOrasePeTara(Vanzare v[], int n) {
    // Alocre dinamica
    TaraOras *tc = (TaraOras*)malloc(sizeof(TaraOras) * (size_t)n);
    TopTara  *top = (TopTara*)malloc(sizeof(TopTara) * (size_t)n);

    if (!tc || !top) {
        printf("Eroare: nu s-a putut aloca memorie.\n");
        free(tc);
        free(top);
        return;
    }

    int m = 0;

    // 1) Agregare venit pe (tara, oras)
    for (int i = 0; i < n; i++) {
        int gasit = 0;
        for (int j = 0; j < m; j++) {
            if (strcmp(tc[j].tara, v[i].tara) == 0 && strcmp(tc[j].oras, v[i].oras) == 0) {
                tc[j].venit += v[i].venit;
                gasit = 1;
                break;
            }
        }
        if (!gasit) {
            strcpy(tc[m].tara, v[i].tara);
            strcpy(tc[m].oras, v[i].oras);
            tc[m].venit = v[i].venit;
            m++;
        }
    }

    // 2) Top oras per tara
    int k = 0;
    for (int i = 0; i < m; i++) {
        int idx = -1;
        for (int j = 0; j < k; j++) {
            if (strcmp(top[j].tara, tc[i].tara) == 0) {
                idx = j;
                break;
            }
        }

        if (idx == -1) {
            strcpy(top[k].tara, tc[i].tara);
            strcpy(top[k].oras, tc[i].oras);
            top[k].venitMax = tc[i].venit;
            k++;
        } else if (tc[i].venit > top[idx].venitMax) {
            strcpy(top[idx].oras, tc[i].oras);
            top[idx].venitMax = tc[i].venit;
        }
    }

    printf("\nOrasul cu cel mai mare venit pentru fiecare tara:\n");
    for (int i = 0; i < k; i++) {
        printf("%s -> %s : %.2f\n", top[i].tara, top[i].oras, top[i].venitMax);
    }

    free(tc);
    free(top);
}

void tendinteSubcategorii(Vanzare v[], int n) {
    printf("\nTendinte lunare pe subcategorii:\n");

    for (int luna = 1; luna <= 12; luna++) {
        printf("\nLuna %d:\n", luna);
        for (int i = 0; i < n; i++)
            if (v[i].luna == luna)
                printf("%s: %.2f\n", v[i].subcategorie, v[i].venit);
    }
}

int main() {
    Vanzare *v = malloc(sizeof(Vanzare) * (size_t)MAX);
    int n = citesteCSV("C:/zyx/sales.csv", v, MAX);

    venitPeLuni(v, n);
    top5Produse(v, n);
    vanzariPeCategorii(v, n);
    topOrasePeTara(v, n);
    tendinteSubcategorii(v, n);
    free(v);
    return 0;
}
