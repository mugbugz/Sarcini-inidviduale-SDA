#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

// Inițializare tablou cu valori aleatorii
int* initArray(int n) {
    int* arr = (int*)malloc(n * sizeof(int));
    for(int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
    }
    return arr;
}

// Afișare tablou
void printArray(int* arr, int n) {
    for(int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Eliberare memorie
void freeArray(int* arr) {
    free(arr);
}

// Bubble Sort
void bubbleSort(int* arr, int n) {
    clock_t start = clock();
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
    clock_t end = clock();
    printf("Bubble Sort executat în %.5f secunde\n", (double)(end-start)/CLOCKS_PER_SEC);
}

// Quick Sort
int partition(int* arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for(int j = low; j < high; j++) {
        if(arr[j] < pivot) {
            i++;
            int temp = arr[i]; arr[i] = arr[j]; arr[j] = temp;
        }
    }
    int temp = arr[i+1]; arr[i+1] = arr[high]; arr[high] = temp;
    return i+1;
}

void quickSort(int* arr, int low, int high) {
    if(low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi-1);
        quickSort(arr, pi+1, high);
    }
}

void quickSortWrapper(int* arr, int n) {
    clock_t start = clock();
    quickSort(arr, 0, n-1);
    clock_t end = clock();
    printf("Quick Sort executat în %.5f secunde\n", (double)(end-start)/CLOCKS_PER_SEC);
}

// Linear Search
void linearSearch(int* arr, int n, int key) {
    clock_t start = clock();
    for(int i = 0; i < n; i++) {
        if(arr[i] == key) {
            clock_t end = clock();
            printf("Linear Search găsit la index %d în %.5f secunde\n", i, (double)(end-start)/CLOCKS_PER_SEC);
            return;
        }
    }
    clock_t end = clock();
    printf("Linear Search nu a găsit elementul în %.5f secunde\n", (double)(end-start)/CLOCKS_PER_SEC);
}

// Binary Search (tablou trebuie sortat)
int binarySearch(int* arr, int n, int key) {
    clock_t start = clock();
    int left = 0, right = n-1;
    while(left <= right) {
        int mid = left + (right-left)/2;
        if(arr[mid] == key) {
            clock_t end = clock();
            printf("Binary Search găsit la index %d în %.5f secunde\n", mid, (double)(end-start)/CLOCKS_PER_SEC);
            return mid;
        }
        if(arr[mid] < key) left = mid+1;
        else right = mid-1;
    }
    clock_t end = clock();
    printf("Binary Search nu a găsit elementul în %.5f secunde\n", (double)(end-start)/CLOCKS_PER_SEC);
    return -1;
}

// Structură pentru numere mari (unsigned long long pentru simplitate)
typedef unsigned long long ULL;

void multiply(ULL F[2][2], ULL M[2][2]) {
    ULL x = F[0][0]*M[0][0] + F[0][1]*M[1][0];
    ULL y = F[0][0]*M[0][1] + F[0][1]*M[1][1];
    ULL z = F[1][0]*M[0][0] + F[1][1]*M[1][0];
    ULL w = F[1][0]*M[0][1] + F[1][1]*M[1][1];
    F[0][0] = x; F[0][1] = y; F[1][0] = z; F[1][1] = w;
}

void power(ULL F[2][2], int n) {
    if(n == 0 || n == 1) return;
    ULL M[2][2] = {{1,1},{1,0}};
    power(F, n/2);
    multiply(F,F);
    if(n%2 != 0) multiply(F,M);
}

ULL fib(int n) {
    if(n == 0) return 0;
    ULL F[2][2] = {{1,1},{1,0}};
    power(F, n-1);
    return F[0][0];
}

int main() {
    srand(time(NULL));
    int n = 10;
    int* arr = NULL;
    int choice, key;

    do {
        printf("\n--- MENIU ---\n");
        printf("1. Initializare tablou\n");
        printf("2. Afisare tablou\n");
        printf("3. Eliberare memorie\n");
        printf("4. Sortare Bubble Sort\n");
        printf("5. Sortare Quick Sort\n");
        printf("6. Cautare Linear Search\n");
        printf("7. Cautare Binary Search\n");
        printf("8. Fibonacci\n");
        printf("0. Iesire\n");
        printf("Alege: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                printf("Dimensiune tablou: "); scanf("%d",&n);
                arr = initArray(n);
                break;
            case 2:
                if(arr) printArray(arr,n);
                else printf("Tablou neinitializat!\n");
                break;
            case 3:
                if(arr) { freeArray(arr); arr=NULL; }
                break;
            case 4:
                if(arr) bubbleSort(arr,n);
                break;
            case 5:
                if(arr) quickSortWrapper(arr,n);
                break;
            case 6:
                if(arr) { printf("Numar de cautat: "); scanf("%d",&key); linearSearch(arr,n,key); }
                break;
            case 7:
                if(arr) { printf("Numar de cautat: "); scanf("%d",&key); quickSortWrapper(arr,n); binarySearch(arr,n,key); }
                break;
            case 8:
                printf("Pozitia N: "); scanf("%d",&key);
                clock_t start = clock();
                ULL result = fib(key);
                clock_t end = clock();
                printf("Fib(%d) = %llu calculat în %.5f secunde\n", key, result, (double)(end-start)/CLOCKS_PER_SEC);
                break;
        }
    } while(choice != 0);

    return 0;
}
