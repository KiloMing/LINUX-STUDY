#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

//O_RDONLY: Open the file for reading only.
//O_WRONLY: Open the file for writing only.
//O_CREAT: Create the file if it does not exist.
//O_TRUNC: Truncate the file to zero length if it already exists.
//O_RDWR: Open the file for reading and writing.
//O_APPEND: Open the file in append mode.



int main() {
    int src_fd;
    int dir_fd;
    char buffer[100];
    ssize_t bytesRead;
    src_fd = open("test.txt", O_RDONLY);
    dir_fd = open("dir.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (src_fd == -1) {
        perror("Error opening file");
        return 1;
    }
    if (dir_fd == -1) {
        perror("Error opening file");
        close(src_fd);
        return 1;
    }
    while ((bytesRead = read(src_fd, buffer, sizeof(buffer))) > 0) {
        if (write(dir_fd, buffer, bytesRead) != bytesRead) {
            perror("Error writing to file");
            close(src_fd);
            close(dir_fd);
            return 1;
        }
    }
    close(src_fd);
    close(dir_fd);
    return 0;
}