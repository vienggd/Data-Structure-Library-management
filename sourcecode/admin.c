#include "readsavefiles.h"
#include "login.h"
#include "algorithm.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "log.h"
#include "user.h"

void manage_books();
void manage_queue();
void manage_accounts();
void print_book_table_admin();
void print_account_table();
void print_queue_table();
void delete_book();
void edit_book();
void admin_sort_books();
void add_book();
void admin_search_books();
void add_account(void);
void delete_account(void); 
void edit_account(void);
void view_log_interface();

// Prints a formatted action message if `msg` is not empty
void print_action_message(const char *msg) {
    if(strlen(msg)) printf("[Action]: %s!\n\n", msg);
}

// Clears remaining input in the stdin buffer to prevent input issues
void clear_input_buffer() {
    while(getchar() != '\n');
}

// Main dashboard for admin functionalities
// Allows admin to manage books, queues, accounts, and logs
void admin_function() {
    char action_msg[CHARACTERINPUT] = "";
    int choice;
    
    do {
        print_section_header("Admin Dashboard");
        print_action_message(action_msg);
        action_msg[0] = '\0';
        
        printf("[1] Manage Books\n");
        printf("[2] Manage Queue\n");
        printf("[3] Manage Accounts\n");
        printf("[4] View Log\n");
        printf("[0] Logout\n");
        printf("---------------------\n");
        printf("Select an option: ");
        
        if(scanf("%d", &choice) != 1) {
            clear_input_buffer();
            strcpy(action_msg, "Invalid input");
            continue;
        }

        switch(choice) {
            case 1: 
                log_action("Accessed book management");
                manage_books(); 
                break;
            case 2: 
                log_action("Accessed queue management");
                manage_queue(); 
                break;
            case 3: 
                log_action("Accessed account management");
                manage_accounts(); 
                break;
            case 4: 
                log_action("Viewed system logs");
                view_log_interface();
                break;
            case 0: 
                log_action("Admin logged out");
                return;
            default: 
                strcpy(action_msg, "Invalid choice");
        }
    } while(1);
}

// Displays system logs with timestamp and action
// Reads from "library.log"
void view_log_interface() {
    int choice;
    FILE *log_file = fopen("library.log", "r");
    
    do {
        print_section_header("System Logs");
        printf("%-20s %s\n", "Timestamp", "Action");
        printf("----------------------------------------\n");
        
        if(log_file) {
            char line[CHARACTERINPUT];
            while(fgets(line, CHARACTERINPUT, log_file)) {
                line[strcspn(line, "\n")] = '\0';
                printf("%.19s - %s\n", line + 1, line + 22);
            }
            rewind(log_file);
        } else {
            printf("No log entries found!\n");
        }
        
        printf("\n[0] Return\nSelect option: ");
        scanf("%d", &choice);
    } while(choice != 0);
    
    if(log_file) fclose(log_file);
}

// Submenu for book management
// Options: sort, search, add, delete, edit
void manage_books() {
    char action_msg[CHARACTERINPUT] = "";
    int choice;
    
    do {
        print_section_header("Manage Books");
        print_action_message(action_msg);
        action_msg[0] = '\0';
        
        print_book_table_admin();
        
        printf("\n[1] Sort\n[2] Search\n[3] Add\n[4] Delete\n[5] Edit\n[0] Return\n");
        printf("Select option: ");

        if(scanf("%d", &choice) != 1) {
            clear_input_buffer();
            strcpy(action_msg, "Invalid input");
            continue;
        }

        switch(choice) {
            case 1: admin_sort_books(); break;
            case 2: admin_search_books(); break;
            case 3: add_book(); break;
            case 4: delete_book(); break;
            case 5: edit_book(); break;
            case 0: return;
            default: strcpy(action_msg, "Invalid choice");
        }
    } while(1);
}

