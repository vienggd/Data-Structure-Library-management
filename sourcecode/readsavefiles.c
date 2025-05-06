#include "readsavefiles.h"
#include "log.h"

// These are the main global variables used throughout the program.
// They store all the accounts, books, borrow records, and queues in memory.
struct account *accountroot = NULL;
struct booklist *bookhead = NULL;
struct borrowlist *borrowhead = NULL;
struct queuelist *queuehead = NULL;
int accountfilecount = 0;
int bookfilecount = 0;
int borrowfilecount = 0;
int queuefilecount = 0;

// Searches for an account in the binary search tree by username
struct account* find_account(struct account* root, const char *username) {
    if (root == NULL) return NULL;
    
    int cmp = strcmp(username, root->username);
    if (cmp == 0) return root;                    // Found the account
    if (cmp < 0) return find_account(root->left, username);  // Search left
    return find_account(root->right, username);              // Search right
}

// Inserts a new account into the BST based on username
struct account* insert_account(struct account* node, struct account* newnode) {
    if (node == NULL) return newnode;
    if (strcmp(newnode->username, node->username) < 0)
        node->left = insert_account(node->left, newnode);
    else
        node->right = insert_account(node->right, newnode);
    return node;
}

// Reads all accounts from a CSV file and builds the account tree
bool readaccountfile() {
    FILE *openfile = fopen("readfile/account.csv", "r");
    accountroot = NULL;
    accountfilecount = 0;

    // If file doesn't exist, create default admin account
    if (openfile == NULL) {
        struct account *admin = malloc(sizeof(struct account));
        admin->id = 0;
        strcpy(admin->username, "admin");
        strcpy(admin->password, "admin");
        admin->isadmin = 1;
        admin->left = NULL;
        admin->right = NULL;
        accountroot = admin;
        accountfilecount++;
        saveaccountfile();  // Save the new admin account to file
        fclose(openfile);
        log_action("Created new account file with admin account");
        return true;
    }

    char header[CHARACTERINPUT];
    fscanf(openfile, "%[^\n]\n", header); // Skip the CSV header line

    // Read each account entry
    while(1) {
        struct account *newnode = malloc(sizeof(struct account));
        newnode->left = NULL;
        newnode->right = NULL;

        int result = fscanf(openfile, "%d,%[^,],%[^,],%d\n",
            &newnode->id,
            newnode->username,
            newnode->password,
            &newnode->isadmin);

        if(result != 4) {
            free(newnode);  // End of file or error
            break;
        }

        // Remove any newline chars in password
        newnode->password[strcspn(newnode->password, "\r\n")] = '\0';
        accountroot = insert_account(accountroot, newnode);
        accountfilecount++;
    }

    fclose(openfile);

    // If admin account is missing, add it
    if (find_account(accountroot, "admin") == NULL) {
        struct account *admin = malloc(sizeof(struct account));
        admin->id = 0;
        strcpy(admin->username, "admin");
        strcpy(admin->password, "admin");
        admin->isadmin = 1;
        admin->left = NULL;
        admin->right = NULL;
        accountroot = insert_account(accountroot, admin);
        accountfilecount++;
        saveaccountfile();
        log_action("Added admin account to system");
    }

    log_action("Loaded account file");
    return true;
}

// Helper function to write each account to file using in-order traversal
void saveaccount_helper(FILE *file, struct account *node) {
    if (node == NULL) return;
    
    saveaccount_helper(file, node->left);
    fprintf(file, "%d,%s,%s,%d\n",
        node->id,
        node->username,
        node->password,
        node->isadmin);
    saveaccount_helper(file, node->right);
}

// Saves all accounts to CSV file
void saveaccountfile() {
    FILE *openfile = fopen("readfile/account.csv", "w");
    fprintf(openfile, "id,username,password,isadmin\n");
    saveaccount_helper(openfile, accountroot);
    fclose(openfile);
    log_action("Saved account file");
}

// Loads book data from file into a linked list
bool readbookfile() {
    FILE *openfile = fopen("readfile/booklist.csv", "r");
    bookhead = NULL;
    bookfilecount = 0;

    if (openfile == NULL) {
        log_action("Book file not found");
        return false;
    }

    char header[CHARACTERINPUT];
    fscanf(openfile, "%[^\n]\n", header); // Skip header

    struct booklist *current = NULL;
    while(1) {
        struct booklist *newnode = malloc(sizeof(struct booklist));
        newnode->next = NULL;

        int result = fscanf(openfile, "%[^,],%[^,],%[^,],%d\n",
            newnode->title,
            newnode->author,
            newnode->isbn,
            &newnode->availability);

        if(result != 4) {
            free(newnode);
            break;
        }

        // Clean up newlines
        newnode->title[strcspn(newnode->title, "\r\n")] = '\0';
        newnode->author[strcspn(newnode->author, "\r\n")] = '\0';
        newnode->isbn[strcspn(newnode->isbn, "\r\n")] = '\0';

        if(bookhead == NULL) {
            bookhead = newnode;
            current = newnode;
        } else {
            current->next = newnode;
            current = newnode;
        }
        bookfilecount++;
    }

    fclose(openfile);
    log_action("Loaded book file");
    return true;
}

