#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

void parseCmd(char* cmd, char** params);
int executeCmd(char** params);


void pwd(){
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("%s$ ", cwd);
}

// Split command into array of parameters
int parse(char* cmd, char** params)
{   int total =0;
    for(int i = 0; i < 100; i++) {
        params[i] = strsep(&cmd, " ");
        total++;
        if(params[i] == NULL) break;
    }
    //sanity check
    //printf("Total args = %d\n", total);
    return total;
}

//change directory command
void CD(char destination[]){
	if(chdir(destination)==-1){
		char* error = strerror(errno);
		printf("%s\n", error);
	}
}

int execute(char** params)
{
    // Fork process
    pid_t pid = fork();

    // Error
    if (pid == -1) {
        char* error = strerror(errno);
        printf("%s\n", error);
        return 1;
    }

    // Child process
    else if (pid == 0) {
        // Execute command
        execvp(params[0], params);  

        // Error occurred
        char* error = strerror(errno);
        printf("shell: %s: %s\n", params[0], error);
        return 0;
    }

    // Parent process
    else {
        // Wait for child process to finish
        int childStatus;
        waitpid(pid, &childStatus, 0);
        return 1;
    }
}

int main()
{
    char command[100];
    char* params[10];
      
    while(1) {
    
    	FILE *outfile;
    	FILE *infile;
	
        //start by printing directory
        pwd();
        
        
        int limit=0;
        int exe=1;
        
        
        
        // Read command from standard input, exit on Ctrl+D
        if(fgets(command, sizeof(command), stdin) == NULL) break;

        // Remove trailing newline character, if any
        if(command[strlen(command)-1] == '\n') {
            command[strlen(command)-1] = '\0';
        }

        // Split cmd into array of parameters
        int ParamSize = parse(command, params);
        
        // apropriately sized paramaters
        char* TrueParams[ParamSize];
        
        
        //populating TrueParams
        for(int i=0; i<ParamSize-1; i++){
        	TrueParams[i] = params[i];
        	TrueParams[i+1] = NULL;
        }     
        
        //check for cd
        if(strcmp(TrueParams[0], "cd") ==0){
        	CD(TrueParams[1]);
        	exe=0;
        }
        
        //check for PWD
        if(strcmp(TrueParams[0], "pwd")==0){
        	pwd();
        	printf("\n");
        	exe=0;
        }

        // Exit?
        if(strcmp(TrueParams[0], "exit") == 0) break;
        
        //check for input output redirection
        for(int i=0; i<ParamSize-1; i++){
        	if(strcmp(TrueParams[i], ">") == 0){
        	
        	limit=i;
        	outfile = fopen(TrueParams[i+1], "w");
        	dup2(fileno(outfile), 1);
        	//dup2(fileno(outfile), STDERR_FILENO);
        	fclose(outfile);
        	
        	//break;
        	}
        	if(strcmp(TrueParams[i], "<") == 0){
        	
        	infile=fopen(TrueParams[i+1], "r");
        	dup2(fileno(infile), 0);
        	fclose(infile);
        	//break;
        	}
        
        
        }
	
        // Execute command
        //check if CD, or PWD has already been passed and executed
        if(exe){
        	//check if I/O redirection has been passed
        	//This one is if no I/O redirection has been passed
        	if(limit == 0){
        		if(execute(TrueParams) == 0) break;
        	}
        	//If I/O redirection has been passed
        	else{
        		int NullIndex;
        		char* backupParams[limit + 1]; //o --> limit -1 
        		for(int i=0; i<limit; i++){
        		backupParams[i] = TrueParams[i];
        		NullIndex=i+1;
        		//sanity check print
        		//printf("Param is %s", params[i]);	
        		}
        		backupParams[NullIndex] = NULL; // put NULL at index limit
        		if(execute(backupParams) == 0) break;
        	}
        }
    }

    return 0;
}


