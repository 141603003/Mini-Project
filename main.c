#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "question.h"
#include "print.h"


int main() {
	int i = 1, choice, select;
	char filename[20], *file, dir[100], more;
	int id = 0;
	char ch;
	QuestionBank qb;
	QuizSet quizset;
	initquizset(&quizset);
	Quiz* quiz = &(quizset.quiz);
	initial();
	choice = getchar();
	if(choice == 27)/*ESC 27*/{
		system("clear");
		exit(0);
	}
	initialization();
	
	choice = loginprint();
	validation(choice);
	while(1) {
		select = printmenu(choice);
		init(&qb);
		strcpy(dir, "./QuestionBank/");
		switch(select) {
			case CREATEQB:
				
				init(&qb);
				id = 0;
				do {/**for new question bank*/
					printheader("Quizzy");
					printmiddle("Creation of Question Bank");
					addQuestion(&qb, &id);
				
					printf("\nDo u want to enter more questions (y | n) :\n");
					scanf("%c", &more);
					getchar();
				}while(more != 'n');
				printheader("Quizzy");
				printf("Your Question Bank :\n");
				readQBstruct(&qb, 0);
				printf("\n(Press ENTER key)\n");
				getchar();
				printf("Save the question bank(Y | N):\n");
				ch = getchar();
				getchar();
				
				if(ch == 'Y') {/*storing*/
					printf("Enter a name for Question Bank:\n");
					scanf("%s", filename);
					getchar();
					strcat(filename, ".qb");
					strcpy(filename, strcat(dir, filename));
					storeQB(&qb, filename, 0);
				}

				break;

			case VIEWQB:
				printheader("Quizzy");
				printmiddle("View Question Banks");
				file = getfilename();
				if(file) {
					printheader("Quizzy");
					printmiddle("View Question Banks");
					printmiddle(file);
					readQB(file);
				}
				
				break;
			case CREATEQZ:
				init(quiz);
				printheader("Quizzy");
				printmiddle("Creation of Quiz");
				file = getfilename();
				readQBQuiz(&quizset, file);/*reads the qb and stores in quiz*/
				if(quizset.total_marks != 0){
					printheader("Quizzy");
					printf("Your Quiz :\n");
					readQBstruct(quiz, 1);/*show marks*/
					getchar();
					printf("Save the question bank(Y | N):\n");
					ch = getchar();
					getchar();
					printf("Enter the file name :\n");
					scanf("%s", filename);
					getchar();
					if(ch == 'Y') {
						strcat(filename, ".qb");
						strcpy(filename, strcat(dir, filename));
						storeQB(quiz, filename, 1);
					}
				}
				break;
		}
	}
	return 0;
}
