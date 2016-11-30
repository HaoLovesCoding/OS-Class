#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int pipe_c_p[2];
int pipe_p_c[2];
#define INPUT_BUFFER_SIZE 1
int main(int argc, char ** argv) {
    pid_t cpid;
    char buffer_keyboard[INPUT_BUFFER_SIZE]={};
    char buffer_child[INPUT_BUFFER_SIZE]={};
    pipe(pipe_c_p);
    pipe(pipe_p_c);
    cpid=fork();
    if (cpid==0) {//Child
        close(pipe_p_c[1]);
        close(pipe_c_p[0]);

        close(0);
        dup(pipe_p_c[0]);
        close(pipe_p_c[0]);

        close(1);
        dup(pipe_c_p[1]);
        close(2);
        dup(pipe_c_p[1]);
        close(pipe_c_p[1]);

        write(1, "Child is born\n", strlen("Child is born\n"));
        ssize_t read_pipe_size;
        int rc = execl("/bin/bash", "/bin/bash", NULL);
        write(1, "Child is dead", strlen("Child is dead"));
        _exit(EXIT_SUCCESS);
    }
    else{
        int read_size;
        close(pipe_p_c[0]);
        close(pipe_c_p[1]);
        write(pipe_p_c[1], "ls\n", strlen("ls\n"));
        while( (read_size = read(pipe_c_p[0], buffer_keyboard, INPUT_BUFFER_SIZE) ) >0 ){
            write(1, buffer_keyboard, INPUT_BUFFER_SIZE);
        }

        exit(EXIT_SUCCESS);
    }
}