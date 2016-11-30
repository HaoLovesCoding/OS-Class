#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char *argv[]){
	int opt;
	printf("listen\n");
	while((opt=getopt(argc,argv,"abc:")) != -1  ){
		printf("%d\n", opt);
		switch(opt){
			case 97:
				printf("a\n");
				break;
			case 98:
				printf("b\n");
				break;
			case 99:
				printf("c\n");
				break;
			default:
				printf("e\n");
				break;
		}
	}
	printf("%d\n", opt);
	return 0;
}