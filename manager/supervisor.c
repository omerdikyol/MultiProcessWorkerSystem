#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

#define SHM_NAME "/shm_name"
#define FIFO_NAME "my_fifo"

void read_pairs(FILE *pairs_file);
void read_result();

int main(int argc, char *argv[])
{
    // Check existence of command line argument
    if (argc != 2)
    {
        printf("Usage: %s <pairs.txt>\n", argv[0]);
        exit(1);
    }

    // Open pairs.txt
    FILE *pairs_file = fopen(argv[1], "r");
    if (pairs_file == NULL)
    {
        perror("Error opening pairs.txt");
        exit(2);
    }

    read_pairs(pairs_file);

    // Close pairs.txt
    if (fclose(pairs_file) == -1)
    {
        perror("Error closing pairs.txt");
        exit(3);
    }

    read_result();

    return 0;
}

void read_pairs(FILE *pairs_file)
{
    // Create fifo
    if (mkfifo(FIFO_NAME, 0600) == -1)
    {
        perror("Error creating fifo");
        exit(3);
    }

    // Open fifo
    int fifo = open(FIFO_NAME, O_WRONLY);
    if (fifo == -1)
    {
        perror("Error opening fifo");
        exit(4);
    }

    // Read pairs from file
    int operand1, operand2;
    while (fscanf(pairs_file, "%d %d", &operand1, &operand2) == 2)
    {
        // Write pair to fifo
        if (write(fifo, &operand1, sizeof(int)) == -1 || write(fifo, &operand2, sizeof(int)) == -1)
        {
            perror("Error writing to fifo");
            exit(5);
        }
    }
}

void read_result()
{
    // Open shared memory object
    int shm = shm_open(SHM_NAME, O_RDONLY, 0600);
    if (shm == -1)
    {
        perror("Error opening shared memory object");
        exit(6);
    }

    // Map shared memory object
    int *result = mmap(NULL, 2 * sizeof(int), PROT_READ, MAP_SHARED, shm, 0);

    // Print result
    printf("%d * %d = %d\n", result[0], result[1], result[0] * result[1]);

    // Unmap shared memory object
    if (munmap(result, 2 * sizeof(int)) == -1)
    {
        perror("Error unmapping shared memory object");
        exit(7);
    }

    // Close shared memory object
    if (close(shm) == -1)
    {
        perror("Error closing shared memory object");
        exit(8);
    }

    // Unlink shared memory object
    if (shm_unlink(SHM_NAME) == -1)
    {
        perror("Error unlinking shared memory object");
        exit(9);
    }
}