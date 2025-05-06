#include "readsavefiles.h" 
#include "user.h"
#include "algorithm.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "log.h"

#define MAX_INPUT 99

// Prints a header section with a title and the user's ID
// This helps to organize the output and show which user is logged in
void print_user_section_header(const char *title, int user_id) {
    // Clear screen (commented out, can be enabled if needed)
    // system("cls");
    printf("\n==============\n");
    printf("%s [ID: %d]\n", title, user_id);
    printf("==============\n\n");
}

// Converts a string to lowercase and removes all spaces
// This is useful for making searches case-insensitive and ignoring spaces
void normalize_string(char *str) {
    int write_index = 0;
    for(int i = 0; str[i]; i++) {
        if(str[i] != ' ') { // Skip spaces
            str[write_index++] = tolower(str[i]); // Convert to lowercase
        }
    }
    str[write_index] = '\0'; // Null-terminate the new string
}

// Prints a table of all books in the system with their details
// Shows number, title, author, ISBN, and availability status
void print_book_table() {
    system("cls"); // Clear the console screen for neatness
    printf("\n%-5s %-35s %-30s %-20s %-10s\n", 
         "NUM", "Title", "Author", "ISBN", "Available");
    printf("--------------------------------------------------------------------------------------------------------\n");

    struct booklist *current = bookhead;
    int counter = 1;
    while(current != NULL) {
        // Print each book's details in a formatted row
        printf("[%-2d] %-35s %-30s %-20s %-10s\n", 
            counter++,
            current->title,
            current->author,
            current->isbn,
            current->availability > 0 ? "Yes" : "No"); // Show Yes if available, No if not
        current = current->next;
    }
    printf("\nTotal books: %d\n", bookfilecount);
}

// Handles user actions on a selected queue entry (waiting list)
// Allows user to borrow the book if available, cancel queue, or return to previous menu
void handle_queue_selection(struct account *user, struct queuelist *selected, struct queuelist *prev, char *action_msg) {
    // Find the book in the book list that matches the queue entry's ISBN
    struct booklist *book = bookhead;
    while(book && strcmp(book->isbn, selected->isbn)) book = book->next;

    int choice;
    printf("\nSelected: %s\n", book ? book->title : "Unknown");
    printf("Status: %s\n", (book && book->availability > 0 && selected->queue == 1) ? 
                          "Ready to borrow" : "Waiting");
    
    if(book && book->availability > 0 && selected->queue == 1) {
        // If the book is available and user is first in queue, offer options
        printf("[0] Return\n[1] Borrow now\n[2] Cancel queue\n");
        printf("Select an option: ");
        scanf("%d", &choice);
        
        if(choice == 1) { // Borrow now
            int days;
            do {
                printf("Enter borrowing days (1-30): ");
                scanf("%d", &days);
            } while(days < 1 || days > 30);

            time_t now = time(NULL);
            struct tm *tm = localtime(&now);
            
            // Create a new borrow record for this user and book
            struct borrowlist *new_borrow = malloc(sizeof(struct borrowlist));
            new_borrow->userid = user->id;
            strcpy(new_borrow->isbn, selected->isbn);
            
            // Record borrow date and calculate return date
            strftime(new_borrow->borrowdate, CHARACTERINPUT, "%d/%m/%Y", tm);
            tm->tm_mday += days;
            mktime(tm);
            strftime(new_borrow->returndate, CHARACTERINPUT, "%d/%m/%Y", tm);
            
            // Add borrow record to the front of the borrow list
            new_borrow->next = borrowhead;
            borrowhead = new_borrow;
            book->availability--; // Decrease availability count
            
            // Remove user from the queue
            if(prev) prev->next = selected->next;
            else queuehead = selected->next;
            
            // Update queue positions for others waiting for the same book
            struct queuelist *q = queuehead;
            while(q) {
                if(strcmp(q->isbn, selected->isbn) == 0 && q->queue > selected->queue) {
                    q->queue--;
                }
                q = q->next;
            }
            
            free(selected); // Free memory for removed queue node
            
            // Save updated data to files
            savebookfile();
            saveborrowfile();
            savequeuefile();
            
            // Log the borrowing action
            char log_msg[CHARACTERINPUT];
            snprintf(log_msg, CHARACTERINPUT, "User %d borrowed from queue: %s", user->id, book->title);
            log_action(log_msg);
            
            strcpy(action_msg, "Book borrowed");
        }
        else if(choice == 2) { // Cancel queue
            // Remove user from queue and update positions
            if(prev) prev->next = selected->next;
            else queuehead = selected->next;
            
            struct queuelist *q = queuehead;
            while(q) {
                if(strcmp(q->isbn, selected->isbn) == 0 && q->queue > selected->queue) {
                    q->queue--;
                }
                q = q->next;
            }
            
            free(selected);
            savequeuefile();
            
            log_action("User cancelled queue");
            strcpy(action_msg, "Queue cancelled");
        }
    } else {
        // If book is not ready to borrow, only allow cancel queue or return
        printf("[0] Return\n[1] Cancel queue\n");
        printf("Select an option: ");
        scanf("%d", &choice);
        
        if(choice == 1) {
            if(prev) prev->next = selected->next;
            else queuehead = selected->next;
            
            struct queuelist *q = queuehead;
            while(q) {
                if(strcmp(q->isbn, selected->isbn) == 0 && q->queue > selected->queue) {
                    q->queue--;
                }
                q = q->next;
            }
            
            free(selected);
            savequeuefile();
            
            log_action("User cancelled queue");
            strcpy(action_msg, "Queue cancelled");
        }
    }
}

