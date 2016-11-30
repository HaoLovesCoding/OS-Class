//
//  main.c
//  Mutex
//
//  Created by Hao WU on 11/28/16.
//  Copyright © 2016 Hao WU. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
pthread_t tid[2];
int counter=0;
pthread_mutex_t lock;

void* doSomeThing(void *arg){
    pthread_mutex_lock(&lock);
    unsigned long i=0;
    counter+=1;
    printf("\n Job %d started \n", counter);
    for (i=0; i<0xFFFFFFFF; i++);
    printf("\n Job %d finished\n", counter);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    int i = 0;
    int err;
    if(pthread_mutex_init(&lock,NULL)!=0){
        printf("mutex init failed");
        return  1;
    }
    while(i < 2)
    {
        err = pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        i++;
    }
    
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(&lock);
    return 0;
    return 0;
}
