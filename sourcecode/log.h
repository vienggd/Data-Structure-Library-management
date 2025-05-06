#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <time.h>

// Logs a given action string with a timestamp to a log file.
// Example log entry: [2025-05-06 14:32:10] Added new book: "C Programming"
void log_action(const char *action);

#endif
