#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//function to print out current working directory
//As to appear more like a real command line tool
void PWD(){
	char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) != NULL){
		printf("%s$ ",cwd);}
	else{
		perror("getcwd() error");}
}
void Tokens(char *arr, int len_arr){
	int TokenCount = 1;
	//int len_arr = sizeof(arr);
	printf("len = %d\n", len_arr);
	for(int i=0; i < len_arr; i++){
		if (arr[i] == ' '){
			printf("\n");
			TokenCount++;
		}
		else{
		printf("%c",arr[i]);
		}
	}
	printf("\n%d token (s) read\n", TokenCount);
	
}

void ProgramLoop(){
	char str[256]; char *s;
	PWD();
	scanf("%[^\n$]%*c", str);
	printf("Orginal str: %s\n", str);
	s = strstr(str, "exit");
	if (s != NULL){	//check to see if exit command was passed
		exit(0);
	}
	printf("Line read: %s\n", str);
	Tokens(str, strlen(str));
	//printf("%s", str);
}

int main(){
	while(1){
	ProgramLoop();
	}
	return 0;
}

