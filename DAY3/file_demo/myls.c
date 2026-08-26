#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>


// Function to print the file mode in a human-readable format

void print_file_mode(mode_t mode)
{
    char file_type;
    if (S_ISREG(mode))
        file_type = '-';
    else if (S_ISDIR(mode))
        file_type = 'd';
    else if (S_ISLNK(mode))
        file_type = 'l';
    else
        file_type = '?';

    printf("%c", file_type);
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}   

// Function to list files and directories in the given path
void myls(const char *path)
{
    char full_path[1024];
    DIR *dir;
    dir = opendir(path);
    if (dir == NULL)
    {
        perror("Error opening directory");
        return;   
    }
    struct dirent *entry;
    struct stat fileStat;
    while ((entry = readdir(dir)) != NULL)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        if(stat(full_path, &fileStat) < 0)
        {
            perror("Error getting file status");
            continue;
        }
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || entry->d_name[0] == '.')
        {
            continue;
        }
        if (S_ISDIR(fileStat.st_mode))
        {
            print_file_mode(fileStat.st_mode);
            printf(" %s ", full_path);
            printf(" %s ", entry->d_name);
            printf(" %lld\n", (long long)fileStat.st_size);
            printf("\n");
            myls(full_path);
            
        }
        else
        {
            print_file_mode(fileStat.st_mode);
            printf(" %s ", full_path);
            printf(" %s ", entry->d_name);
            printf(" %lld\n", (long long)fileStat.st_size);
        }
        printf("\n");
    }
    closedir(dir);
    return;
}


int main()
{
    const char *path = "/home/kiloming/mygit";
    myls(path);
    return 0;
}