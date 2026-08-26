#include <iostream>
#include <cstring>

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main(void)
{
    const size_t SIZE = 256;
    int fd = open("shared.dat", O_RDWR);
    if(ftruncate(fd, SIZE))
    if(fd == -1)
    {
        perror("open");
        return 1;
    }
    void *ptr = mmap(
            nullptr,
            SIZE,
            PROT_READ | PROT_WRITE,
            MAP_SHARED,
            fd,
            0
    );
    if(ptr == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return 1;
    }
    char *data = static_cast<char *> (ptr);
    const char *message = "Hello from wtiter";
    std::memset(data, 0, SIZE);
    std::memcpy(data, message, std::strlen(message));
    std::cout << "Write" << message << std::endl;
    munmap(ptr,SIZE);
    close(fd);
    return 0;
}