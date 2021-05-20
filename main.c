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


typedef struct
{
    char** command_list;
    int num_token;
}command_line;



int count_token (char* buf, const char* delim)
{
	if(buf == NULL){
		return 0;
	}
	char *replace;
	char *token;
	int count = 0;
	replace = strdup(buf);
	while(token = strtok_r(replace, delim, &replace)){
		count += 1;
	}
	return count+1;
}

command_line str_filler (char* buf, const char* delim)
{
	//TODO：
	/*
	*	#1.	create command_line variable to be filled
	*	#2.	use function strtok_r to take out \n character then count
	*			number of tokens with count_token function, set num_token.
	*	#3. malloc memory for token array inside command_line variable
	*			base on the number of tokens.
	*	#4. malloc each index of the array with the length of tokens,
	*			fill array with tokens, and fill last spot with NULL.
	*	#5. return the variable.
	*/
	command_line *new = malloc(sizeof(command_line));

	buf[strcspn(buf, "\n")] = 0;
    int count = count_token(buf, delim);
    int c1 = 0;

    new->command_list = malloc(sizeof(char*)*count);
    new->num_token = count;

    char *replace;
    char *token;

    replace = strdup(buf);

    while(token = strtok_r(replace, delim, &replace)){
        new->command_list[c1] = malloc((strlen(token)+1)*sizeof(char));
        new->command_list[c1] = token;
        c1++;
    }
	new->command_list[count-1] = NULL;
    return *new;
}

void free_command_line(command_line* command)
{
	//TODO：
	/*
	*	#1.	free the array base num_token
	
	for(int i = 0; i < command->num_token; i++){
		free(command->command_list[i]);
	}
	*/
	free(command->command_list);
	command->num_token = 0;

}


int command_app(command_line small_token_buffer){
	if(!strcmp(small_token_buffer.command_list[0],"listDir")){
		if(small_token_buffer.num_token != 2){
			char* error = "Usage - listDir\n";
			write(1, error, strlen(error));
		}else{
			listDir();
		}
	}else if(!strcmp(small_token_buffer.command_list[0],"showCurrentDir")){
		//Call showCurrentDir
		showCurrentDir();
	}else if(!strcmp(small_token_buffer.command_list[0],"makeDir")){
		//Call makeDir
		if(small_token_buffer.num_token != 3){
			char* buf = "Usage - makeDir <filename>\n";
			write(1, buf, strlen(buf));
		}else{
			makeDir(small_token_buffer.command_list[1]);
		}	
	}else if(!strcmp(small_token_buffer.command_list[0],"changeDir")){
		//Call changeDir
		if(small_token_buffer.num_token != 3){
			char* buf = "Usage - changeDir <directory>\n";
			write(1, buf, strlen(buf));
		}else{
			changeDir(small_token_buffer.command_list[1]);
		}
	}else if(!strcmp(small_token_buffer.command_list[0],"copyFile")){
		//Call copyFile
		if(small_token_buffer.num_token != 4){
			char* error = "Usage - copyFile <file> <dir> // copyFile <file> <file\n";
			write(1, error, strlen(error));
		}else{
			copyFile(small_token_buffer.command_list[1], small_token_buffer.command_list[2]);
		}
	}else if(!strcmp(small_token_buffer.command_list[0],"moveFile")){
		//Call moveFile
		if(small_token_buffer.num_token != 4){
			char* error = "Usage - moveFile <file> <dir> // moveFile <file> <file>\n";
			write(1, error, strlen(error));
		}else{
			moveFile(small_token_buffer.command_list[1], small_token_buffer.command_list[2]);
		}
	}else if(!strcmp(small_token_buffer.command_list[0],"deleteFile")){
		//Call deleteFile
		if(small_token_buffer.num_token != 3){
			char* buf = "Usage - deleteFile <filename>\n";
			write(1, buf, strlen(buf));
		}else{
			deleteFile(small_token_buffer.command_list[1]);
		}
	}else if(!strcmp(small_token_buffer.command_list[0],"displayFile")){
		//Call displayFile
		if(small_token_buffer.num_token != 3){
			//Error
		}else{
			displayFile(small_token_buffer.command_list[1]);
		}	
	}else if(!strcmp(small_token_buffer.command_list[0],"exit")){
		//Exit shell
		char* buf = "Exiting Shell....\n";
		write(1, buf, strlen(buf));
		return 0;
	}else{
		char* error = "Unknown Command\n";
		write(1, error, strlen(error));
	}
	return 1;
}

int main(int argc, char const *argv[]){
    if(argc == 1){
		//Interactive shell mode

		size_t len = 0;
		char *lineptr;

		command_line large_token_buffer;
		command_line small_token_buffer;

		int com = 1;
		while(com == 1){
			printf(">>> ");
			getline(&lineptr, &len, stdin);
			large_token_buffer = str_filler(lineptr, ";");
			for(int i = 0; large_token_buffer.command_list[i] != NULL; i++){
				small_token_buffer = str_filler(large_token_buffer.command_list[i], " ");
				com = command_app(small_token_buffer);
				free_command_line(&small_token_buffer);
				memset(&small_token_buffer,0,0);
			}
			free_command_line(&large_token_buffer);
			memset(&large_token_buffer, 0, 0);
		}
		free(lineptr);
		

	}
	else if(argc == 3){
		if(!strcmp(argv[1], "-f")){
			FILE *fp;
			fp = fopen(argv[2], "r");
			size_t len = 0;
			char *lineptr;
			command_line large_token_buffer;
			command_line small_token_buffer;

			while(getline(&lineptr, &len, fp) != -1){
				large_token_buffer = str_filler(lineptr, ";");
				for(int i = 0; large_token_buffer.command_list[i] != NULL; i++){
					small_token_buffer = str_filler(large_token_buffer.command_list[i], " ");
					int c = command_app(small_token_buffer);
					if(c == 0){
						break;
					}
				}
			}
		}else{
			printf("Invalid Flag\n");
		}
	}
	else{
		printf("Usage - Interactive Mode: ./pseudo-shell, File Mode: ./pseudo-shell -f <FILE>\n");
	}
}

