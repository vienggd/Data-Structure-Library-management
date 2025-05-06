#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "readsavefiles.h"
#include "login.h"
#include "user.h"
#include "admin.h"

int main(void) {
    // First, load all the necessary data from files (accounts, books, borrow records, queues)
    readallfiles();

    // Then show the login page to the user so they can log in as admin or normal user
    login_page();
    
    return 0;
}