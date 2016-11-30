//
//  main.c
//  NonCAN
//
//  Created by Hao WU on 11/25/16.
//  Copyright © 2016 Hao WU. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>


struct termios saved_attributes;//This is used to remember the original terminal attributes


void reset_input(){
    tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);// TCSANOW means taking affect immediately
}

void set_input_mode(void){
    struct termios temp_attr;// temp attribute in for the set up
    char *name;
    
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


int main(int argc, const char * argv[]) {
    char c;
    set_input_mode();
    while (1) {
        read(STDIN_FILENO, &c, 1);
        if(c=='\004') // once read, the while loop will break, 004 is control + d
            break;
        else if(c=='q'){
            c='j';
            putchar(c);}
        else
            putchar(c);
    }
    return EXIT_SUCCESS;
}
