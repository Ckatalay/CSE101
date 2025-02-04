#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

/**
 * @brief Listens for a specific pattern in serial port data and extracts content between [] brackets
 *
 * This function opens and configures a serial port, then continuously reads data from it
 * until it finds a specified pattern. When the pattern is found, it extracts and returns
 * the content between the first set of square brackets that follows the pattern.
 *
 * @param port The serial port device path (e.g., "/dev/ttyUSB0")
 * @param target_pattern The pattern to search for in the incoming serial data
 *
 * @return A dynamically allocated string containing the content between brackets,
 *         or NULL if an error occurs or if no valid pattern is found
 *         The caller is responsible for freeing the returned string.
 *
 * @note The function configures the serial port with the following settings:
 *       - Baud rate: 115200
 *       - 8 data bits, no parity, 1 stop bit
 *       - No hardware flow control
 *       - Raw mode (no canonical processing)
 *
 * @warning This function runs in an infinite loop until the pattern is found or an error occurs
 */
char *listen_for_pattern(const char* port, const char* target_pattern)
{
    if (!port || !target_pattern) {
        printf("Invalid parameters\n");
        return NULL;
    }

    int serialPort = open(port, O_RDWR);
    if (serialPort < 0) {
        printf("Error opening port %s: %s\n", port, strerror(errno));
        return NULL;
    }

    struct termios tty;
    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(serialPort, &tty) != 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        close(serialPort);
        return NULL;
    }

    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);
    tty.c_cflag |= CS8 | CLOCAL | CREAD;
    tty.c_cflag &= ~(PARENB | CSTOPB | CRTSCTS);
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY | IGNBRK | INLCR | ICRNL);
    tty.c_oflag &= ~OPOST;

    if (tcsetattr(serialPort, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        close(serialPort);
        return NULL;
    }

    printf("Listening for pattern '%s'...\n", target_pattern);

    char buffer[1024];
    char accum_buffer[4096];
    size_t accum_size = 0;
    char* result = NULL;
    
    memset(buffer, 0, sizeof(buffer));
    memset(accum_buffer, 0, sizeof(accum_buffer));
    
    while (1) {
        int bytes_read = read(serialPort, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            
            if (accum_size + bytes_read < sizeof(accum_buffer)) {
                memcpy(accum_buffer + accum_size, buffer, bytes_read + 1);
                accum_size += bytes_read;
            }
            
            char* pattern_start = strstr(accum_buffer, target_pattern);
            if (pattern_start != NULL) {
                char* first = strchr(pattern_start, '[');
                char* end = strchr(pattern_start, ']');
                
                if (first && end && (end > first)) {
                    size_t len = end - first - 1;
                    if (len > 0) {
                        result = (char*)malloc(len + 1);
                        if (result != NULL) {
                            strncpy(result, first + 1, len);
                            result[len] = '\0';
                            printf("Pattern found: %s\n", result);
                            close(serialPort);
                            return result;
                        }
                    }
                }
            }
            
            if (accum_size > sizeof(accum_buffer) - 256) {
                memset(accum_buffer, 0, sizeof(accum_buffer));
                accum_size = 0;
            }
        }
    }

    close(serialPort);
    return NULL;
}

int write_to_serial(const char* port, const char* message) {
    int serialPort = open(port, O_RDWR);
    if (serialPort < 0) {
        printf("Error opening port %s: %s\n", port, strerror(errno));
        return -1;
    }

    struct termios tty;
    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(serialPort, &tty) != 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        close(serialPort);
        return -1;
    }

    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);
    tty.c_cflag |= CS8 | CLOCAL | CREAD;
    tty.c_cflag &= ~(PARENB | CSTOPB | CRTSCTS);
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY | IGNBRK | INLCR | ICRNL);
    tty.c_oflag &= ~OPOST;

    if (tcsetattr(serialPort, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        close(serialPort);
        return -1;
    }

    int bytes_written = write(serialPort, message, strlen(message));
    close(serialPort);
    return bytes_written;
}