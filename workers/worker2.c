#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/socket.h>

#define SHM_NAME "/shm_name"

void read_result(int pipe);

int main(int argc, char *argv[])
{
    // Create child process
    pid_t pid = fork();

    // Create anonymous channel
    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("Error creating anonymous channel");
        exit(6);
    }

    if (pid < 0)
    {
        perror("Error creating child process");
        exit(1);
    }

    if (pid == 0)
    {
        // Child process
        close(fd[0]);               // Close read end of pipe
        dup2(fd[1], STDOUT_FILENO); // Redirect stdout to write end of pipe
        close(fd[1]);               // Close write end of pipe

        execl("./worker1", "worker1", NULL);
        perror("Error executing worker1");
        exit(2);
    }

    // Parent process

    close(fd[1]); // Close write end of pipe

    read_result(fd[0]); // Read from anonymous channel

    // Wait for child process to terminate
    int status;
    wait(&status);

    close(fd[0]); // Close read end of pipe
    return 0;
}

void read_result(int pipe)
{
    // Create shared memory object
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0660);

    if (shm_fd < 0)
    {
        perror("Error creating shared memory object");
        exit(3);
    }

    // Set size of shared memory object
    if (ftruncate(shm_fd, 2 * sizeof(int)) == -1)
    {
        perror("Error setting size of shared memory object");
        exit(4);
    }

    // Map shared memory object
    int *shm_ptr = mmap(0, 2 * sizeof(int), PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if (shm_ptr == MAP_FAILED)
    {
        perror("Error mapping shared memory object");
        exit(5);
    }

    // Read from anonymous channel
    int difference = 0;
    int number1, number2;

    while (read(pipe, &number1, sizeof(int)) > 0)
    {
        read(pipe, &number2, sizeof(int));

        if (abs(number1 - number2) > difference)
        {
            difference = abs(number1 - number2);
            shm_ptr[0] = number1;
            shm_ptr[1] = number2;
        }
    }

    // Close shared memory object
    if (munmap(shm_ptr, 2 * sizeof(int)) == -1)
    {
        perror("Error unmapping shared memory object");
        exit(7);
    }
    if (close(shm_fd) == -1)
    {
        perror("Error closing shared memory object");
        exit(8);
    }
}
