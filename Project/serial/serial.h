#ifndef SERIAL_H
#define SERIAL_H

#include <stddef.h>

/**
 * @brief Listens for a specific pattern in serial port data and extracts content between [] brackets
 *
 * @param port The serial port device path (e.g., "/dev/ttyUSB0")
 * @param target_pattern The pattern to search for in the incoming serial data
 * @return char* containing the content between brackets, or NULL if error occurs
 */
char *listen_for_pattern(const char* port, const char* target_pattern);

/**
 * @brief Writes a message to the specified serial port
 *
 * @param port The serial port device path (e.g., "/dev/ttyUSB0")
 * @param message The message to write to the serial port
 * @return int Number of bytes written, or -1 if error occurs
 */
int write_to_serial(const char* port, const char* message);

#endif /* SERIAL_H */