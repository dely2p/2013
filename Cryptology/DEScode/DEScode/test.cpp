#include <stdio.h>
int main(){
	unsigned char b[5]={'a','b','c','d','e'};
	int i;
	for(i=0;i<20;i++)
		b[i]<<2;
	for(i=0;i<20;i++)
		printf("%c",b[i]);
}