//
//  main.c
//  Week2PartA
//
//  Created by Hao WU on 11/25/16.
//  Copyright © 2016 Hao WU. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#define INPUT_BUFFER_SIZE 1

struct termios saved_attributes;

void reset_input(){
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
    atexit(reset_input);// atexit will make sure the registered function be called at extit (mian function exit or exit(3))
    tcgetattr(STDIN_FILENO, &temp_attr);
    temp_attr.c_lflag &= ~(ICANON | ECHO);// clear ICANON and ECHO
    temp_attr.c_cc[VMIN]=1;
    temp_attr.c_cc[VTIME]=0;// those two lines are setting that read(2) blocks until MIN bytes are available, and returns up to the number of bytes requested.
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &temp_attr);// the change occurs after all output written to the object referred by fd has been transmitted, and all the input that has been received but not read will be discarded before the change is made
}

int map_and_write(char *buffer,int stored_buffer_length){
    for(int i=0;i<stored_buffer_length;i++){
        if (buffer[i]==004) {
            return 1;
        }
        if(buffer[i]==0x0D || buffer[i]==0x0A){
            buffer[i]=0x0A;
            putchar(buffer[i]);
        }
        else{
            putchar(buffer[i]);
        }
    }
    return 0;
}


int main(int argc, const char * argv[]) {
    char buffer[INPUT_BUFFER_SIZE] = {}; // This is the buffer we are going to read the input in
    int stored_buffer_length=0;//The read buffer length is the length stored in the buffer
    set_input_mode();
    int read_c_D=0;// when read the buffer, abort the while loop
    while(1){
        int read_size=read(0, buffer, INPUT_BUFFER_SIZE);
        stored_buffer_length+=read_size;
        if(stored_buffer_length>0){
           read_c_D=map_and_write(buffer, stored_buffer_length);
            if (read_c_D) {
                break;
            }
        }
        stored_buffer_length=0;
    }
    return 0;
}
