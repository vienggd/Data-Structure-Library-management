#include "login.h"          
#include "readsavefiles.h"   
#include "admin.h"       
#include "user.h"            
#include "log.h"            
#include <stdlib.h>

// Function to print a nice-looking section title
void print_section_header(const char *title) {
    system("cls"); // Clears the screen (Windows only)
    printf("\n==============\n");
    printf("%s\n", title);  // Print the section name
    printf("==========\\\\\n\n");
}

// Function to create a new user account
void create_account(char *action_msg) {
    char username[CHARACTERINPUT];
    char password[CHARACTERINPUT];
    int valid = 0;  // Loop flag, 0 means keep asking for input

    while(!valid) {
        print_section_header("Create Account");

        // If there's a message to show (like error or success), display it
        if(strlen(action_msg)) {
            printf("[Action bar]: %s!\n\n", action_msg);
            action_msg[0] = '\0'; // Clear the message after showing
        }

        printf("[0] Cancel\n");
        printf("[!] Enter username (4+ characters): ");
        if(scanf("%99s", username) != 1) {
            while(getchar() != '\n'); // Clean leftover input
            strcpy(action_msg, "Invalid input format");
            continue;
        }

        // User cancels
        if(strcmp(username, "0") == 0) {
            strcpy(action_msg, "Account creation cancelled");
            log_action("Account creation cancelled");
            return;
        }

        // Check if username is long enough
        if(strlen(username) < 4) {
            strcpy(action_msg, "Username too short");
            continue;
        }

        // Check if username already exists
        if(find_account(accountroot, username) != NULL) {
            strcpy(action_msg, "Username already exists");
            continue;
        }

        printf("\n[0] Cancel\n");
        printf("[!] Enter password (6+ characters): ");
        if(scanf("%99s", password) != 1) {
            while(getchar() != '\n');
            strcpy(action_msg, "Invalid input format");
            continue;
        }

        // Cancel again
        if(strcmp(password, "0") == 0) {
            strcpy(action_msg, "Account creation cancelled");
            log_action("Account creation cancelled");
            return;
        }

        if(strlen(password) < 6) {
            strcpy(action_msg, "Password too short");
            continue;
        }

        valid = 1; // All checks passed
    }

    // Find the next available unique ID for the new account
    int new_id = 1;
    struct account *current = accountroot;
    while(current != NULL) {
        if(current->id >= new_id) new_id = current->id + 1;
        current = current->right;
    }

    // Create and fill the new account structure
    struct account *new_acc = malloc(sizeof(struct account));
    new_acc->id = new_id;
    strcpy(new_acc->username, username);
    strcpy(new_acc->password, password);
    new_acc->isadmin = 0;  // New accounts are regular users
    new_acc->left = NULL;
    new_acc->right = NULL;

    // Add the account to our binary tree and save it to the CSV file
    accountroot = insert_account(accountroot, new_acc);
    accountfilecount++;
    saveaccountfile(); // Save changes to the file

    // Log this creation
    char log_msg[CHARACTERINPUT];
    snprintf(log_msg, CHARACTERINPUT, "Account created: %s", username);
    log_action(log_msg);
    strcpy(action_msg, "Account created successfully");
}

// Function that handles the login page UI and options
void login_page() {
    readaccountfile(); // Load all accounts from the CSV

    char action_msg[CHARACTERINPUT] = ""; // For showing messages

    while(1) {
        print_section_header("Library System");
        if(strlen(action_msg)) {
            printf("[Action bar]: %s!\n\n", action_msg);
            action_msg[0] = '\0';
        }

        // Main menu
        printf("[1] Login\n");
        printf("[2] Create account\n");
        printf("[0] Exit\n");
        printf("---------------------\n");
        printf("Select an option: ");

        int choice;
        if(scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            strcpy(action_msg, "Invalid input");
            continue;
        }

        if(choice == 0) {
            // Exit the whole program
            printf("\nExiting system...\n");
            log_action("System exited");
            exit(0);
        }

        if(choice == 1) {
            // User selected login
            char username[CHARACTERINPUT], password[CHARACTERINPUT];
            int login_attempt = 1;

            while(login_attempt) {
                print_section_header("User Login");

                if(strlen(action_msg)) {
                    printf("[Action bar]: %s!\n\n", action_msg);
                    action_msg[0] = '\0';
                }

                printf("[0] Cancel\n");
                printf("[!] Enter username: ");
                if(scanf("%99s", username) != 1) {
                    while(getchar() != '\n');
                    strcpy(action_msg, "Invalid username format");
                    continue;
                }

                if(strcmp(username, "0") == 0) {
                    strcpy(action_msg, "Login cancelled");
                    login_attempt = 0;
                    break;
                }

                printf("\n[0] Cancel\n");
                printf("[!] Enter password: ");
                if(scanf("%99s", password) != 1) {
                    while(getchar() != '\n');
                    strcpy(action_msg, "Invalid password format");
                    continue;
                }

                if(strcmp(password, "0") == 0) {
                    strcpy(action_msg, "Login cancelled");
                    login_attempt = 0;
                    break;
                }

                // Check credentials
                struct account *user = find_account(accountroot, username);
                if(user != NULL && strcmp(user->password, password) == 0) {
                    // Login success
                    char log_msg[CHARACTERINPUT];
                    snprintf(log_msg, CHARACTERINPUT, "User logged in: %s", username);
                    log_action(log_msg);

                    strcpy(action_msg, "Login successful");

                    // Check if user is admin
                    if(user->isadmin) {
                        admin_function();
                    } else {
                        user_function(user);
                    }

                    login_attempt = 0;
                    break;
                }

                // Login failed
                strcpy(action_msg, "Invalid credentials");
                char log_msg[CHARACTERINPUT];
                snprintf(log_msg, CHARACTERINPUT, "Failed login attempt for username: %s", username);
                log_action(log_msg);
            }
        }

        else if(choice == 2) {
            // Go to account creation
            create_account(action_msg);
        }

        else {
            strcpy(action_msg, "Invalid selection");
        }
    }
}