#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>



int main(void)
{
    // printf("Executing 'ls -l' using execvp:\n");
    // execlp("ls", "ls", "-l", NULL);
    // printf("This line will not be executed if execvp is successful.\n");
    pid_t pid = fork();
    if(pid < 0)
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)
    {
        // Child process
        printf("Executing 'ls -l' in child process using execvp:\n");
        execlp("ls", "ls", "-l", NULL);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent process
        wait(NULL); // Wait for the child process to finish
        printf("Child process finished executing 'ls -l'.\n");
    }
}