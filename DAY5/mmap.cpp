#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main()
{
    int fd = open("test.txt", O_RDWR);
    if(fd == -1)
    {
        perror("open");
        return 1;
    }
    struct stat st;
    if(fstat(fd, &st) == -1)
    {
        perror("open");
        close(fd);
        return 1;
    }

    size_t file_size = st.st_size;
    ftruncate(fd, 256);
    void *ptr = mmap(
        nullptr, 
        256, 
        PROT_READ|PROT_WRITE, 
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
    data[45] = 's';
    //std::cout.write(data, file_size);
    //close the mmap and file .
    munmap(ptr,file_size);
    close(fd);

    return 0;
}