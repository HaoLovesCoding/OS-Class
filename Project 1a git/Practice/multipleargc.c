#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <pthread.h>


int a=100;
void addme(void *a){// To pass to the pthread_create, the 
	printf("hello\n");
	int c = *(int*)a+1;
	int d=10;
	printf("%d\n", c);
}
struct variable_info{
	int a;
	int b;
};
void magic(void *a){
	struct variable_info *v= (struct  variable_info*)a;
	int sum=v->a+v->b;
	v->a=v->a*v->a;
	printf("%d\n", sum);
}

int main(int argc, char ** argv) {
		int *yh;
		//yh=calloc(1,sizeof(struct variable_info));
		//yh->num=9;
		yh=(int*)calloc(1,sizeof(int));
		*yh=9;
		int yyh=89;

		struct variable_info* pass= (struct variable_info*)calloc(1,sizeof(struct variable_info));
		pass->a=10;
		pass->b=20;
		printf("%d\n", *yh);
        pthread_t child_input_thread;
        pthread_create(&child_input_thread,NULL,(void*)magic,pass);
        sleep(2);
        printf("%d\n", pass->a);
}