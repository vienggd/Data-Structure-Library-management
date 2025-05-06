#ifndef ALGORITHM_H
#define ALGORITHM_H

// Constants for sorting modes
#define SORT_TITLE 0          // Sort books by title
#define SORT_AUTHOR 1         // Sort books by author
#define SORT_AVAILABILITY 2   // Sort books by availability (available/unavailable)

// Sorts the global book list based on the specified sort mode
// Modes: SORT_TITLE, SORT_AUTHOR, SORT_AVAILABILITY
void sort_book_list(int sort_mode);

#endif
