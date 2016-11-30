//
//  main.c
//  PipeEchoInput
//
//  Created by Hao WU on 11/28/16.
//  Copyright © 2016 Hao WU. All rights reserved.
//

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int pipe_c_p[2];
int pipe_p_c[2];
#define INPUT_BUFFER_SIZE 1
int main(int argc, const char * argv[]) {
    pid_t cpid;
    char buffer_keyboard[INPUT_BUFFER_SIZE]={};
    char buffer_child[INPUT_BUFFER_SIZE]={};
    pipe(pipe_c_p);
    pipe(pipe_p_c);
    cpid=fork();
    if (cpid==0) {//Child
        close(pipe_p_c[1]);
        write(1, "Child is born", strlen("Child is born"));
        ssize_t read_pipe_size;
        
        while ( (read_pipe_size = read(pipe_p_c[0], buffer_child, INPUT_BUFFER_SIZE)) > 0) {
            write(1, "Y", strlen("Y"));
            write(1, buffer_child, INPUT_BUFFER_SIZE);
        }
        write(1, "Child is dead", strlen("Child is dead"));
        _exit(EXIT_SUCCESS);
    }
    else{
        int read_size;
        close(pipe_p_c[0]);
        while ( (read_size=read(0, buffer_keyboard, INPUT_BUFFER_SIZE)) > 0 ) {
            write(pipe_p_c[1], buffer_keyboard, INPUT_BUFFER_SIZE);
        }
        
        exit(EXIT_SUCCESS);
    }
}
