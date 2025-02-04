/**
 * @file assignment2.c
 * @brief A calculator program with various mathematical operations
 * @author Çağatay Kağan Atalay 2401040040
 * 
 * This program implements a calculator with the following functions:
 * - Addition of two numbers
 * - Subtraction of two numbers
 * - Multiplication of two numbers
 * - Division of two numbers
 * - Power calculation
 * - Average calculation of multiple numbers
 * - Maximum value finding from multiple numbers
 *
 * @functions
 * get_int() - Gets integer input from user
 * get_float() - Gets float input from user
 * get_array() - Gets array of floats from user
 * sum() - Adds two float numbers
 * sub() - Subtracts two float numbers
 * mult() - Multiplies two float numbers
 * divd() - Divides two float numbers
 * powr() - Calculates power of a number
 * avg() - Calculates average of an array
 * max() - Finds maximum value in an array
 *
 * @global_variables
 * STUDENT - Student name constant
 * SIZE - Maximum size for arrays
 * STOP - Character to stop array input
 * count - Stores array size for array operations
 * by_zero - Flag for division by zero error
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT "ÇAĞATAY KAĞAN ATALAY"
#define SIZE 100
#define STOP ';'

int get_int(char *prompt);
float get_float(char *prompt);
float *get_array(char *prompt, char stop, int *count);
int count;


float sum(float a, float b);
float sub(float a, float b);
float mult(float a, float b);
float divd(float numerator, float denominator);
float powr(float base, int exponent);
float avg(float *nums, int size);
float max(float *nums, int size);
int by_zero = 0;

int main(void)
{
    int menu = -1; // Value for menu choice

    while (1)
    {
        printf("WELCOME TO GTU CALCULATOR MACHINE\n"
                "STUDENT NAME: %s\n", STUDENT);

        do
        {
            menu = get_int("PLEASE SELECT FROM THE FOLLOWING MENU:\n"
                        "(1) ADD TWO NUMBERS\n"
                        "(2) SUBTRACT TWO NUMBERS\n"
                        "(3) MULTIPLY TWO NUMBERS\n"
                        "(4) DIVIDE TWO NUMBERS\n"
                        "(5) TAKE THE NTH POWER OF A NUMBER\n"
                        "(6) FIND AVERAGE OF NUMBERS INPUTTED\n"
                        "(7) FIND THE MAXIMUM OF NUMBERS INPUTTED\n"
                        "(0) EXIT\n"
                        "PLEASE SELECT: ");
            if (menu < 0 || menu > 7)
                printf("Invalid input, please input a number from 0 to 7\n");
        } while (menu < 0 || menu > 7); 

        switch (menu)
        {
            case 1:
            {
                float num1 = get_float("Plase input the first number: ");
                float num2 = get_float("Plase input the second number: ");

                printf("%.2f + %.2f = %.2f\n", num1, num2, sum(num1, num2));
                break;
            }
                
            case 2:
            {
                float num1 = get_float("Plase input the first number: ");
                float num2 = get_float("Plase input the second number: ");

                printf("%.2f - %.2f = %.2f\n", num1, num2, sub(num1, num2));
                break;
            }

            case 3:
            {
                float num1 = get_float("Plase input the first number: ");
                float num2 = get_float("Plase input the second number: ");

                printf("%.2f * %.2f = %.2f\n", num1, num2, mult(num1, num2));
                break;
            }

            case 4:
            {
                float num = get_float("Plase input the numerator: ");
                float deno = get_float("Plase input the denominator: ");
                float result = divd(num, deno);

                if (!by_zero)
                {
                    printf("%.2f / %.2f = %.2f\n", num, deno, result);
                }
                break;  
            }
            case 5:
            {
                float num1 = get_float("Plase input the base: ");
                int num2 = get_int("Plase input the exponent: ");

                printf("%.2f^%d = %.2f\n", num1, num2, powr(num1, num2));
                break;
            }

            case 6:
            {
                float *nums = get_array("Enter an array of numbers. Type ';' to indicate the end of input: ", ';', &count);
                printf("Average: %.2f\n", avg(nums, count));
                free(nums);
                break;
            }

            case 7:
            {
                float *nums = get_array("Enter an array of numbers. Type ';' to indicate the end of input: ", ';', &count);
                printf("Max: %.2f\n", max(nums, count));
                free(nums);
                break;
            }

            case 0:
                return 0;
        }
    }
}

float sum(float a, float b)
{
    return a + b;
}

float sub(float a, float b)
{
    return a - b;
}

float mult(float a, float b)
{
    return a * b;
}

float divd(float numerator, float denominator)
{
    if (denominator != 0)
    {
        return numerator / denominator;
    }
    else
    {
        printf("Division by zero is undefined.\n");
        by_zero = 1;
        return 0;
    }
}

float powr(float base, int exponent)
{
    float result = 1.0;
    int is_negative = 0;

    // Handle negative exponents
    if (exponent < 0)
    {
        is_negative = 1;
        exponent = -exponent;
    }

    // Perform multiplication
    for (int i = 0; i < exponent; i++)
    {
        result *= base;
    }

    // Handle negative exponents
    if (is_negative)
    {
        result = 1.0 / result;
    }

    return result;
}

float avg(float *nums, int size)
{
    float sum = 0;

    for (int i = 0; i < size; i++)
    {
        sum += nums[i];
    }

    return sum / size;
}

float max(float *nums, int size)
{
    float max = nums[0];

    for (int i = 1; i < size; i++)
    {
        if (nums[i] > max)
        {
            max = nums[i];
        }
    }

    return max;
}

// Gets input from user and returns it
int get_int(char *prompt)
{
    int i;
    char c;
    while (1)
    {
        printf("%s", prompt);
        if (scanf("%d", &i) == 1)
        {
            // Valid integer input
            break;
        }
        else
        {
            // Invalid input, clear the buffer
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }
    return i;
}

float get_float(char *prompt)
{
    float f;
    char c;
    while (1)
    {
        printf("%s", prompt);
        if (scanf("%F", &f) == 1)
        {
            // Valid integer input
            break;
        }
        else
        {
            // Invalid input, clear the buffer
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }
    return f;
}

float *get_array(char *prompt, char stop, int *count)
{
    float *array = malloc(SIZE * sizeof(float));

    char c;
    float num;
    *count = 0;

    printf("%s", prompt);
    while (1)
    {
        if (scanf("%f %c", &num, &c) != 2)
        {
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input, please input numbers followed by '%c'\n", stop);
            continue;
        }

        array[*count] = num;
        (*count)++;

        if (c == stop)
        {
            break;
        }
    }
    return array;
}