// Allows sorting books by title, author, or availability
// Logs action and saves sorted list
void admin_sort_books() {
    char action_msg[CHARACTERINPUT] = "";
    int choice;
    
    do {
        print_section_header("Sort Books");
        print_action_message(action_msg);
        print_book_table_admin();
        
        printf("\n[1] Title\n[2] Author\n[3] Availability\n[0] Return\n");
        printf("Select option: ");

        if(scanf("%d", &choice) != 1) {
            clear_input_buffer();
            strcpy(action_msg, "Invalid input");
            continue;
        }

        if(choice == 0) return;
        if(choice < 1 || choice > 3) {
            strcpy(action_msg, "Invalid choice");
            continue;
        }

        sort_book_list(choice - 1);
        savebookfile();
        log_action("Sorted books");
        strcpy(action_msg, "Books sorted");
    } while(1);
}

// Prompts admin to select a book and removes it from the linked list
// Updates file and logs deletion
void delete_book() {
    char action_msg[CHARACTERINPUT] = "";
    int choice;
    
    print_section_header("Delete Book");
    print_book_table_admin();
    
    printf("\n[0] Cancel\n[1-%d] Select book: ", bookfilecount);
    if(scanf("%d", &choice) != 1) {
        clear_input_buffer();
        strcpy(action_msg, "Invalid input");
        return;
    }
    
    if(choice == 0) return;
    if(choice < 1 || choice > bookfilecount) {
        strcpy(action_msg, "Invalid choice");
        return;
    }

    struct booklist **pp = &bookhead;
    for(int i = 1; i < choice; i++) pp = &(*pp)->next;

    if(*pp) {
        char log_msg[CHARACTERINPUT];
        snprintf(log_msg, CHARACTERINPUT, "Deleted book: %s", (*pp)->title);
        log_action(log_msg);

        struct booklist *to_delete = *pp;
        *pp = (*pp)->next;
        free(to_delete);
        bookfilecount--;
        savebookfile();
        strcpy(action_msg, "Book deleted");
    }
}

// Prompts for new values for book fields (title, author, ISBN, availability)
// Keeps current values if input is empty
void edit_book() {
    char action_msg[CHARACTERINPUT] = "";
    int choice;
    
    print_section_header("Edit Book");
    print_book_table_admin();
    
    printf("\n[0] Cancel\n[1-%d] Select book to edit: ", bookfilecount);
    if(scanf("%d", &choice) != 1) {
        clear_input_buffer();
        strcpy(action_msg, "Invalid input");
        return;
    }
    
    if(choice == 0) return;

    struct booklist *book = bookhead;
    for(int i = 1; i < choice; i++) {
        if(!book) break;
        book = book->next;
    }

    if(!book) {
        strcpy(action_msg, "Invalid selection");
        return;
    }

    char input[CHARACTERINPUT];
    clear_input_buffer();
    
    // Edit Title
    printf("\nCurrent title: %s", book->title);
    printf("\nNew title (press Enter to keep current): ");
    fgets(input, CHARACTERINPUT, stdin);
    input[strcspn(input, "\n")] = '\0';
    if(strlen(input) > 0) strcpy(book->title, input);

    // Edit Author
    printf("\nCurrent author: %s", book->author);
    printf("\nNew author (press Enter to keep current): ");
    fgets(input, CHARACTERINPUT, stdin);
    input[strcspn(input, "\n")] = '\0';
    if(strlen(input) > 0) strcpy(book->author, input);

    // Edit ISBN
    printf("\nCurrent ISBN: %s", book->isbn);
    printf("\nNew ISBN (press Enter to keep current): ");
    fgets(input, CHARACTERINPUT, stdin);
    input[strcspn(input, "\n")] = '\0';
    if(strlen(input) > 0) strcpy(book->isbn, input);

    // Edit Availability
    printf("\nCurrent availability: %s", book->availability ? "Available" : "Unavailable");
    printf("\nNew availability (1=Available, 0=Unavailable, Enter=keep current): ");
    fgets(input, CHARACTERINPUT, stdin);
    input[strcspn(input, "\n")] = '\0';
    if(strlen(input) > 0) {
        int new_avail = atoi(input);
        if(new_avail == 0 || new_avail == 1) {
            book->availability = new_avail;
        } else {
            strcpy(action_msg, "Invalid availability value - keeping current");
        }
    }

    savebookfile();
    
    char log_msg[CHARACTERINPUT];
    snprintf(log_msg, CHARACTERINPUT, "Edited book: %s (ISBN: %s)", 
           book->title, book->isbn);
    log_action(log_msg);
    
    if(strlen(action_msg)) return;
    strcpy(action_msg, "Book updated successfully");
}

