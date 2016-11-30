//
//  main.c
//  Week2PartB
//
//  Created by Hao WU on 11/28/16.
//  Copyright © 2016 Hao WU. All rights reserved.
//

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <getopt.h>
#include <pthread.h>
#define INPUT_BUFFER_SIZE 1

struct termios saved_attributes;
int pipe_c_p[2];
int pipe_p_c[2];
char line_feed[2]={0x0D,0x0A};
int shellrunning;
struct termios oldattr, newattr;

pid_t shell_id_glb;

void reset_input(){
    int shell_stat;
    waitpid(shell_id_glb, &shell_stat, 0);
    if (WIFEXITED(shell_stat)) {
        printf("Shell exit status is %d\n", WEXITSTATUS(shell_stat));
    } else if (WIFSIGNALED(shell_stat)) {
        printf("Shell signalled to exit! Signal: %d\n", WTERMSIG(shell_stat));
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);
}

void set_input_mode(void){
    struct termios temp_attr;// temp attribute in for the set up
    if(!isatty(STDIN_FILENO)){// isatty is to detect whether STDIN_FILENO is a terminal or not
        // STDIN_FILENO is the standard input, it is almost 0
        fprintf(stderr, "Not A Terminal.\n");
        exit(EXIT_FAILURE);
    }
    tcgetattr(STDIN_FILENO, &saved_attributes);// get the attribute from STDIN_FILENO and save it to saved_attributes
    
    tcgetattr(STDIN_FILENO, &temp_attr);
    temp_attr.c_lflag &= ~(ICANON | ECHO);// clear ICANON and ECHO
    temp_attr.c_cc[VMIN]=1;
    temp_attr.c_cc[VTIME]=0;// those two lines are setting that read(2) blocks until MIN bytes are available, and returns up to the number of bytes requested.
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &temp_attr);// the change occurs after all output written to the object referred by fd has been transmitted, and all the input that has been received but not read will be discarded before the change is made
}

int map_and_write(char *buffer,int stored_buffer_length,pid_t shell_pid){
    for(int i=0;i<stored_buffer_length;i++){
        if (buffer[i]==004) {// receive Ctrl+D from the terminal
            if (shellrunning) {
                close(pipe_p_c[1]);
                close(pipe_c_p[0]);
                kill(shell_pid, SIGHUP);
                exit(0);
            }
            else{
                exit(0);
            }
        }
        if (buffer[i]==003){ // receive Ctrl+C from the terminal
            //write(1, "cc", 2);
            if(shellrunning){
                kill(shell_pid, SIGINT);
                exit(0);
            }
        }
        if(buffer[i]==0x0D || buffer[i]==0x0A){
            write(1, line_feed, 2);
        }
        else{
            write(1,buffer+i,1);
            return 1;
        }
    }
    return 0;
}

void listenKeyboard(pid_t *shell_pid_ptr){
    ssize_t read_size;
    char buffer_keyboard[]={};
    while( (read_size = read(0, buffer_keyboard, INPUT_BUFFER_SIZE) ) >0 ){
        write(pipe_p_c[1], buffer_keyboard, INPUT_BUFFER_SIZE);// forward to pipe
        map_and_write(buffer_keyboard, INPUT_BUFFER_SIZE,*shell_pid_ptr);
        //write(1, buffer_keyboard, INPUT_BUFFER_SIZE);// output to screenß
    }
}

static void sigint_handler(int signo)
{
    if (shellrunning) {
        kill(shell_id_glb, SIGINT);
        shellrunning = 0;
    }
}

static void sigpipe_handler(int signo)
{
    exit(1);
}

int main(int argc, char ** argv) {
    pid_t cpid;
    char buffer_pipe[INPUT_BUFFER_SIZE]={};
    atexit(reset_input);// atexit will make sure the registered function be called at extit (mian function exit or exit(3))
    
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
        
        //write(1, "Child is born\n", strlen("Child is born\n"));
        shellrunning=1;
        int rc = execl("/bin/bash", "/bin/bash", NULL);
        _exit(EXIT_SUCCESS);
    }
    else{
        ssize_t read_size;
        signal(SIGINT, sigint_handler);
        signal(SIGPIPE, sigpipe_handler);
        pid_t *shellid_ptr=calloc(1, sizeof(pid_t));
        *shellid_ptr=cpid;
        shell_id_glb=cpid;
        close(pipe_p_c[0]);
        close(pipe_c_p[1]);
        //write(pipe_p_c[1], "ls\n", strlen("ls\n"));
        
        //set_input_mode();
        
/*        if (tcgetattr(STDIN_FILENO, &oldattr) == -1) {
         perror("Cannot get attr of terminal!");
         // a custom exit status for all other unexpected errors
         exit(3);
         }
         newattr = oldattr;
         newattr.c_lflag &= ~(ICANON | ECHO);
         if (tcsetattr(STDIN_FILENO, TCSANOW, &newattr) == -1) {
         perror("Cannot set attr of terminal!");
         exit(3);
         }*/
        set_input_mode();
        pthread_t child_input_thread;
        pthread_create(&child_input_thread,NULL,(void*)listenKeyboard,shellid_ptr);
        while( (read_size = read(pipe_c_p[0], buffer_pipe, INPUT_BUFFER_SIZE) ) >0 ){// this thread is will read from child and output to screen
            write(1, buffer_pipe, INPUT_BUFFER_SIZE);
        }
        free(shellid_ptr);
        exit(EXIT_SUCCESS);
    }
}
