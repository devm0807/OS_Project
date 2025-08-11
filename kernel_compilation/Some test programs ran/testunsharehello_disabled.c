#define _GNU_SOURCE
#include<sys/syscall.h>
#include<unistd.h>
#include<stdio.h>
#include<sched.h>
#include<linux/sched.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<stdbool.h>
#include<semaphore.h>
#include<unistd.h>
#include<time.h>
#include <sched.h>

int main(){
	int x = unshare(CLONE_NEWPID);
	fork();
	int retsys = syscall(548);
	printf("Hello world, Im process %d. My custom syscall status: %d.\n", getpid(), retsys);
	return 0;
}
