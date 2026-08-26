#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <string>
#include <sstream>              //support for istringstream

int main(void)
{
    std::string command = "";
    std::getline(std::cin, command);
    std::istringstream iss(command);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token)
    {
        tokens.push_back(token);
    }
    pid_t pid = fork();
    if(pid < 0)
    {
        std::cerr << "Fork failed" << std::endl;
        return 1;
    }
    else if(pid == 0)
    {
        // Child process
        std::vector<char*> args;
        for(auto& t : tokens)
        {
            args.push_back(&t[0]);
        }
        args.push_back(nullptr);
        execvp(args[0], args.data());
        std::cerr << "Exec failed" << std::endl;
        return 1;
    }
    else
    {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    }
}