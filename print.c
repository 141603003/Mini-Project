
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "print.h"
int maxwidth;
int filefound;
/*creates question bank directory if not available*/
void initialization() {
	int found = 0;
	DIR *dp = NULL;
    	struct dirent *dptr = NULL;
	if((dp = opendir("./")) == NULL) {
		printf("\n No access of current directoy");
		exit(1);
   	}
	while((dptr = readdir(dp)) != NULL ) {
		printf("%s\n", dptr->d_name);
   		if(!strcmp("QuestionBank", dptr->d_name)) {
			found = 1;
			break;
		}
        }
	closedir(dp);
	if(!found) {
		mkdir("QuestionBank",S_IRWXU|S_IRWXG|S_IRWXO);
	}
	
}
void printheader(char* str) {
	int i, pos;
	system("clear");
	for(i = 0; i < maxwidth; i++)
		printf("=");
	printf("\n\n");
	pos = maxwidth / 2 - (strlen(str) / 2);
	for(i = 0; i < pos; i++) 
		printf(" ");
	printf("%s\n\n", str);
	for(i = 0; i < maxwidth; i++)
		printf("=");
	printf("\n");
	
	
}
void initial() {
	maxwidth = 1366 / 10;
	printheader("Welcome to Quizzy");
	printmiddle("Press ENTER to continue.......");
	printmiddle("Press ESC <ENTER> to exit.............");
}
void printmiddle(char* str) {
	int i, pos;
	pos = maxwidth / 2 - (strlen(str) / 2);
	for(i = 0; i < pos; i++) 
		printf(" ");
	printf("%s\n", str);
}

int loginprint() {
	char choice;
	system("clear");
	printheader("Quizzy");
	printmiddle("LOGIN PAGE");
	printmiddle("(Press ESC to exit any time)");
	printf("Select the type of Login :\n");
	printf("\n%-10s%20s\n", "1. ADMIN", "[1]");
	printf("%-10s%20s\n\n", "2. STUDENT", "[2]");
	printf("Please Enter your choice code : ");
	scanf("%c", &choice);
	if(choice == 27) {
		system("clear");
		exit(0);
	}
	else
		return (int)(choice - '0');
	
}

void validator() {
	int i = 0;
	char str[100];
	getchar();
	printf("Enter Admin password :\n");
	while(i < 3) {
		scanf("%s", str);
		getchar();/*for enter*/
		if(!strcmp("1234", str))
			return;
		else {
			printf("Wrong password\n");
			if(i == 1)
				printf("Last attempt:\n");
				
		}
		i++;
	}
	printf("Are you intruder?\n");
	getchar();
	system("clear");
	exit(1);
}

void validation(int l) {
	FILE* fp = NULL;
	char mis[20];
	if(l == ADMIN) {
		validator();
	}
	else {
		getchar();
		printf("Enter your MISID : ");
		scanf("%s", mis);
		fp = fopen(mis,"a");
		if(!fp) {
			printf("Problem with file");
			exit(1);
		}
		fclose(fp);
	}
}
int printmenu(int l) {
	char choice;
	
	system("clear");
	if(l == ADMIN) {
		printheader("Quizzy");
		printmiddle("ADMIN");
		printf("\n%-40s%5s\n","1. Create NEW Question Bank", "[1]");
		printf("%-40s%5s\n","2. View Available Question Bank", "[2]");
		printf("%-40s%5s\n","3. Create NEW Quiz", "[3]");
		printf("%-40s%5s\n","4. View Available Quiz", "[4]");
		printf("%-40s%5s\n","5. Export a QuestionBank", "[5]");
		printf("%-40s%5s\n\n","6. Import a QuestionBank", "[6]");
		printf("Enter your choice:");
		scanf("%c", &choice);
		getchar();
		if(choice == 27) {
			system("clear");
			exit(0);
		}
		else
			return (int)(choice - '0');
	}
	else if(l == STUDENT) {
		printheader("Quizzy");
		printmiddle("STUDENT");
		printf("\n%-40s%5s\n","1. Run a Quiz", "[1]");
		printf("%-40s%5s\n\n","2. View Pending Quiz", "[2]");
		printf("Enter your choice:");
		scanf("%c", &choice);
		getchar();
		if(choice == 27) {
			system("clear");
			exit(0);
		}
		else
			return (int)((choice - '0') + 7);
	}
}
void availablefilename(){
	DIR *dp = NULL;
	char filenames[100][20];
    	struct dirent *dptr = NULL;
	int len,i = 1;
	char *last_three;
	filefound = 0;
	
	printf("Available Question Banks in current directory: \n");
	
	if((dp = opendir("./QuestionBank/")) == NULL) {
		printf("\n No access of current directoy");
		getchar();
		return ;
   	}
	while((dptr = readdir(dp)) != NULL ) {
		len = strlen(dptr->d_name);
		if(len > 3){
			last_three = &dptr->d_name[len - 3];
			if(!strcmp(last_three, ".qb")) {
				strcpy(filenames[i - 1], dptr->d_name); 
				printf("%d. %s\n", i, dptr->d_name);
   				++i;
				filefound++;
			}
		}
        }
	if(i == 1)
		printf("No Question Bank available yet........\n");
	getchar();
	closedir(dp);
}

char* getfilename() {
	char file[100], *temp;
	char dir[100] = "./QuestionBank/";
	availablefilename();
	if(filefound != 0) {
		printf("Enter the question Bank name :\n");
		scanf("%s",file);
		getchar();
		strcat(file, ".qb");
		strcpy(file, strcat(dir, file));
		temp = (char*) malloc(sizeof(char) * strlen(file) + 1);
		strcpy(temp, file);
		return temp;
	}
	else 
		return NULL;
		
}
