#include "readsavefiles.h"
#include "algorithm.h"
#include <stdlib.h>
#include <string.h>

// Helper function to compare books based on sorting mode
int compare_books(struct booklist *a, struct booklist *b, int sort_mode) {
    switch(sort_mode) {
        case SORT_TITLE:
            return strcasecmp(a->title, b->title);
        case SORT_AUTHOR:
            return strcasecmp(a->author, b->author);
        case SORT_AVAILABILITY:
            return b->availability - a->availability;
        default:
            return 0;
    }
}

// Helper function to swap two book pointers
void swap_books(struct booklist **a, struct booklist **b) {
    struct booklist *temp = *a;
    *a = *b;
    *b = temp;
}

// Quicksort partition function
int partition(struct booklist **books, int low, int high, int sort_mode) {
    struct booklist *pivot = books[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (compare_books(books[j], pivot, sort_mode) < 0) {
            i++;
            swap_books(&books[i], &books[j]);
        }
    }
    swap_books(&books[i+1], &books[high]);
    return i + 1;
}

// Main quicksort function
void quicksort(struct booklist **books, int low, int high, int sort_mode) {
    if (low < high) {
        int pi = partition(books, low, high, sort_mode);
        quicksort(books, low, pi-1, sort_mode);
        quicksort(books, pi+1, high, sort_mode);
    }
}

// Convert linked list to array
struct booklist** list_to_array(int *count) {
    *count = 0;
    struct booklist *current = bookhead;
    
    // Count books
    while (current) {
        (*count)++;
        current = current->next;
    }
    
    // Create array
    struct booklist **arr = malloc(*count * sizeof(struct booklist*));
    current = bookhead;
    for(int i = 0; i < *count; i++) {
        arr[i] = current;
        current = current->next;
    }
    return arr;
}

// Rebuild linked list from array
void array_to_list(struct booklist **arr, int count) {
    if (count == 0) {
        bookhead = NULL;
        return;
    }
    
    bookhead = arr[0];
    for(int i = 0; i < count-1; i++) {
        arr[i]->next = arr[i+1];
    }
    arr[count-1]->next = NULL;
}

// Main sorting function
void sort_book_list(int sort_mode) {
    int count;
    struct booklist **arr = list_to_array(&count);
    
    if (count > 1) {
        quicksort(arr, 0, count-1, sort_mode);
        array_to_list(arr, count);
        savebookfile();
    }
    
    free(arr);
}
