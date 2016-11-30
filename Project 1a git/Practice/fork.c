#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	printf("Hello World (pid:%d)\n",(int)getpid());
	int rc=fork();
    int a=100;
	if(rc<0){
		fprintf(stderr,"fork failed\n");
		exit(1);
	}
	else if(rc==0){
        wait(100);
		printf("hello,I am child (pid:%d)\n",(int)getpid());
        printf("a is %d\n",a);
	}
	else{
        a=200;
		printf("hello,I am parent of %d (pid:%d)\n",rc,(int)getpid());
        printf("a is %d\n",a);
	}
}
