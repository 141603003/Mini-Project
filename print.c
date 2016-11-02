/**************************************************************************************
 * For displaying formatted text / output on screen and creation of initial directories.
 * Copyright (C) 2016 Vinay P. Desai desaivinay1997@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 *************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "print.h"

int maxwidth = 1366 / 10;
int filefound;
/*creates question bank directory if not available*/
void initialization() {
	int foundqb = 0, foundqz = 0, foundinfo = 0, foundexp = 0;/*boolean for found*/
	DIR *dp = NULL;
    	struct dirent *dptr = NULL;
	if((dp = opendir("./")) == NULL) {
		printf("\n No access to current directoy");
		exit(1);
   	}
	while((dptr = readdir(dp)) != NULL ) {
		//printf("%s\n", dptr->d_name);
   		if((foundqb == 0) && (!strcmp("QuestionBank", dptr->d_name)) ) {
			foundqb = 1;
		}
		if((foundqz == 0) && (!strcmp("Quiz", dptr->d_name)) ) {
			foundqz = 1;
		}
		if((foundinfo == 0) && (!strcmp("Info", dptr->d_name)) ) {
			foundinfo = 1;
		}
		if((foundexp == 0) && (!strcmp("Export_Files", dptr->d_name)) ) {
			foundexp = 1;
		}
		
        }
	closedir(dp);
	if(!foundqb) {
		mkdir("QuestionBank",S_IRWXU|S_IRWXG|S_IRWXO);
	}
	if(!foundqz) {
		mkdir("Quiz",S_IRWXU|S_IRWXG|S_IRWXO);
	}
	if(!foundinfo) {
		mkdir("Info",S_IRWXU|S_IRWXG|S_IRWXO);
	}
	if(!foundexp) {
		mkdir("Export_Files",S_IRWXU|S_IRWXG|S_IRWXO);
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
	printmiddle("Press ESC + <ENTER> to exit.............");
}
void printmiddle(char* str) {
	int i, pos;
	pos = maxwidth / 2 - (strlen(str) / 2);
	for(i = 0; i < pos; i++) 
		printf(" ");
	printf("%s\n", str);
}

int loginprint() {
	int choice;
	char temp[100];
	system("clear");
	printheader("Quizzy");
	printmiddle("LOGIN PAGE");
	printmiddle("(Press ESC to exit any time)");
	printf("Select the type of Login :\n");
	printf("\n%-10s%20s\n", "1. ADMIN", "[1]");
	printf("%-10s%20s\n\n", "2. STUDENT", "[2]");
	do {
		printf("Please Enter your choice code : \n");
		scanf("%s", temp);
		getchar();/*to remove enter*/
		if(temp[0] == 27) {
			system("clear");
			exit(0);
		}
		choice = atoi(temp);
		if((choice) < 1 || (choice) > 2) {
				printf("Wrong choice code entered\n");
		}
	}while((choice) < 1 || (choice) > 2);
	
	return (int)(choice);
	
}

void shuffle(question_ll* q, int n) {
	int i, j, k;
	question_ll temp;
	srand(time(NULL));
	getchar();
	if(n > 1) {
		for(i = 0; i < n -1; i++){
			j = i + rand() / (RAND_MAX / (n -i) + 1);
			if(j >= n || j < 0) {
				printf("There is problem with suffler right now\n Press ENTER\n");
				getchar();
			}
			temp = q[j];
			q[j] = q[i];
			q[i] = temp;
			
			k = q[j].q.id;
			
			q[j].q.id = q[i].q.id;
			q[i].q.id = k;
		}
	}
}

void shufflepairs(pair* p, int n){
	int i, j;
	char temp[20];
	//srand(time(NULL));
	if(n > 1) {
		for(i = 0; i < n -1; i++){
			j = i + rand() / (RAND_MAX / (n -i) + 1);
			strcpy(temp, p[j].str2);
			strcpy(p[j].str2, p[i].str2);
			strcpy(p[i].str2, temp);
			
		}
	}
}
void  validation(int l, char* si) {
	FILE* fp = NULL;
	char mis[20], file[100];
	if(l == STUDENT) {
		
		printf("Enter your MISID : ");
		scanf("%s", mis);
		getchar();
		strcpy(si, mis);
		strcpy(file, "./Quiz/");
		strcat(file, mis);
		strcat(file, ".state");
		fp = fopen(file,"a");
		if(!fp) {
			printf("Problem with file\n Press Enter\n");
			
			getchar();
			exit(1);
		}
		fclose(fp);
	}
	
}
int printmenu(int l) {
	int choice;
	char temp[100];
	system("clear");
	if(l == ADMIN) {
		printheader("Quizzy");
		printmiddle("ADMIN");
		printmiddle("(Press ESC + ENTER key to exit)");
		printf("\n%-40s%5s\n","1. Create NEW Question Bank", "[1]");
		printf("%-40s%5s\n","2. View Available Question Bank", "[2]");
		printf("%-40s%5s\n","3. Create NEW Quiz", "[3]");
		printf("%-40s%5s\n","4. View Available Quiz", "[4]");
		printf("%-40s%5s\n","5. Export a QuestionBank", "[5]");
		printf("%-40s%5s\n","6. Import a QuestionBank", "[6]");
		printf("%-40s%5s\n\n","7. Change login to STUDENT", "[7]");
		do{
			printf("Enter your choice code:\n");
			scanf("%s", temp);
			getchar();
			if(temp[0] == 27) {
				system("clear");
				exit(0);
			}
			choice = atoi(temp);
			if((choice) < 1 || (choice) > 7) {
				printf("Wrong choice code entered\n");
			}
		}while((choice) < 1 || (choice) > 7);
		
	
		return (int)(choice);
	}
	else if(l == STUDENT) {
		printheader("Quizzy");
		printmiddle("STUDENT");
		printmiddle("(Press ESC + ENTER key to exit)");
		printf("\n%-40s%5s\n","1. Run a Quiz", "[1]");
		printf("%-40s%5s\n","2. Resume Pending Quiz", "[2]");
		printf("%-40s%5s\n\n","3. Change login to ADMIN", "[3]");
		do{
			printf("Enter your choice code:\n");
			scanf("%s", temp);
			getchar();
			if(temp[0] == 27) {
				system("clear");
				exit(0);
			}
			choice = atoi(temp);
			if((choice) < 1 || (choice) > 3) {
				printf("Wrong choice code entered\n");
			}
		}while((choice) < 1 || (choice) > 3);
		
	
		return (int)(choice + 7);
	}
	else {
		return 0;/*to suppress the warning*/
	}
}

char* removeExtension(char* file) {
	char* filename;
	int i;
	for(i = strlen(file) - 1; i >= 0; i--) {
		if(file[i] == '.') {
			break;	
		}
	}
	if(i == -1){
		printf("Wrong file extension");
		exit(1);
	}
	filename = (char*)malloc(sizeof(char) * (strlen(file) + 1));
	strcpy(filename, file);
	filename[i] = '\0';
	return filename;
}


char*  getfilename(char* dirname, char* filetype){
	DIR *dp = NULL;
	char filenames[100][40], *temp;
	int cmp, lenofext;
    	struct dirent *dptr = NULL;
	int len,i = 1, choice;
	char *last_three;
	filefound = 0;
	lenofext = strlen(filetype);
	cmp = strcmp(filetype, ".qb");/*Compares with.qb   0->qb    1->quiz*/
	if(cmp == 0)
		printf("Available Question Banks in current directory: \n");
	else
		printf("Available Quiz Sets in current directory: \n");
		
	if((dp = opendir(dirname)) == NULL) {
		printf("\n No access of current directoy");
		getchar();
		return NULL;
   	}
	while((dptr = readdir(dp)) != NULL ) {
		len = strlen(dptr->d_name);
		if(len > lenofext){
			last_three = &dptr->d_name[len - lenofext];
			if(!strcmp(last_three, filetype)) {
				strcpy(filenames[i - 1], removeExtension(dptr->d_name)); 
				printf("%d. %s\n", i, filenames[i - 1]);
   				++i;
				filefound++;
			}
		}
        }
	if(i == 1) {
		if(cmp == 0)
			printf("No Question Bank available yet........\n");
		else
			printf("No Quiz Sets available yet........\n");
	}
	printf("\n(Press Enter key)\n");
	getchar();
	closedir(dp);
	if(filefound != 0) {
		do{
			printf("\nEnter your choice from above :\n");		
			scanf("%d", &choice);
			getchar();
			if(choice < 1 || choice > filefound) {
				printf("Wrong number entered\n");
			}
		}while(choice < 1 || choice > filefound);
		
		temp = (char*) malloc(sizeof(char) * (strlen(filenames[choice - 1]) + 1));
		strcpy(temp, filenames[choice -1]);
		return temp;
	}
	else 
		return NULL;
}



char* checkforterminate(char* str) {
	char *temp;
	if(str[0] == 20){
		return NULL;
	}
	temp = (char*) malloc(sizeof(char) * (strlen(str) + 1));
	strcpy(temp, str);
	return temp;
}

