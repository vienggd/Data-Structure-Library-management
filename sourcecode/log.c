#include "log.h" // Include the header file for logging

// This function logs actions (like login, logout, errors, etc.) to a text file
void log_action(const char *action) {
    // Open the log file in append mode ("a" means it won't overwrite, just add to the end)
    FILE *log_file = fopen("readfile/library.log", "a");

    // If the file can't be opened for some reason, just stop the function
    if (log_file == NULL) return;

    // Get the current time
    time_t now;
    time(&now); // This gets the current time in raw format
    struct tm *local = localtime(&now); // Converts it into local time (readable format)

    // Write the timestamp and the action message to the log file
    fprintf(log_file, "[%02d/%02d/%04d %02d:%02d:%02d] %s\n",
            local->tm_mday,         // Day
            local->tm_mon + 1,      // Month (starts from 0, so we add 1)
            local->tm_year + 1900,  // Year (starts from 1900)
            local->tm_hour,         // Hour
            local->tm_min,          // Minutes
            local->tm_sec,          // Seconds
            action);                // The actual message passed to the function

    // Done writing, close the file
    fclose(log_file);
}