// Displays the user's current queue (waiting list) for books
// Allows selecting a queue entry to borrow or cancel
void view_queue_interface(struct account *user, char *action_msg) {
    int choice, count = 0;
    struct queuelist *current = queuehead;

    // Count how many queue entries belong to this user
    while(current) {
        if(current->userid == user->id) count++;
        current = current->next;
    }

    if(count == 0) {
        print_user_section_header("My Queue", user->id);
        printf("[Action Bar] : Your queue is empty!\n\n");
        printf("[0] Return: ");
        int ch;
        do { scanf("%d", &ch); } while(ch != 0);
        return;
    }

    do {
        print_user_section_header("My Queue", user->id);
        if(strlen(action_msg)) {
            printf("[Action Bar] : %s!\n\n", action_msg);
            action_msg[0] = '\0';
        }

        // Print table header for queue list
        printf("%-5s %-35s %-30s %-20s %-15s\n", 
            "NUM", "Title", "Author", "ISBN", "Status");
        printf("--------------------------------------------------------------------------------------------------------\n");

        int pos = 1;
        current = queuehead;
        while(current) {
            if(current->userid == user->id) {
                // Find corresponding book details
                struct booklist *book = bookhead;
                while(book && strcmp(book->isbn, current->isbn)) book = book->next;
                
                char status[20];
                if(book && book->availability > 0 && current->queue == 1) {
                    strcpy(status, "Ready to borrow");
                } else {
                    snprintf(status, 20, "Waiting (%d)", current->queue);
                }

                // Print each queue entry with status
                printf("[%-2d] %-35s %-30s %-20s %-15s\n", 
                    pos++, 
                    book ? book->title : "Unknown",
                    book ? book->author : "Unknown",
                    current->isbn,
                    status);
            }
            current = current->next;
        }

        printf("\n[0] Return\n[1-%d] Select queue\n", count);
        printf("Select an option: ");
        
        if(scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); // Clear invalid input
            strcpy(action_msg, "Invalid input");
            continue;
        }

        if(choice == 0) return;
        if(choice < 1 || choice > count) {
            strcpy(action_msg, "Invalid selection");
            continue;
        }

        // Find the selected queue entry based on user choice
        struct queuelist *selected = queuehead;
        struct queuelist *prev = NULL;
        int target = 1;
        while(selected) {
            if(selected->userid == user->id) {
                if(target == choice) break;
                target++;
            }
            prev = selected;
            selected = selected->next;
        }

        if(selected) {
            // Handle borrow or cancel queue for selected entry
            handle_queue_selection(user, selected, prev, action_msg);
            // Recount queue entries after possible changes
            count = 0;
            current = queuehead;
            while(current) {
                if(current->userid == user->id) count++;
                current = current->next;
            }
            if(count == 0) {
                strcpy(action_msg, "Queue is now empty");
                return;
            }
        }
    } while(1);
}

