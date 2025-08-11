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

int disable_fork(){
	return syscall(550);
}

int fork_if_not_disable(){
	return syscall(549);
}

int main(){
	printf("First custom fork call: %d\n",fork_if_not_disable());
	disable_fork();
	printf("Custom fork call after disable: %d\n", fork_if_not_disable());
	return 0;
}
