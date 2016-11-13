//
//  main.c
//  Project0
//
//  Created by Hao WU on 11/12/16.
//  Copyright © 2016 Hao WU. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
static void sigsegv_handler(int signal){
    fprintf(stderr, "caught a SIGSEGV signal");
    exit(3);
}

int main(int argc, char ** argv) {
    size_t max_size=4096;
    static int segmentFaultFlag=0;//static constant will stay in the memory for the life time
    static struct option long_opitons[]={
        {"input",required_argument,NULL,'i'},// 'i' is the value to be returned if read, NULL means it will return the val
        {"output",required_argument,NULL,'o'},// infact it would return if the option string is in "input" and start with i
        {"segfault",no_argument,&segmentFaultFlag,'s'},// if segfault is caught segmentFaultFlag will be 's'
        {"catch",no_argument,NULL,'c'}
    };
    int opt;
    int option_index=0;
    int ifd=0,ofd=1;
    //printf("lol"); printf might in the different order
    while ( (opt = getopt_long(argc, argv, "si:oc:", long_opitons, &option_index)) != -1) {//i is followed by : because it requires an argument
        switch (opt) {
            case 'c':
                signal(SIGSEGV, sigsegv_handler);
                break;
            case 'i':
                ifd=open(optarg, O_RDONLY);
                //printf("i");
                //printf("i '%s'",optarg);
                if (ifd>=0) {
                    close(0);//close the fd0
                    dup(ifd);//use the lowest file descriptor, copy ifd to that
                    close(ifd);//close ifd
                }
                else{
                    fprintf(stderr, "Unable to open %s\n",optarg);
                    perror("Unable to open the file, Yo");
                    exit(1);
                }
                break;
            case 'o':
                //printf("o '%s'",optarg);
                ofd=creat(optarg, 0666);
                if (ifd>=0) {
                    close(1);
                    dup(ofd);
                    close(ofd);
                }
                else{
                    fprintf(stderr, "Unable to create %s\n",optarg);
                    perror("Unable to create the file, Yo");
                    exit(2);
                }
                break;
            default:
                break;
       }
    }
    //printf("%d", segmentFaultFlag);
    if (segmentFaultFlag) {
        int *fault_ptr = NULL;
        *fault_ptr = 3;
    }
    
    size_t readsize=0;
    char buff[max_size];
    while ( (readsize = read(0, buff, max_size)) >0) {
        write(1, buff, readsize);
    }
    return 0;
}
