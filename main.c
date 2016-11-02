/*****************************************************************************
 * Defines the main handler program of quizzy software.
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
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "question.h"
#include "print.h"
#include "commandline.h"
void usage();	

int main(int argc, char* argv[]) {
	int choice, select, vqbcounter = 0, vqcounter = 0;
	char filename[30], *file, temp[100], more;
	question_ll* questionsarr;
	StudentInfo studentinfo;
	int id = 0, no;
	char ch;
	QuestionBank qb;/*QuestionBank*/
	QuizSet quizset;/*Quiz + info*/
	CMLStates cmd;
	initCMLStates(&cmd);
	if(argc > 1) {/*Checking for options*/
		if(strcmp(argv[1],"-h") == 0) {
			usage();
			return 0;
		}
	}
	createStates(&cmd, argc, argv);/*Command line option accepted*/
	initquizset(&quizset);
	Quiz* quiz = &(quizset.quiz);/*only quiz(not info part)*/
	if(cmd.cmd == 0){
		initial();/*initial screen*/
		choice = getchar();
		if(choice == 27)/*ESC 27*/{
			system("clear");
			exit(0);
		}
	}
	initialization();/*Creates required directory*/
	if(cmd.cmd == 0) {
		choice = loginprint();/*Prints login menu*/
		validation(choice, studentinfo.ID);/*Asks for mis id*/
	}
	while(1) {
		if(cmd.cmd == 0) {
			select = printmenu(choice);/*prints menu and gives selected choice*/
		}
		else {
			select = cmd.optionarray[cmd.pointer];
			cmd.pointer++;
		}
		init(&qb);
		switch(select) {
			case CREATEQB:
				
				init(&qb);
				id = 0;
				do {/**for new question bank*/
					printheader("Quizzy");
					printmiddle("Creation of Question Bank");
					printmiddle("(Press ESC + ENTER to exit)");
					printf("\n");
					printmiddle("     Type of question                Choice of code");
					printf("\n");
					printmiddle("Multiple Choice Question  (MCQ)                [1]");
					printmiddle("Multiple Answer Question  (MAQ)                [2]");
					printmiddle("Numerical Answer Question (NAQ)                [3]");
					printmiddle("Match the pair            (MTP)                [4]");
					printmiddle("Write an Essay            (ESSAY)              [5]");
					printmiddle("True or False             (T or F)             [6]");
					printf("\n");
					addQuestion(&qb, &id);
				
					printf("\nDo u want to enter more questions (y | n) :\n");
					scanf("%c", &more);
					more = tolower(more);
					getchar();
				}while(more != 'n');
				printheader("Quizzy");
				printf("\nYour Question Bank :\n");
				readQBstruct(&qb, 0);
				printf("\n(Press ENTER key)\n");
				getchar();
				printf("Save the question bank(Y | N):\n");
				ch = getchar();
				getchar();
				ch = toupper(ch);
				if(ch == 'Y') {/*storing*/
					if(cmd.no_of_cqbfiles == 0) {
						printf("Enter a name for Question Bank:\n");
						scanf("%[^\n]s", filename);
						getchar();
					}
					else
						strcpy(filename, cmd.cqbfilename);
					strcpy(temp, "./QuestionBank/");
					strcat(temp, filename);
					strcat(temp, ".qb");
				
					storeQB(&qb, temp, 0);
				}
				freeQuestionLinkedList(qb.start);
				break;

			case VIEWQB:
				printheader("Quizzy");
				printmiddle("View Question Banks");
				if((cmd.no_of_vqbfiles == 0)) {
					file = getfilename("./QuestionBank/", ".qb");
				}
				else {
					file = (char*) malloc(sizeof(char) * (strlen(cmd.vqbfilename[vqbcounter]) + 1));
					strcpy(file, (cmd.vqbfilename[vqbcounter]));
					vqbcounter++;
				}
				if(file) {
					strcpy(temp, "./QuestionBank/");
					strcat(temp, file); strcat(temp, ".qb");
					printheader("Quizzy");
					printmiddle("View Question Banks");
					printmiddle(temp);
					readQB(temp, 0);/*0 for not reading marks*/
				}
				if(file)
					free(file);	
				
				break;
			case CREATEQZ:
				init(quiz);
				printheader("Quizzy");
				printmiddle("Creation of Quiz");
				if((cmd.no_of_cqfiles == 0)) {
				 	file = getfilename("./QuestionBank/", ".qb");
				}
				else {
					file = (char*) malloc(sizeof(char) * (strlen(cmd.cqfilename) + 1));
					strcpy(file, (cmd.cqfilename));
				}
				if(file == NULL)
					break;
				strcpy(temp, "./QuestionBank/");
				strcat(temp, file); strcat(temp, ".qb");
				readQBQuiz(&quizset, temp);/*reads the question bank and stores in quiz*/
				if(quizset.total_marks != 0){
					printheader("Quizzy");
					printf("Your Quiz :\n");
					readQBstruct(quiz, 1);/*show marks*/
					printf("Press ENTER\n");
					getchar();
					printf("\nSave the quiz set(Y | N):\n");
					ch = getchar();
					getchar();
					ch = toupper(ch);
					if(ch == 'Y') {
						printf("Enter a name for the quiz set :\n");
						scanf("%[^\n]s", filename);
						getchar();
						strcpy(temp, "./Quiz/");
						strcat(temp, filename); 
						strcat(temp, ".quiz");
						storeQB(quiz, temp, 1);/*Stores the quiz part in the file .quiz*/
						strcpy(temp, "./Info/");
						strcat(temp, filename); 
						strcat(temp, ".info");
						storeQuizInfo(&quizset, temp);
					}
				}
				if(file)
					free(file);
				freeQuestionLinkedList(quiz->start);
				break;
			case VIEWQZ:
				printheader("Quizzy");
				printmiddle("View Quiz Sets");
				if((cmd.no_of_vqfiles == 0)) {
					file = getfilename("./Quiz/", ".quiz");
				}
				else {
					file = (char*) malloc(sizeof(char) * (strlen(cmd.vqfilename[vqcounter]) + 1));
					strcpy(file, (cmd.vqfilename[vqcounter]));
					vqcounter++;
				}
				if(file == NULL)
					break;
				printheader("Quizzy");
				printmiddle("View Quiz Sets");
				printmiddle(file);
				readQuizSet(file);
				if(file)
					free(file);
				break;
			case EXPORT:
				if(cmd.spresent == 0) {
					printheader("Quizzy");
					printmiddle("Export Window");
					file = getfilename("./QuestionBank/", ".qb");
				}
				else {
					file = (char*) malloc(sizeof(char) * (strlen(cmd.source) + 1));
					strcpy(file, (cmd.source));
				
				}
				if(file == NULL)
					break;
				/*printf("Enter the question bank name to be exported:\n");
				scanf("%s", temp);
				getchar();*/
				export2(file, cmd.tpresent, cmd.target);
				if(file)
					free(file);
				break;
			case IMPORT:
				printheader("Quizzy");
				printmiddle("Import Window");
				if(cmd.cmd == 0) {
					printf("Enter the path for xml file name to be imported (with .xml extension):\n");
					scanf("%[^\n]s", temp);
					getchar();
				}
				else {
					strcpy(temp, (cmd.importfilename));
				}
				import2(temp);
				break;
			case RUNQZ :
				if(cmd.cmd == 1) {
					validation(2, studentinfo.ID);
				}
				
				if((cmd.no_of_runfiles == 0)) {
					printheader("Quizzy");
					printmiddle("List of Quiz Set");
					file = getfilename("./Quiz/", ".quiz");
				}
				else {
					file = (char*) malloc(sizeof(char) * (strlen(cmd.runfilename) + 1));
					strcpy(file, (cmd.runfilename));
		
				}
				if(file == NULL)
					break;
				/*Fetching the data from "file"*/
				questionsarr = NULL;
				strcpy(temp, "./Quiz/");
				strcat(temp, file);
				strcat(temp, ".quiz");
				fetchquizinfo(&studentinfo, file);
				questionsarr = fetchquiz(temp/*filename*/ );
				if(questionsarr == NULL) {
					break;
				}
				displayInstructions(studentinfo);
				printf("\n\n");
				printmiddle("PRESS ENTER 2 times to start Quiz");
				getchar();
				shuffle(questionsarr, studentinfo.totalques);

				if(runquiz(&studentinfo, questionsarr) == NULL) {//termination
					free(questionsarr);
					break;
				}
				if(questionsarr)
					free(questionsarr);
				if(file)
					free(file);
			
				break;
			case PENDINGQZ :
				if(cmd.cmd == 1) {
					strcpy(studentinfo.ID, cmd.ID);
				}
				printheader("Quizzy");
				printmiddle("Pending quiz set");
				
				no = getNoOfPendingQuiz(studentinfo);
				if(no == -1 || no == 0) {
					printf("\n\nThere are No quiz set pending for you.\nPress ENTER\n");
					getchar();
					break;
				}
				printf("\n\nNumber of quiz pending for you are : %d\n", no);
				if(cmd.cmd == 0) /*No command line but still resume*/
					cmd.r = 1;
				file = selectPendingQuiz(studentinfo, no, cmd.r);
				if(file == NULL && cmd.r == 0)
					break;
				modifyStudentInfo(&studentinfo, file);
				questionsarr = NULL;
				strcpy(temp, "./Quiz/");
				strcat(temp, studentinfo.filename);
				questionsarr = fetchquiz(temp);
				if(questionsarr == NULL)
					printf("Error Null");
				displayInstructions(studentinfo);
				printf("\n\n");
				printmiddle("PRESS ENTER to start Quiz");
				getchar();
				if(runquiz(&studentinfo, questionsarr) == NULL) {//termination
					free(questionsarr);
					break;
				}
				storeStudentInfo(&studentinfo);
				free(file);
				free(questionsarr);
				break;
			case CHANGETOSTUDENT :
				choice = 2;
				validation(choice, studentinfo.ID);
				break;
			case CHANGETOADMIN:
				choice = 1;
				break;
				
		}
		if(cmd.cmd){
			if(cmd.pointer == cmd.no_of_options){
				return 0;
			}
		}

	}
	return 0;
}
/*Prints usage with the help  of quizzy.help file*/
void usage() {
	FILE* fp;
	char ch;
	fp = fopen("quizzy.help", "r");
	if(fp == NULL) {
		printf("Help file is missing\n");
		return;
	}
	
	while((fread(&ch, sizeof(char), 1, fp)) != 0) {
		printf("%c", ch);
	}
	fclose(fp);	
}