// Interface for students to borrow books
// Lists available books, allows selection, borrowing or queueing if unavailable
void borrow_books_interface(struct account *user, char *action_msg) {
    int choice, days;
    char confirm;
    
    do {
        print_user_section_header("Borrow Books", user->id);
        if(strlen(action_msg)) {
            printf("[Action Bar] : %s!\n\n", action_msg);
            action_msg[0] = '\0';
        }
        
        print_book_table();
        
        printf("\n[0] Cancel\n[1-%d] Select book\n", bookfilecount);
        printf("Select an option: ");
        
        if(scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            strcpy(action_msg, "Invalid input");
            continue;
        }

        if(choice == 0) return;

        // Find the selected book by index
        struct booklist *book = bookhead;
        for(int i = 1; i < choice; i++) {
            if(!book) break;
            book = book->next;
        }

        if(!book || choice < 1 || choice > bookfilecount) {
            strcpy(action_msg, "Invalid selection");
            continue;
        }

        printf("\nConfirm borrow '%s'? (Y/N): ", book->title);
        scanf(" %c", &confirm);
        if(toupper(confirm) != 'Y') {
            strcpy(action_msg, "Borrow cancelled");
            continue;
        }

        if(book->availability > 0) {
            // If book is available, ask for borrowing days and create borrow record
            do {
                printf("Enter borrowing days (1-30): ");
                if(scanf("%d", &days) != 1) {
                    while(getchar() != '\n');
                    strcpy(action_msg, "Invalid input");
                    break;
                }
            } while(days < 1 || days > 30);

            if(days < 1 || days > 30) continue;

            time_t now = time(NULL);
            struct tm *tm = localtime(&now);
            
            struct borrowlist *new_borrow = malloc(sizeof(struct borrowlist));
            new_borrow->userid = user->id;
            strcpy(new_borrow->isbn, book->isbn);
            
            strftime(new_borrow->borrowdate, CHARACTERINPUT, "%d/%m/%Y", tm);
            tm->tm_mday += days;
            mktime(tm);
            strftime(new_borrow->returndate, CHARACTERINPUT, "%d/%m/%Y", tm);
            
            new_borrow->next = borrowhead;
            borrowhead = new_borrow;
            book->availability--;
            
            savebookfile();
            saveborrowfile();
            
            char log_msg[CHARACTERINPUT];
            snprintf(log_msg, CHARACTERINPUT, "User %d borrowed: %s", user->id, book->title);
            log_action(log_msg);
            
            strcpy(action_msg, "Book borrowed successfully");
        } else {
            // If book not available, check if user already in queue
            struct queuelist *q = queuehead;
            while(q) {
                if(q->userid == user->id && !strcmp(q->isbn, book->isbn)) {
                    strcpy(action_msg, "Already in queue");
                    break;
                }
                q = q->next;
            }

            if(strlen(action_msg)) continue;

            // Add user to queue for this book
            struct queuelist *new_q = malloc(sizeof(struct queuelist));
            new_q->userid = user->id;
            strcpy(new_q->isbn, book->isbn);
            new_q->queue = 1;

            struct queuelist *temp = queuehead;
            while(temp) {
                if(strcmp(temp->isbn, book->isbn) == 0) new_q->queue++;
                temp = temp->next;
            }

            new_q->next = queuehead;
            queuehead = new_q;
            
            savequeuefile();
            
            char log_msg[CHARACTERINPUT];
            snprintf(log_msg, CHARACTERINPUT, "User %d queued for: %s (Position %d)", user->id, book->title, new_q->queue);
            log_action(log_msg);
            
            snprintf(action_msg, CHARACTERINPUT, "Added to queue (Position %d)", new_q->queue);
        }
    } while(1);
}

