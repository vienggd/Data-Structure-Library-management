#ifndef READSAVEFILES_H
#define READSAVEFILES_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define CHARACTERINPUT 100  // Maximum character size for string fields

// Binary search tree node for user accounts
struct account {
    int id;                                 
    char username[CHARACTERINPUT];         
    char password[CHARACTERINPUT];          
    int isadmin;                            
    struct account *left, *right;           
};

// Singly linked list node for books
struct booklist {
    char title[CHARACTERINPUT];             
    char author[CHARACTERINPUT];            
    char isbn[CHARACTERINPUT];              
    int availability;                      
    struct booklist *next;                  
};

// Singly linked list node for borrow records
struct borrowlist {
    int userid;                             
    char isbn[CHARACTERINPUT];              
    char borrowdate[CHARACTERINPUT];        
    char returndate[CHARACTERINPUT];        
    struct borrowlist *next;               
};

// Singly linked list node for queueing users for a book
struct queuelist {
    int userid;                             
    int queue;                              
    char isbn[CHARACTERINPUT];              
    struct queuelist *next;                 
};

extern struct account *accountroot;         
extern struct booklist *bookhead;           
extern struct borrowlist *borrowhead;       
extern struct queuelist *queuehead;         

extern int accountfilecount;                
extern int bookfilecount;                 
extern int borrowfilecount;                 
extern int queuefilecount;                 

bool readaccountfile();                     
void saveaccountfile();                     

bool readbookfile();                        
void savebookfile();                        

bool readborrowfile();                     
void saveborrowfile();                      

bool readqueuefile();                      
void savequeuefile();                      

void readallfiles();                        

// Find account by username in BST
struct account* find_account(struct account* root, const char *username);  
// Insert a new account into BST
struct account* insert_account(struct account* node, struct account* newnode);

#endif
