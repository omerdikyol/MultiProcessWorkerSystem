#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>

#define FIFO_NAME "my_fifo"

int gcd(int a, int b);
void process_pairs(int fifo);

int main(int argc, char *argv[])
{
    // Create fifo
    if (mkfifo(FIFO_NAME, 0600) == -1)
    {
        perror("Error creating fifo");
        exit(3);
    }

    // Open fifo
    int fifo = open(FIFO_NAME, O_RDONLY);

    // Read pairs
    process_pairs(fifo);

    // Close fifo
    close(fifo);

    return 0;
}

int gcd(int a, int b)
{
    // Calculate greatest common divisor of two integers
    int temp;
    while (b != 0)
    {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

void process_pairs(int fifo)
{
    // Read pairs from fifo
    int number1, number2;
    while (read(fifo, &number1, sizeof(int)) > 0 && read(fifo, &number2, sizeof(int)) > 0)
    {
        // Check if gcd is equal to 1
        if (gcd(number1, number2) == 1)
        {
            // Send to worker2
            write(STDOUT_FILENO, &number1, sizeof(int)); // Write to stdout (which is redirected to worker2)
            write(STDOUT_FILENO, &number2, sizeof(int)); // Write to stdout (which is redirected to worker2)
        }
    }
}