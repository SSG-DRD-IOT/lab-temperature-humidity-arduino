#include "shell.h"

using namespace std;

#define READ   0
#define WRITE  1

pid_t popen2(String command, FILE* & fd_in, FILE* &fd_out , char* base_dir)
{
    pid_t child_pid;
    int fd[2];
    pipe(fd);

    if((child_pid = fork()) == -1)
    {
        perror("fork");
    }

    /* child process */
    if (child_pid == 0)
    {
        dup2(fd[WRITE], 1); //Redirect stdout to pipe
        dup2(fd[READ], 0);   //Redirect stdin to pipe

        if (base_dir != NULL) {
            chdir(base_dir);
        }

        execl("/bin/sh", "/bin/sh", "-c", command.c_str(), NULL);
    }

    fd_in = fdopen(fd[READ], "r");
    fd_out =  fdopen(fd[WRITE], "w");

    return child_pid;
}

int pclose2(FILE * fp, FILE * fp_out, pid_t pid)
{
    fclose(fp);
    fclose(fp_out);

    return 0;
}