// Interface for returning borrowed books
// Lists books borrowed by user and allows selection to return
void return_books_interface(struct account *user, char *action_msg) {
    int choice, count = 0;
    struct borrowlist *current = borrowhead;

    do {
        count = 0;
        current = borrowhead;
        // Count how many books user currently borrowed
        while(current) {
            if(current->userid == user->id) count++;
            current = current->next;
        }

        print_user_section_header("Return Books", user->id);
        if(strlen(action_msg)) {
            printf("[Action Bar] : %s!\n\n", action_msg);
            action_msg[0] = '\0';
        }

        if(count == 0) {
            printf("No books to return!\n");
            printf("[0] Return: ");
            int ch;
            do { scanf("%d", &ch); } while(ch != 0);
            return;
        }

        // Print table of borrowed books
        printf("%-5s %-20s %-35s %-15s %-15s\n", 
            "NUM", "ISBN", "Title", "Borrow Date", "Return Date");
        printf("------------------------------------------------------------------------------------------------\n");
        
        current = borrowhead;
        int pos = 1;
        while(current) {
            if(current->userid == user->id) {
                // Find book details for each borrowed book
                struct booklist *book = bookhead;
                while(book && strcmp(book->isbn, current->isbn)) book = book->next;
                
                printf("[%-2d] %-20s %-35s %-15s %-15s\n", 
                    pos++, 
                    current->isbn, 
                    book ? book->title : "Unknown",
                    current->borrowdate,
                    current->returndate);
            }
            current = current->next;
        }

        printf("\n[0] Cancel\n[1-%d] Select book to return\n", count);
        printf("Select an option: ");
        
        if(scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            strcpy(action_msg, "Invalid input");
            continue;
        }

        if(choice == 0) return;
        if(choice < 1 || choice > count) {
            strcpy(action_msg, "Invalid selection");
            continue;
        }

        // Find the borrow record user wants to return
        struct borrowlist **pp = &borrowhead;
        int target = 1;
        while(*pp) {
            if((*pp)->userid == user->id) {
                if(target++ == choice) break;
            }
            pp = &(*pp)->next;
        }

        if(*pp) {
            // Find the book and update availability
            struct booklist *book = bookhead;
            while(book && strcmp(book->isbn, (*pp)->isbn)) book = book->next;
            
            if(book) {
                book->availability++;
                // Notify next user in queue if any
                struct queuelist *q = queuehead;
                while(q) {
                    if(strcmp(q->isbn, book->isbn) == 0 && q->queue == 1) {
                        printf("\nNotification: User %d can now borrow %s!\n", q->userid, book->title);
                        break;
                    }
                    q = q->next;
                }
            }
            
            // Remove borrow record and free memory
            struct borrowlist *to_free = *pp;
            *pp = (*pp)->next;
            free(to_free);
            
            // Save updated data
            savebookfile();
            saveborrowfile();
            savequeuefile();
            
            // Log the return action
            char log_msg[CHARACTERINPUT];
            snprintf(log_msg, CHARACTERINPUT, "User %d returned book", user->id);
            log_action(log_msg);
            
            strcpy(action_msg, "Book returned successfully");
        }
    } while(1);
}

// Main menu for logged-in user with options to sort, search, borrow, return, and view queue
void user_function(struct account *user) {
    int choice;
    char action_msg[CHARACTERINPUT] = "";

    do {
        print_book_table();
        print_user_section_header("Main Menu", user->id);

        if(strlen(action_msg)) {
            printf("[Action Bar] : %s!\n\n", action_msg);
            action_msg[0] = '\0';
        }

        // Display menu options
        printf("[1] Sort Books\n");
        printf("[2] Search Books\n");
        printf("[3] Borrow Books\n");
        printf("[4] Return Books\n");
        printf("[5] My Queue\n");
        printf("[0] Logout\n");
        printf("-------------------------------\n");
        printf("Select an option: ");

        if(scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            strcpy(action_msg, "Please enter a number");
            continue;
        }

        // Call appropriate function based on user choice
        switch(choice) {
            case 1: 
                log_action("User sorted books");
                sort_books_interface(user, action_msg); 
                break;
            case 2: 
                log_action("User searched books");
                search_books_interface(user, action_msg); 
                break;
            case 3: 
                log_action("User borrowed books");
                borrow_books_interface(user, action_msg); 
                break;
            case 4: 
                log_action("User returned books");
                return_books_interface(user, action_msg); 
                break;
            case 5: 
                log_action("User viewed queue");
                view_queue_interface(user, action_msg); 
                break;
            case 0: 
                log_action("User logged out");
                return;
            default: 
                strcpy(action_msg, "Invalid choice - Select 0-5");
        }
    } while(choice != 0);
}

