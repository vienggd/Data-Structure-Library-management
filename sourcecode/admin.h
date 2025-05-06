#ifndef ADMIN_H
#define ADMIN_H

// Entry point for admin-related operations (main admin menu)
void admin_function();
// Allows admin to sort books (e.g., by title, author, etc.)
void admin_sort_books();
// Allows admin to search for books using title, author, or ISBN
void admin_search_books();
// Handles management of the borrow queue (view, add, remove)
void manage_queue();
// Handles account management (view, edit, delete users)
void manage_accounts();
// Prints a formatted table of all accounts (used in UI)
void print_account_table();
// Prints a formatted table of the borrow queue
void print_queue_table();
#endif
