#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include <unistd.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void listDir(){
    struct dirent *cwdirent;
    DIR *cwd;
    long size;
    char *directory;

    size = pathconf(".", _PC_PATH_MAX);

    if((directory = (char *)malloc((size_t)size)) != NULL){
        cwd = opendir(getcwd(directory, (size_t)size));
        while((cwdirent = readdir(cwd)) != NULL){
        if(strcmp(cwdirent->d_name, ".") && strcmp(cwdirent->d_name, "..")){
            write(1, cwdirent->d_name, strlen(cwdirent->d_name));
            write(1," ", 1);
            }
        }
        write(1, "\n", 1);
        closedir(cwd);
        free(directory);
    }else{
        char *error = "Could not allocate memory\n";
        write(1, error, strlen(error));
    }
}

void showCurrentDir(){
    long size;
    char *directory;

    size = pathconf(".", _PC_PATH_MAX);
    if((directory = (char *)malloc((size_t)size)) != NULL){
        write(1,getcwd(directory, (size_t)size), size);
        write(1,"\n",1);
    }
}

void makeDir(char *dirName){
    int c = mkdir(dirName, 0777);
    char* error = "Directory Name already in Use\n"; 
    if(c){
        write(1, error, strlen(error));
    }
}

void changeDir(char *dirName){
    if(chdir(dirName) == -1){
        char* error = "Invalid Directory\n";
        write(1, error, strlen(error));
    }
}

void copyFile(char *sourcePath, char *destinationPath){
    DIR *cwd;
    char path[_PC_PATH_MAX];
    cwd = opendir(destinationPath);
    if(cwd == NULL){
        char buffer[1024];
        int fd = open(destinationPath, O_WRONLY | O_CREAT, 0644);
        int ffr = open(sourcePath, O_RDONLY); 
        ssize_t c; 
        while ((c = read(ffr, buffer, sizeof(buffer))) != 0){
            write(fd, buffer, c);
        }
    }else{
        char buffer[1024];
        sprintf(path, "%s/%s", destinationPath, sourcePath);
        int fd = open(path, O_WRONLY | O_CREAT, 0644);
        int ffr = open(sourcePath, O_RDONLY);
        ssize_t c;
        while((c = read(ffr, buffer, sizeof(buffer))) != 0){
            write(fd, buffer, c);
        }
    }
   
}

void moveFile(char *sourcePath, char *destinationPath){
    struct dirent *cwdirent;
    DIR *cwd;
    char path[_PC_PATH_MAX];
    cwd = opendir(destinationPath);
    if(cwd == NULL){
        char buffer[1024];
        int fd = open(destinationPath, O_WRONLY | O_CREAT, 0644);
        int ffr = open(sourcePath, O_RDONLY); 
        ssize_t c; 
        while ((c = read(ffr, buffer, sizeof(buffer))) != 0){
            write(fd, buffer, c);
        }
    }else{
        char buffer[1024];
        sprintf(path, "%s/%s", destinationPath, sourcePath);
        int fd = open(path, O_WRONLY | O_CREAT, 0644);
        int ffr = open(sourcePath, O_RDONLY);
        ssize_t c;
        while((c = read(ffr, buffer, sizeof(buffer))) != 0){
            write(fd, buffer, c);
        }
    }
    unlink(sourcePath);
}

void deleteFile(char *filename){
    int c = unlink(filename);
    if(c == -1){
        char *buf = "Error - Invalid File\n";
        write(1, buf, strlen(buf));
    }
}

void displayFile(char *filename){
    char buffer[1024];
    int ffr = open(filename, O_RDONLY); 
    ssize_t c; 
    while ((c = read(ffr, buffer, sizeof(buffer))) != 0){
        write(1, buffer, c);
    }
    
}