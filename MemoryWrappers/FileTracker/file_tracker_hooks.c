/******************************************************
 * Project: Group 3 OS project
 * Description: File open and closing wrappers.
 * 
 * Author(s): Raadhes, Dev
 * Date: 16/Nov/2024 last editted data
 *
 ******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <stdarg.h>


// Structure to manage open files
typedef struct {
    int* fds;  		       // Array to store file descriptors
    int count;                 // Number of open files
    int MAX_OPEN_FILES;
} OpenFileManager;

// Define the global OpenFileManager instance
OpenFileManager global_file_manager = { .count = 0 , .MAX_OPEN_FILES = 0};

// Open a file and track the file descriptor
//Need to update this to support hooking
int open(const char* pathname, int flags, ...){
    printf("Tracked open\n");
    int (*orig_open)(const char* pathname, int flags, ...);
    int fd;

    orig_open = dlsym(RTLD_NEXT, "open");

    va_list args;
    va_start(args, flags);
    mode_t mode = va_arg(args, mode_t);

    fd = orig_open(pathname, flags, mode);

    if(fd == -1){
        perror("open failed");
        return -1;
    }
    
    if (global_file_manager.count == 0){
    	global_file_manager.fds = malloc(10 * sizeof(int));
    	global_file_manager.MAX_OPEN_FILES = 10;
    }
    else if (global_file_manager.count == global_file_manager.MAX_OPEN_FILES) {
        int* newfds = malloc(2 * global_file_manager.count);
        for(int i = 0; i < global_file_manager.count; i++){
      	    newfds[i] = global_file_manager.fds[i];
        }
        free(global_file_manager.fds);
        global_file_manager.fds = newfds;
    }

    global_file_manager.fds[global_file_manager.count++] = fd;
    return fd;
}

int close(int fd) {
    printf("Tracked close\n");
    int (*orig_close)(int fd);

    orig_close = dlsym(RTLD_NEXT, "close");

    for (int i = 0; i < global_file_manager.count; ++i) {
        if (global_file_manager.fds[i] == fd) {
            global_file_manager.fds[i] = global_file_manager.fds[global_file_manager.count - 1];  // Replace with the last file
            --global_file_manager.count;
            if (orig_close(fd) == -1) {
                perror("close failed");
                return -1;
            } else {
            	return 0;	
            }
        }
    }
    
    
    fprintf(stderr, "File descriptor not found in the file manager : %d.\n", fd);
    return -1;
}

void close_all_files(void) {
    for (int i = 0; i < global_file_manager.count; ++i) {
        close(global_file_manager.fds[i]);
    }
    global_file_manager.count = 0;  // Reset the file tracking list
}

int main() {
    int fd1 = open("file1.txt", O_CREAT | O_WRONLY, 0644);
    int fd2 = open("file2.txt", O_CREAT | O_WRONLY, 0644);
    
    close(fd2);
    close(100);
    close_all_files();

    return 0;
}


