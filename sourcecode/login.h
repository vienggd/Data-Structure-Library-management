#ifndef LOGIN_H
#define LOGIN_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Displays the login interface and handles user authentication
void login_page();

// Allows a user to create a new account; action_msg provides feedback to UI
void create_account(char *action_msg);

// Utility function to print a stylized section header (e.g., "==== Title ====")
void print_section_header(const char *title);

#endif
