/**
 * @file stats.c
 * @brief Functions for analyzing and displaying water consumption statistics
 * 
 * This file contains functions to analyze water consumption data stored in a file
 * and display daily and overall statistics. The consumption rate is defined as
 * 10 grams per second (K constant).
 *
 * @constant K Water consumption rate in grams per second
 *
 * @function is_today
 * @brief Checks if a given Unix timestamp is from today
 * @param unix_time Unix timestamp to check
 * @return 1 if timestamp is from today, 0 otherwise
 *
 * @function print_stats
 * @brief Reads and displays water consumption statistics from a file
 * @param filename Path to the statistics file
 * @return 0 on success, 1 if file cannot be opened
 *
 * The statistics file should contain records in the format:
 * unix_timestamp,duration
 * where duration is in seconds
 *
 * Output includes:
 * - List of today's consumption entries with times
 * - Today's total consumption
 * - All-time daily average consumption
 */
#include <stdio.h>
#include <time.h>
#include <string.h>

#define K 10 // 10 grams per second

/**
 * Checks if a given Unix timestamp represents the current date.
 * 
 * @param unix_time The Unix timestamp to check against current date
 * 
 * @return Returns 1 if the given timestamp is from today, 0 otherwise
 *         Returns 0 on any error (e.g., time conversion failures)
 * 
 * This function compares year, month and day components of two timestamps:
 * the provided unix_time and the current system time. Time of day is ignored
 * in the comparison.
 */
int is_today(time_t unix_time) 
{
    time_t now;
    struct tm time_check, time_now;
    
    // Get current time
    now = time(NULL);
    if (now == -1) return 0;
    
    // Convert times to tm structs safely
    struct tm *tmp = localtime(&unix_time);
    if (!tmp) return 0;
    time_check = *tmp;
    
    tmp = localtime(&now);
    if (!tmp) return 0;
    time_now = *tmp;
    
    return (time_now.tm_year == time_check.tm_year &&
            time_now.tm_mon == time_check.tm_mon &&
            time_now.tm_mday == time_check.tm_mday);
}

/**
 * Reads and displays water consumption statistics from a file.
 * 
 * This function reads time-stamped water consumption records from the specified file
 * and displays them in a formatted table. It shows today's consumption logs with 
 * timestamps and amounts, calculates today's total consumption, and the daily average
 * consumption across all recorded days.
 * 
 * @param filename Path to the statistics file containing timestamp and consumption data
 *                 in the format: "timestamp,amount"
 * 
 * @return 0 on successful execution, 1 if file opening fails
 * 
 * The output format includes:
 * - A table of today's consumption logs with time and amount columns
 * - Today's total consumption
 * - All-time daily average consumption (if there are records)
 * 
 * Note: Consumption values are multiplied by constant K for unit conversion
 */
int print_stats(char *filename)
{
    time_t time;
    float dur;
    float sum_all = 0;
    float sum_today = 0;
    int n = 0;
    char timestr[26];
    FILE *stats = fopen(filename, "r");

    if (stats == NULL)
    {
        printf("Error opening stats file.\n");
        return 1;
    }

    printf("Today's logs\n");
    printf("-----------------------\n");
    printf("%-11s|%11s\n", "Time", "Amount");
    printf("-----------------------\n");

    time_t last_time = 0;
    int last_year = -1;
    int last_yday = -1;

    while (fscanf(stats, "%ld,%f", &time, &dur) == 2)
    {
        sum_all += dur;
        
        // Get the time structure
        struct tm *tm_day = localtime(&time);
        if (tm_day)
        {
            // Check if this is a new day by comparing both year and day of year
            if (tm_day->tm_year != last_year || tm_day->tm_yday != last_yday) 
            {
                n++;
                last_year = tm_day->tm_year;
                last_yday = tm_day->tm_yday;
            }
        }

        if (is_today(time))
        {
            sum_today += dur;
            struct tm *tm_info = localtime(&time);
            if (tm_info)
            {
                strftime(timestr, sizeof(timestr), "%H:%M:%S", tm_info);
                printf("%-11s|%9.2f g\n", timestr, dur * K);
            }
        }
    }

    printf("-----------------------\n");
    printf("Today's total: %.2f g\n", sum_today * K);
    if (n > 0)
    {
        printf("All-time average: %.2f g\n", (sum_all * K) / n);
    }

    fclose(stats);
    return 0;
}