// Submenu to manage the queue of book requests
// Admin can add or remove users from the queue for specific books
void manage_queue() {
    char action_msg[CHARACTERINPUT] = "";
    int choice;
    
    do {
        print_section_header("Manage Queue");
        print_action_message(action_msg);
        print_queue_table();
        
        printf("\n[1] Add\n[2] Remove\n[0] Return\nSelect option: ");

        if(scanf("%d", &choice) != 1) {
            clear_input_buffer();
            strcpy(action_msg, "Invalid input");
            continue;
        }

        switch(choice) {
            case 1: {
                int book_num, user_num;
                print_book_table_admin();
                printf("\nSelect book (0 cancel): ");
                scanf("%d", &book_num);
                if(book_num < 1 || book_num > bookfilecount) {
                    strcpy(action_msg, "Invalid book");
                    continue;
                }

                print_account_table();
                printf("\nSelect user (0 cancel): ");
                scanf("%d", &user_num);
                if(user_num < 1 || user_num > accountfilecount) {
                    strcpy(action_msg, "Invalid user");
                    continue;
                }

                struct booklist *book = bookhead;
                for(int i = 1; i < book_num; i++) book = book->next;

                struct account *user = NULL;
                struct account *stack[100];
                int top = -1;
                struct account *current = accountroot;
                int count = 1;

                while(current || top >= 0) {
                    while(current) {
                        stack[++top] = current;
                        current = current->left;
                    }
                    current = stack[top--];
                    if(count++ == user_num) {
                        user = current;
                        break;
                    }
                    current = current->right;
                }

                struct queuelist *new_q = malloc(sizeof(struct queuelist));
                new_q->userid = user->id;
                strcpy(new_q->isbn, book->isbn);
                new_q->queue = 1;

                struct queuelist *q = queuehead;
                while(q) {
                    if(strcmp(q->isbn, book->isbn) == 0) new_q->queue++;
                    q = q->next;
                }

                new_q->next = queuehead;
                queuehead = new_q;
                savequeuefile();

                char log_msg[CHARACTERINPUT];
                snprintf(log_msg, CHARACTERINPUT, "Added to queue: %s for %s", 
                       book->title, user->username);
                log_action(log_msg);
                
                strcpy(action_msg, "Queue entry added");
                break;
            }
            case 2: {
                int queue_num;
                printf("\nSelect entry to remove (0 cancel): ");
                scanf("%d", &queue_num);
                if(queue_num < 1 || queue_num > queuefilecount) {
                    strcpy(action_msg, "Invalid entry");
                    continue;
                }

                struct queuelist **pp = &queuehead;
                for(int i = 1; i < queue_num; i++) pp = &(*pp)->next;
                
                struct queuelist *to_remove = *pp;
                *pp = (*pp)->next;
                free(to_remove);
                savequeuefile();
                
                log_action("Removed queue entry");
                strcpy(action_msg, "Entry removed");
                break;
            }
            case 0: return;
            default: strcpy(action_msg, "Invalid choice");
        }
    } while(1);
}

// In-order traversal of BST to display all accounts
// Shows username, password, and admin status
void print_account_table() {
    printf("\n%-5s %-15s %-15s %-8s\n", 
         "NUM", "Username", "Password", "Admin");
    printf("--------------------------------------------\n");
    
    struct account *stack[100];
    int top = -1;
    struct account *current = accountroot;
    int counter = 1;

    while(current || top >= 0) {
        while(current) {
            stack[++top] = current;
            current = current->left;
        }
        current = stack[top--];
        
        printf("[%-2d] %-15s %-15s %-8s\n",
            counter++,
            current->username,
            current->password,
            current->isadmin ? "Yes" : "No");
            
        current = current->right;
    }
    printf("\nTotal accounts: %d\n", accountfilecount);
}

