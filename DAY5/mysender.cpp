#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>

int main()
{
    const char* file = "myfifo";
    std::string message = "Hello, World!";
    if(mkfifo(file, 0666) == -1)
    {
        if(errno != EEXIST)
        {
            perror("mkfilo");
            return 1;
        }
    }
    int fd = open(file, O_WRONLY);
    int i = 5;
    while(i--)
    {
        std::cout << "wait for receive" << std::endl;
        
        if(fd == -1)
        {
            perror("open");
            close(fd);
            return 1;
        }
        std::cout << "connected" << std::endl;
        if(write(fd, message.c_str(), message.size()) == -1)
        {
            perror("write");
            return 1;
        }
        
    }
    close(fd);
}