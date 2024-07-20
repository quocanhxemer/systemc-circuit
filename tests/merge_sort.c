#include <stdio.h>
#include <stdlib.h>


void merge(int arr[], int l, int m, int r, FILE *log) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++) {
        fprintf(log, "r %d\n", (l + i)*32);
        L[i] = arr[l + i];
    }
    for (int j = 0; j < n2; j++) {
        fprintf(log, "r %d\n", (m + 1 + j)*32 );
        R[j] = arr[m + 1 + j];
    }

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            fprintf(log, "w %d %d\n", k*32, L[i]);
            arr[k] = L[i];
            i++;
        } else {
            fprintf(log, "w %d %d\n", k*32, R[j]);
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        fprintf(log, "w %d %d\n", k*32, L[i]);
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        fprintf(log, "w %d %d\n", k*32, R[j]);
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSort(int arr[], int l, int r, FILE *log) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m, log);
        mergeSort(arr, m + 1, r, log);
        merge(arr, l, m, r, log);
    }
}

int main() {
    int SIZE = 6;
    int arr[SIZE+5];

    FILE *log = fopen("../examples/merge_sort.csv", "w");
    for (int i = 0; i < SIZE; i++) {
        arr[i] = rand() % SIZE + 1;
        fprintf(log, "w %d %d\n", i*32, arr[i]);
    }
    mergeSort(arr, 0, SIZE-1, log);
    for (int i=0; i<SIZE;i++){
        fprintf(log, "r %d \n", i*32);
    }
    fclose(log);

    return 0;
}
