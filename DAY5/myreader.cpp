#include <iostream>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int received_times = 0;
    const char *file = "myfifo";
    std::cout << "writing for sending" << std::endl;
    int fd = open("myfifo", O_RDONLY);
    if(fd == -1)
    {
        perror("reading");
        return fd;
    }
    while(1){
        std::cout << "sending connected " << std::endl;
        char buffer[256];
        ssize_t n_buffer = read(fd, buffer, sizeof(buffer)-1);
        if(n_buffer == -1)
        {
            perror("receive");
            close(fd);
            return 1;
        }
        if(n_buffer > 1)
        {
            received_times++;
            buffer[n_buffer] = '\0';
            std::cout << "Received" << received_times << std::endl;
        }
        if(n_buffer == 0)
        {
            std::cout << "finish receiveing " << std::endl;
        }
        std::cout << buffer << std::endl;
        
    }
    close(fd);   
}