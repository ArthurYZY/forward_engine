#include <stdio.h>
#include <stdlib.h>

void fuc(char *str){
	printf("get\n");
}

void printIP(unsigned int ip){
	int off = 24;
	while(off){
		int tmp = ip >> off;
		printf("%01d.", tmp & 255);
		off -= 8;
	}
	printf("%01d\n", ip & 255);
}
int main(){
	printf("hh");
	printIP(-1);
	return 0;
}