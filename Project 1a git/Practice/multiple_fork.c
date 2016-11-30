#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char ** argv){
	int numberOfChildren = 2;
	pid_t *childPids = NULL;
	pid_t p;

	/* Allocate array of child PIDs: error handling omitted for brevity */
	childPids = malloc(numberOfChildren * sizeof(pid_t));

	/* Start up children */
	for (int ii = 0; ii < numberOfChildren; ++ii) {
 	  if ((p = fork()) == 0) {
 	     // Child process: do your work here
 	  		for (int i = 0; i < 0xFFFFFFFF; ++i)
 	  		{
 	  			/* code */
 	  		}
 	  		printf("I am Child %d\n", getpid());
	        exit(0);// This is a must! otherwise child processes will generate a lot of process
	   }
	   else {
	      childPids[ii] = p;// to use and wait 
	      //exit(0);
	}
	}
	int stillWaiting;
	do {
   		stillWaiting = 0;
    	for (int ii = 0; ii < numberOfChildren; ++ii) {
     	if (childPids[ii] > 0) {
        	if (waitpid(childPids[ii], NULL, WNOHANG) != 0) { // not zero, means the process state changed
             /* Child is done */
        		childPids[ii] = 0;
        	}
        	else {
             /* Still waiting on this child */
           		stillWaiting = 1;
        	}
     	}
     	/* Give up timeslice and prevent hard loop: this may not work on all flavors of Unix */
     	sleep(0);
    	}
	} while (stillWaiting);

}