// Admin submenu for managing user accounts
// Options: add, delete, or edit users
void manage_accounts() {
    char action_msg[CHARACTERINPUT] = "";
    int choice;
    
    do {
        print_section_header("Manage Accounts");
        print_action_message(action_msg);
        print_account_table();
        
        printf("\n[1] Add\n[2] Delete\n[3] Edit\n[0] Return\nSelect option: ");

        if(scanf("%d", &choice) != 1) {
            clear_input_buffer();
            strcpy(action_msg, "Invalid input");
            continue;
        }

        switch(choice) {
            case 1: add_account(); break;
            case 2: delete_account(); break;
            case 3: edit_account(); break;
            case 0: return;
            default: strcpy(action_msg, "Invalid choice");
        }
    } while(1);
}

// Adds a new account to the binary search tree (BST)
// Ensures username is unique and meets minimum length requirements
void add_account() {
    char action_msg[CHARACTERINPUT] = "";
    struct account *new_acc = malloc(sizeof(struct account));
    new_acc->left = new_acc->right = NULL;

    print_section_header("Add Account");
    
    int max_id = 0;
    struct account *stack[100];
    int top = -1;
    struct account *current = accountroot;
    while(current || top >= 0) {
        while(current) {
            stack[++top] = current;
            current = current->left;
        }
        current = stack[top--];
        if(current->id > max_id) max_id = current->id;
        current = current->right;
    }
    new_acc->id = max_id + 1;

    while(1) {
        printf("\nUsername (4+ chars, 0 cancel): ");
        char username[CHARACTERINPUT];
        fgets(username, CHARACTERINPUT, stdin);
        username[strcspn(username, "\n")] = '\0';
        
        if(strcmp(username, "0") == 0) {
            free(new_acc);
            return;
        }
        
        if(strlen(username) < 4) {
            strcpy(action_msg, "Too short");
            continue;
        }
        
        if(find_account(accountroot, username)) {
            strcpy(action_msg, "Username exists");
            continue;
        }
        
        strcpy(new_acc->username, username);
        break;
    }

    while(1) {
        printf("\nPassword (6+ chars, 0 cancel): ");
        char password[CHARACTERINPUT];
        fgets(password, CHARACTERINPUT, stdin);
        password[strcspn(password, "\n")] = '\0';
        
        if(strcmp(password, "0") == 0) {
            free(new_acc);
            return;
        }
        
        if(strlen(password) < 6) {
            strcpy(action_msg, "Too short");
            continue;
        }
        
        strcpy(new_acc->password, password);
        break;
    }

    printf("\nAdmin (1=Yes, 0=No): ");
    char input[10];
    fgets(input, 10, stdin);
    new_acc->isadmin = atoi(input);

    accountroot = insert_account(accountroot, new_acc);
    accountfilecount++;
    saveaccountfile();
    
    char log_msg[CHARACTERINPUT];
    snprintf(log_msg, CHARACTERINPUT, "Added account: %s", new_acc->username);
    log_action(log_msg);
    
    strcpy(action_msg, "Account created");
}

