#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>

struct SharedData
{
    sem_t sem;
    int count;
};

int main(void)
{
    SharedData *shared = static_cast<SharedData *>(
        mmap(
            nullptr,
            sizeof(SharedData),
            PROT_READ | PROT_WRITE,
            MAP_SHARED | MAP_ANONYMOUS, // MAP_ANONYMOUS : don't relay on the file
            -1, // don't map file
            0
        )
    );
    shared->count = 0;
    sem_init(&shared->sem, 1, 1);
    pid_t pid = fork();
    if(pid < 0){
        perror("pid");
        return 1;
    }
    if(pid == 0){
        //child process
        sem_wait(&shared->sem);
        shared->count++;
        sem_post(&shared->sem);
    }
    else
    {
        //parent process
        sem_wait(&shared->sem);
        shared->count++;
        sem_post(&shared->sem);
        wait(nullptr);
        sem_destroy(&shared->sem);
    }
    std::cout << shared->count << std::endl; 
}
