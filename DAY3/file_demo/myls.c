#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>



int main()
{
    DIR *dir;
    dir = opendir(".");
    if (dir == NULL)
    {
        perror("Error opening directory");
        return 1;   
    }
    struct dirent *entry;
    struct stat fileStat;
    while ((entry = readdir(dir)) != NULL)
    {
        stat(entry->d_name, &fileStat);
        if (S_ISDIR(fileStat.st_mode))
        {
            printf("D:%s ", entry->d_name);
            printf(" %lld\n", (long long)fileStat.st_size);
        }
        else
        {
            printf("N:%s ", entry->d_name);
            printf(" %lld\n", (long long)fileStat.st_size);
        }
    }
    closedir(dir);
    return 0;
}