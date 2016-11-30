#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
	printf("Hello World (pid:%d)\n",(int)getpid());
	int rc=fork();
	if(rc<0){
		fprintf(stderr,"fork failed\n");
		exit(1);
	}
	else if(rc==0){
		printf("hello,I am child (pid:%d)\n",(int)getpid());
		char *myargs[3];
		myargs[0]=strdup("wc");
		myargs[1]="fork.c";
		myargs[2]=NULL;
		execvp(myargs[0],myargs);
		printf("should not be here");
	}
	else{
		printf("hello,I am parent of %d(my pid:%d)\n",rc,(int)getpid());
	}
	return 0;
}