// Deletes an account from the binary search tree by in-order number
void delete_account() {
    char action_msg[CHARACTERINPUT] = "";
    int acc_num;
    
    print_section_header("Delete Account");
    print_account_table();
    
    printf("\nSelect account (0 cancel): ");
    if(scanf("%d", &acc_num) != 1) {
        clear_input_buffer();
        strcpy(action_msg, "Invalid input");
        return;
    }
    
    if(acc_num == 0) return;
    // Traverse the tree in-order to find the selected account
    struct account *target = NULL;
    struct account *parent = NULL;
    struct account *current = accountroot;
    struct account *stack[100];
    int top = -1;
    int count = 1;

    while(current || top >= 0) {
        while(current) {
            stack[++top] = current;
            current = current->left;
        }
        current = stack[top--];
        
        if(count++ == acc_num) {
            target = current;
            break;
        }
        
        parent = current;
        current = current->right;
    }
    // If no valid account found
    if(!target) {
        strcpy(action_msg, "Invalid selection");
        return;
    }
    // Prevent deletion of the system admin
    if(target->id == 0) {
        strcpy(action_msg, "Cannot delete admin");
        return;
    }

    if(!target->left && !target->right) {
        if(parent) {
            if(parent->left == target) parent->left = NULL;
            else parent->right = NULL;
        } else {
            accountroot = NULL;
        }
    }
    else if(target->left && target->right) {
        struct account *successor = target->right;
        while(successor->left) successor = successor->left;
        target->id = successor->id;
        strcpy(target->username, successor->username);
        strcpy(target->password, successor->password);
        target->isadmin = successor->isadmin;
        target = successor;
    }
    else {
        struct account *child = target->left ? target->left : target->right;
        if(parent) {
            if(parent->left == target) parent->left = child;
            else parent->right = child;
        } else {
            accountroot = child;
        }
    }
    // Log the deletion
    char log_msg[CHARACTERINPUT];
    snprintf(log_msg, CHARACTERINPUT, "Deleted account: %s", target->username);
    log_action(log_msg);
    // Free memory and update file
    free(target);
    accountfilecount--;
    saveaccountfile();
    strcpy(action_msg, "Account deleted");
}

// Prints a formatted table of all books for admin view
void print_book_table_admin(){
    // Print table headers
    printf("\n%-5s %-35s %-30s %-20s %-10s\n", 
         "NUM", "Title", "Author", "ISBN", "Available");
    printf("--------------------------------------------------------------------------------------------------------\n");

    struct booklist *current = bookhead;
    int counter = 1;
    while(current != NULL) {
        printf("[%-2d] %-35s %-30s %-20s %-10s\n",
            counter++,
            current->title,
            current->author,
            current->isbn,
            current->availability ? "Yes" : "No");
        current = current->next;
    }
    printf("\nTotal books: %d\n", bookfilecount);
}
// Prints a formatted table of the book request queue
void print_queue_table(){
    // Print table headers
    printf("\n%-5s %-20s %-15s %-10s\n", 
         "NUM", "ISBN", "Username", "Position");
    printf("----------------------------------------------------\n");
    
    struct queuelist *q = queuehead;
    int counter = 1;
    // Traverse the queue linked list
    while(q) {
        struct account *user = NULL;
        // === In-order traversal of the account binary search tree to find the user ===
        struct account *stack[100];
        int top = -1;
        struct account *current = accountroot;

        while(current != NULL || top != -1) {
            while(current != NULL) {
                stack[++top] = current;
                current = current->left;
            }
            current = stack[top--];
            
            if(current->id == q->userid) {
                user = current;
                break;
            }
            current = current->right;
        }
        
        printf("[%-2d] %-20s %-15s %-10d\n",
            counter++,
            q->isbn,
            user ? user->username : "Unknown",
            q->queue);
        q = q->next;
    }
    printf("\nTotal queue entries: %d\n", queuefilecount);
}