// Interface to sort books by title, author, or availability
void sort_books_interface(struct account *user, char *action_msg) {
    int choice;

    do {
        print_book_table();
        print_user_section_header("Sort Options", user->id);

        if(strlen(action_msg)) {
            printf("[Action Bar] : %s!\n\n", action_msg);
            action_msg[0] = '\0';
        }

        // Sorting options menu
        printf("[1] Title (A-Z)\n");
        printf("[2] Author\n");
        printf("[3] Availability\n");
        printf("[0] Return\n\n");
        printf("Select an option: ");

        if(scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            strcpy(action_msg, "Invalid input");
            continue;
        }

        if(choice == 0) return;
        if(choice < 1 || choice > 3) {
            strcpy(action_msg, "Invalid choice");
            continue;
        }

        // Call sorting function with selected criteria
        sort_book_list(choice - 1);
        savebookfile();
        
        // Log sorting action
        char log_msg[CHARACTERINPUT];
        const char *types[] = {"Title", "Author", "Availability"};
        snprintf(log_msg, CHARACTERINPUT, "User %d sorted by %s", user->id, types[choice-1]);
        log_action(log_msg);
        
        strcpy(action_msg, "Books sorted successfully");
    } while(1);
}

// Interface for searching books by title, author, or ISBN
void search_books_interface(struct account *user, char *action_msg) {
    int choice;
    char search_term[MAX_INPUT];
    char normalized_term[MAX_INPUT];
    int search_mode = 0;

    do {
        print_book_table();
        print_user_section_header("Search Books", user->id);
        if(strlen(action_msg)) {
            printf("[Action Bar] : %s!\n\n", action_msg);
            action_msg[0] = '\0';
        }

        // Search type menu
        printf("[1] Title Search\n");
        printf("[2] Author Search\n");
        printf("[3] ISBN Search\n");
        printf("[0] Return to Main Menu\n");
        printf("-------------------------------\n");
        printf("Select an option: ");
        
        if(scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            strcpy(action_msg, "Invalid input");
            continue;
        }

        if(choice == 0) return;
        if(choice < 1 || choice > 3) {
            strcpy(action_msg, "Invalid selection");
            continue;
        }

        // Get search term from user
        printf("Enter search term: ");
        scanf(" %99[^\n]", search_term);
        normalize_string(search_term);
        strcpy(normalized_term, search_term);

        // Log search action
        char log_msg[CHARACTERINPUT];
        const char *types[] = {"title", "author", "ISBN"};
        snprintf(log_msg, CHARACTERINPUT, "User %d searched %s: %s", user->id, types[choice-1], search_term);
        log_action(log_msg);

        printf("\nSearch Results:\n");
        printf("%-5s %-35s %-30s %-20s %-10s\n", 
            "NUM", "Title", "Author", "ISBN", "Available");
        printf("--------------------------------------------------------------------------------------------------------\n");

        struct booklist *current = bookhead;
        int position = 1;
        int found = 0;
        
        // Loop through books and print those that match search term
        while(current != NULL) {
            char target[MAX_INPUT];
            switch(choice) {
                case 1: strcpy(target, current->title); break;
                case 2: strcpy(target, current->author); break;
                case 3: strcpy(target, current->isbn); break;
            }
            
            char normalized_target[MAX_INPUT];
            strcpy(normalized_target, target);
            normalize_string(normalized_target);
            
            // Check if normalized search term is in the book's field
            if(strstr(normalized_target, normalized_term)) {
                printf("[%-2d] %-35s %-30s %-20s %-10s\n", 
                    position++, 
                    current->title,
                    current->author,
                    current->isbn,
                    current->availability > 0 ? "Yes" : "No");
                found++;
            }
            current = current->next;
        }

        if(found == 0) {
            printf("No matching books found.\n");
        }
        printf("\nTotal found: %d\n", found);
        
        printf("\n[1] New Search\n");
        printf("[0] Main Menu\n");
        printf("Select an option: ");
        
        int exit_choice;
        while(1) {
            scanf("%d", &exit_choice);
            if(exit_choice == 0 || exit_choice == 1) break;
            printf("Invalid choice! Select again: ");
        }
        
        if(exit_choice == 0) return;
        
    } while(1);
}
