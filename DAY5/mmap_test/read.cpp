#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main()
{
    const size_t SIZE = 256;
    int fd = open("shared.dat", O_RDWR);
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
    char *data = static_cast<char *>(ptr);
    std::cin.get();
    std::cout << "read : " << data << std::endl;
    munmap(ptr, SIZE);
    close(fd);

    return 0;
}