// Allows admin to search books by title, author, or ISBN with normalized comparison
void admin_search_books() {
    char action_msg[CHARACTERINPUT] = "";  // Message for user feedback
    int choice;
    char search_term[CHARACTERINPUT];
    char normalized_term[CHARACTERINPUT];

    do {
        print_section_header("Search Books");
        print_action_message(action_msg);
        action_msg[0] = '\0';  // Clear message

        print_book_table_admin();  // Display current book list

        // Prompt user for search field
        printf("\n[1] Title\n[2] Author\n[3] ISBN\n[0] Return\n");
        printf("Select search type: ");
        
        // Validate input
        if(scanf("%d", &choice) != 1) {
            clear_input_buffer();
            strcpy(action_msg, "Invalid input");
            continue;
        }

        // Handle invalid or exit choices
        if(choice == 0) return;
        if(choice < 1 || choice > 3) {
            strcpy(action_msg, "Invalid choice");
            continue;
        }

        // Get search term from user
        printf("Enter search term: ");
        clear_input_buffer();
        fgets(search_term, CHARACTERINPUT, stdin);
        search_term[strcspn(search_term, "\n")] = '\0';  // Remove newline

        // Normalize input for case-insensitive search
        normalize_string(search_term);
        strcpy(normalized_term, search_term);

        // Print result table header
        printf("\nSearch Results:\n");
        printf("%-5s %-35s %-30s %-20s %-10s\n", 
            "NUM", "Title", "Author", "ISBN", "Available");
        printf("--------------------------------------------------------------------------------------------------------\n");

        struct booklist *current = bookhead;
        int counter = 1;
        int found = 0;

        // Traverse the book list and compare based on selected field
        while(current != NULL) {
            char target[CHARACTERINPUT];
            switch(choice) {
                case 1: strcpy(target, current->title); break;
                case 2: strcpy(target, current->author); break;
                case 3: strcpy(target, current->isbn); break;
            }

            char normalized_target[CHARACTERINPUT];
            strcpy(normalized_target, target);
            normalize_string(normalized_target);

            // Print match
            if(strstr(normalized_target, normalized_term)) {
                printf("[%-2d] %-35s %-30s %-20s %-10s\n", 
                    counter++,
                    current->title,
                    current->author,
                    current->isbn,
                    current->availability ? "Yes" : "No");
                found++;
            }
            current = current->next;
        }

        // Print message if nothing found
        if(!found) {
            printf("No matching books found.\n");
        }

        // Log the search
        char log_msg[CHARACTERINPUT];
        snprintf(log_msg, CHARACTERINPUT, "Searched books by %s: '%s' (%d found)",
                choice == 1 ? "title" : choice == 2 ? "author" : "ISBN",
                search_term, found);
        log_action(log_msg);

        // Ask user if they want to search again
        printf("\n[0] New Search [1] Return: ");
        int action;
        while(scanf("%d", &action) != 1 || (action != 0 && action != 1)) {
            clear_input_buffer();
            printf("Invalid choice! Select 0 or 1: ");
        }
        if(action == 1) return;
    } while(1);
}


// Allows admin to add a new book to the library system
void add_book() {
    char action_msg[CHARACTERINPUT] = "";  // Message shown after action
    struct booklist *new_book = malloc(sizeof(struct booklist));
    memset(new_book, 0, sizeof(struct booklist));  // Initialize memory

    print_section_header("Add New Book");
    print_action_message(action_msg);

    char input[CHARACTERINPUT];

    // === Get book title ===
    printf("\nTitle (0 to cancel): ");
    clear_input_buffer();
    fgets(input, CHARACTERINPUT, stdin);
    input[strcspn(input, "\n")] = '\0';
    if(strcmp(input, "0") == 0) {
        free(new_book);  // Cancel and clean up
        return;
    }
    strcpy(new_book->title, input);

    // === Get author ===
    printf("Author: ");
    fgets(input, CHARACTERINPUT, stdin);
    input[strcspn(input, "\n")] = '\0';
    strcpy(new_book->author, input);

    // === Get ISBN ===
    printf("ISBN: ");
    fgets(input, CHARACTERINPUT, stdin);
    input[strcspn(input, "\n")] = '\0';
    strcpy(new_book->isbn, input);

    // === Get availability status ===
    printf("Availability (1=Available, 0=Unavailable): ");
    int avail;
    while(scanf("%d", &avail) != 1 || (avail != 0 && avail != 1)) {
        clear_input_buffer();
        printf("Invalid input! Enter 1 or 0: ");
    }
    new_book->availability = avail;

    // === Add new book to the start of the linked list ===
    new_book->next = bookhead;
    bookhead = new_book;
    bookfilecount++;     // Increment book count
    savebookfile();      // Persist to file

    // Log the action
    char log_msg[CHARACTERINPUT];
    snprintf(log_msg, CHARACTERINPUT, "Added new book: %s", new_book->title);
    log_action(log_msg);

    strcpy(action_msg, "Book added successfully");
}

