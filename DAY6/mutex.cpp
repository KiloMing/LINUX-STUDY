#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pthread.h>

struct SharedData
{
    int count;
    //if you don't define this variable，the mutex will not be shared
    pthread_mutex_t mutex;
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
    pid_t pid = fork();
    if(pid < 0)
    {
        perror("fork");
        return 1;
    }
    if(pid == 0)
    {
        //child process
        pthread_mutex_lock(&shared->mutex);
        shared->count++;
        std::cout << shared->count << std::endl;
        pthread_mutex_unlock(&shared->mutex);
    }
    else
    {
        // parent process
        pthread_mutex_lock(&shared->mutex);
        shared->count++;
        std::cout << shared->count << std::endl;
        pthread_mutex_unlock(&shared->mutex);
        wait(nullptr);
        pthread_mutex_destroy(&shared->mutex);
    }
}