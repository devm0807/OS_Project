#include<unistd.h>
#include<sys/syscall.h>
#include<stdio.h>

int main(){
	int id = syscall(548);
	printf("%ld\n", id);
	return 0;
}