// Allows admin to edit user account details (except system admin)
void edit_account() {
    char action_msg[CHARACTERINPUT] = "";  // Message to show status or errors
    int acc_num;

    print_section_header("Edit Account");
    print_action_message(action_msg);
    print_account_table();  // Show current list of accounts with numbers

    // Prompt for account number
    printf("\nEnter account number to edit (0 to cancel): ");
    if(scanf("%d", &acc_num) != 1) {
        clear_input_buffer();
        strcpy(action_msg, "Invalid input");
        return;
    }

    if(acc_num == 0) {
        strcpy(action_msg, "Edit cancelled");
        return;
    }

    // === Find account by in-order traversal ===
    struct account *target = NULL;
    struct account *stack[100];
    int top = -1;
    struct account *current = accountroot;
    int count = 0;
    char changes[200] = "";  // Description of changes for logging

    while(current != NULL || top != -1) {
        while(current != NULL) {
            stack[++top] = current;
            current = current->left;
        }
        current = stack[top--];

        if(++count == acc_num) {
            target = current;  // Found the selected account
            break;
        }
        current = current->right;
    }

    // === Check if account was found ===
    if(!target) {
        strcpy(action_msg, "Invalid selection");
        return;
    }

    // === Prevent editing of the system admin (id == 0) ===
    if(target->id == 0) {
        strcpy(action_msg, "Cannot edit system admin");
        return;
    }

    char input[CHARACTERINPUT];
    clear_input_buffer();

    // === Edit Username ===
    printf("\nCurrent username: %s", target->username);
    printf("\nNew username (Enter to keep): ");
    fgets(input, CHARACTERINPUT, stdin);
    input[strcspn(input, "\n")] = '\0';  // Trim newline
    if(strlen(input) > 0) {
        // Check if username already exists
        if(find_account(accountroot, input)) {
            strcpy(action_msg, "Username already exists");
            return;
        }
        strcpy(target->username, input);
        strcat(changes, "Username updated, ");
    }

    // === Edit Password ===
    printf("\nCurrent password: %s", target->password);
    printf("\nNew password (Enter to keep): ");
    fgets(input, CHARACTERINPUT, stdin);
    input[strcspn(input, "\n")] = '\0';
    if(strlen(input) > 0) {
        // Enforce minimum password length
        if(strlen(input) < 6) {
            strcpy(action_msg, "Password too short");
            return;
        }
        strcpy(target->password, input);
        strcat(changes, "Password updated, ");
    }

    // === Edit Admin Status ===
    printf("\nCurrent admin: %s", target->isadmin ? "Yes" : "No");
    printf("\nNew admin (1=Yes, 0=No, Enter=keep): ");
    fgets(input, CHARACTERINPUT, stdin);
    input[strcspn(input, "\n")] = '\0';
    if(strlen(input) > 0) {
        int new_admin = atoi(input);
        if(new_admin != 0 && new_admin != 1) {
            strcpy(action_msg, "Invalid admin status");
            return;
        }
        target->isadmin = new_admin;
        strcat(changes, "Admin status updated, ");
    }

    // === Save changes to file ===
    saveaccountfile();

    // === Prepare log and message ===
    if(strlen(changes) == 0) {
        strcpy(action_msg, "No changes made");
    } else {
        changes[strlen(changes) - 2] = '\0';  // Remove trailing comma and space
        char log_msg[CHARACTERINPUT];
        snprintf(log_msg, CHARACTERINPUT, "Edited account %s: %s", 
                target->username, changes);
        log_action(log_msg);  // Log the edit
        strcpy(action_msg, "Account updated");
    }
}