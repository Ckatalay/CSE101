/**
 * @file main.c
 * @brief Main program for a serial communication and data logging system
 *
 * This program provides functionality to:
 * - Record and log duration statistics from a serial device
 * - Display recorded statistics
 * - Generate and display QR codes for device connection
 *
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 *
 * Command line arguments:
 * - No argument: Continuously records duration stats from serial device
 * - -stats or --s: Displays usage statistics
 * - -qr: Creates and displays QR code for connection
 * - -help or --h: Displays usage information
 *
 * @return 0 on successful execution
 * @return 1 if error opening stats file
 * @return 2 if invalid command line arguments
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include "stats/stats.h"
#include "serial/serial.h"

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        FILE *ptr = fopen("stats.csv", "a");

        if (ptr == NULL)
        {
            printf("Error opening stats file.\n");
            return 1;
        }

        while (1)
        {
            printf("Recording stats..\n");
            char *dur = listen_for_pattern("/dev/cu.usbserial-0001", "Duration:");
            if (dur != NULL) {
                time_t now = time(NULL);
                fprintf(ptr, "%ld,%s\n", now, dur);
                // Flush the file buffer to ensure data is written
                fflush(ptr);
                // Free the allocated memory
                free(dur);
                printf("Recorded.\n");
            }
            else
            {
                printf("Failed to get duration\n");
            }
        }
    }
    else if (argc == 2)
    {
        if (!(strcmp(argv[1], "-stats")) || !(strcmp(argv[1], "--s")))
        {
            print_stats("stats.csv");
        }
        else if (!(strcmp(argv[1], "-help")) || !(strcmp(argv[1], "--h")))
        {
            printf("Arguments:\n-stats --s: Displays usage stats.\n-qr: Creates and displays QR-code for connection.\n"
                    "When used without an argument, records usage stats.\n");
        }
        else if (!(strcmp(argv[1], "-delaytime")) || !(strcmp(argv[1], "--d")))
        {
            int time = 0;
            printf("Please input timer in seconds: ");
            scanf("%i", &time);
            time *= 1000;

            FILE *ptr = fopen("arduino/time.txt", "w");
            
            if (ptr == NULL)
            {
                printf("Error opening file.\n");
                return 1;
            }

            fprintf(ptr, "%i", time);
            printf("Successfully changed delay time.\n");
            return 0;
        }
        else
        {
            printf("Error: Invalid argument '%s'. Use -help or --h for usage details.\n", argv[1]);
        }
    }
    else
    {
        printf("Error: Invalid argument. Use -help or --h for usage details.\n");
        return 2;
    }
}