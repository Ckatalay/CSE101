#ifndef STATS_H
#define STATS_H

#include <time.h>

// Constant for consumption rate in grams per second
#define K 10

/**
 * @brief Checks if a given Unix timestamp is from today
 *
 * @param unix_time Unix timestamp to check
 * @return int 1 if timestamp is from today, 0 otherwise
 */
int is_today(time_t unix_time);

/**
 * @brief Prints statistical data from the specified file
 *
 * @param filename Path to the statistics file
 * @return int 0 on success, non-zero on failure
 */
int print_stats(char *filename);

#endif /* STATS_H */