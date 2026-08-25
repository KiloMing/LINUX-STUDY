#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main() {
    int fd;
    char data[] = "Hello, World!\n";
    fd = open("test.txt", O_WRONLY | O_CREAT, 0666);
    write(fd, data, sizeof(data));
    close(fd);
    printf("s-%d", fd);
    return 0;
}