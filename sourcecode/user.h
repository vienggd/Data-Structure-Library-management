#ifndef USER_H
#define USER_H

#include "readsavefiles.h"

//Main student interface
void user_function(struct account *user);

// Prepares text for searching (removes spaces/lowercase)
void normalize_string(char *str);
// Displays all books in clean table format
void print_book_table(void);
// Shows consistent section headers               
void print_user_section_header(const char *title, int user_id);

// Shows user's waitlist positions
void view_queue_interface(struct account *user, char *action_msg);
// Processes queue choices (borrow/cancel)
void handle_queue_selection(struct account *user,
                           struct queuelist *selected,
                           struct queuelist *prev,
                           char *action_msg);

// Handles book checkout
void borrow_books_interface(struct account *user, char *action_msg);
// Handles book returns
void return_books_interface(struct account *user, char *action_msg);

//Sorts by title/author/availability
void sort_books_interface(struct account *user, char *action_msg);
// Searches books by various criteria
void search_books_interface(struct account *user, char *action_msg);

#endif