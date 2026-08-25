#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

//STDOUT_FILENO: Standard output file descriptor (1).
//STDERR_FILENO: Standard error file descriptor (2).



int main(void)
{
    int p[2];
    pipe(p);
    write(p[1], "Hello, World!\n", 14);
    char buffer[100];
    ssize_t bytesRead = read(p[0], buffer, sizeof(buffer));
    printf("bytesRead: %zd\n", bytesRead);
    ssize_t bytesRead2 = read(p[0], buffer, sizeof(buffer));
    printf("bytesRead2: %zd\n", bytesRead2);
    close(p[0]);
    return 0;
}