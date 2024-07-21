#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//WICHTIG!!!!!!!!!!!!!!!!!
//Dieser File ist nur für Testing. Aus diesem File kann man examples/quick_sort.csv erstellen.
//Dieser File schreibt input-File (in .csv datei) für QuickSort mit bestimmter Größe.
int partition(int arr[], int low, int high, FILE *log) {
    int pivot = arr[high];
    //Da arr type integer, so Value von Integer ist 4 Byte aligned
    fprintf(log, "r %d\n", high*32);
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        fprintf(log, "r %d\n", j*32);
        if (arr[j] <= pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            fprintf(log, "w %d %d\n", i * 32, arr[i]);
            fprintf(log, "w %d %d\n", j * 32, arr[j]);
        }
    }
    //swap pivot element
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    //Swap elemente wieder
    fprintf(log, "w %d %d\n", (i + 1)* 32, arr[i + 1]);
    fprintf(log, "w %d %d\n", high * 32, arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high, FILE *log) {
    if (low < high) {
        int pi = partition(arr, low, high, log);
        quickSort(arr, low, pi - 1, log);
        quickSort(arr, pi + 1, high, log);
    }
}

int main() {
    srand(time(NULL));
    int SIZE = 10;
    int arr[SIZE];

    FILE *log = fopen("../examples/quick_sort.csv", "w");
    for (int i = 0; i < SIZE; i++) {
        arr[i] = rand() % SIZE + 1;
        fprintf(log, "w %d %d\n", i * 32, arr[i]);
    }
    quickSort(arr, 0, SIZE-1, log);

    fclose(log);

    return 0;
}