// Saves the book linked list to a CSV file
void savebookfile() {
    FILE *openfile = fopen("readfile/booklist.csv", "w");
    if(openfile == NULL) {
        log_action("Error saving book file");
        return;
    }

    fprintf(openfile, "Title,Author,ISBN,Availability\n");
    
    struct booklist *temp = bookhead;
    while(temp != NULL) {
        fprintf(openfile, "%s,%s,%s,%d\n",
            temp->title,
            temp->author,
            temp->isbn,
            temp->availability);
        temp = temp->next;
    }
    
    fclose(openfile);
    log_action("Saved book file");
}

// Loads borrow records into a linked list
bool readborrowfile() {
    FILE *openfile = fopen("readfile/borrow.csv", "r");
    borrowhead = NULL;
    borrowfilecount = 0;

    if (openfile == NULL) {
        log_action("Error opening borrow file");
        return false;
    }

    char header[CHARACTERINPUT];
    fscanf(openfile, "%[^\n]\n", header);

    struct borrowlist *current = NULL;
    while(1) {
        struct borrowlist *newnode = malloc(sizeof(struct borrowlist));
        newnode->next = NULL;

        int result = fscanf(openfile, "%d,%[^,],%[^,],%[^\n]\n",
            &newnode->userid,
            newnode->isbn,
            newnode->borrowdate,
            newnode->returndate);

        if(result != 4) {
            free(newnode);
            break;
        }

        if(borrowhead == NULL) {
            borrowhead = newnode;
            current = newnode;
        } else {
            current->next = newnode;
            current = newnode;
        }
        borrowfilecount++;
    }

    fclose(openfile);
    log_action("Loaded borrow file");
    return true;
}

// Saves all borrow records to CSV
void saveborrowfile() {
    FILE *openfile = fopen("readfile/borrow.csv", "w");
    fprintf(openfile, "userid,isbn,borrowdate,returndate\n");
    
    struct borrowlist *temp = borrowhead;
    while(temp != NULL) {
        fprintf(openfile, "%d,%s,%s,%s\n",
            temp->userid,
            temp->isbn,
            temp->borrowdate,
            temp->returndate);
        temp = temp->next;
    }
    fclose(openfile);
    log_action("Saved borrow file");
}

// Reads queue info from file (users waiting for books)
bool readqueuefile() {
    FILE *openfile = fopen("readfile/queue.csv", "r");
    queuehead = NULL;
    queuefilecount = 0;

    if (openfile == NULL) {
        log_action("Error opening queue file");
        return false;
    }

    char header[CHARACTERINPUT];
    fscanf(openfile, "%[^\n]\n", header);

    struct queuelist *current = NULL;
    while(1) {
        struct queuelist *newnode = malloc(sizeof(struct queuelist));
        newnode->next = NULL;

        int result = fscanf(openfile, "%d,%d,%[^\n]\n",
            &newnode->userid,
            &newnode->queue,
            newnode->isbn);

        if(result != 3) {
            free(newnode);
            break;
        }

        if(queuehead == NULL) {
            queuehead = newnode;
            current = newnode;
        } else {
            current->next = newnode;
            current = newnode;
        }
        queuefilecount++;
    }

    fclose(openfile);
    log_action("Loaded queue file");
    return true;
}

// Saves the queue list to file
void savequeuefile() {
    FILE *openfile = fopen("readfile/queue.csv", "w");
    fprintf(openfile, "userid,queue,isbn\n");
    
    struct queuelist *temp = queuehead;
    while(temp != NULL) {
        fprintf(openfile, "%d,%d,%s\n",
            temp->userid,
            temp->queue,
            temp->isbn);
        temp = temp->next;
    }
    fclose(openfile);
    log_action("Saved queue file");
}

// Loads all 4 files (accounts, books, borrows, queues)
void readallfiles() {
    readaccountfile();
    readbookfile();
    readborrowfile();
    readqueuefile();
    log_action("Loaded all data files");
}

// Helper: Print accounts in-order
void printaccountlist_helper(struct account *node) {
    if (node == NULL) return;
    printaccountlist_helper(node->left);
    printf("%d,%s,%s,%d\n", node->id, node->username, node->password, node->isadmin);
    printaccountlist_helper(node->right);
}

// Print all account data
void printaccountlist() {
    printaccountlist_helper(accountroot);
    printf("Total accounts: %d\n", accountfilecount);
}

// Print all books
void printbooklist() {
    struct booklist *temp = bookhead;
    while(temp != NULL) {
        printf("%s,%s,%s,%d\n", temp->title, temp->author, temp->isbn, temp->availability);
        temp = temp->next;
    }
    printf("Total books: %d\n", bookfilecount);
}

// Print borrow records
void printborrowlist() {
    struct borrowlist *temp = borrowhead;
    while(temp != NULL) {
        printf("%d,%s,%s,%s\n", temp->userid, temp->isbn, temp->borrowdate, temp->returndate);
        temp = temp->next;
    }
    printf("Total borrows: %d\n", borrowfilecount);
}

// Print queue list
void printqueuelist() {
    struct queuelist *temp = queuehead;
    while(temp != NULL) {
        printf("%d,%d,%s\n", temp->userid, temp->queue, temp->isbn);
        temp = temp->next;
    }
    printf("Total queue entries: %d\n", queuefilecount);
}