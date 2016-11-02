/********************************************************************************
 * Defines the operations on quizzy files, i.e. creation, loading, suspending,etc.
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
#include <errno.h>
#include <time.h>
#include <mxml.h>
#include "question.h"
#include "print.h"

void init(QuestionBank* qb) {
	qb->start = NULL;
	qb->curr = NULL;
	qb->no_of_question = 0;
}

void initquizset(QuizSet* qz) {
	qz->total_marks = 0;
	qz->time = 0;
}

void addQuestion(QuestionBank* qb, int* id) {
	int t, no, i = 0, j = 0;
	char ch, input[100];
	question_ll* node = (question_ll*)malloc(sizeof(question_ll));
	printf("Q%d.\n", (*id) + 1);
	do{/*Input validation*/
			printf("Enter the choice code for the type of Question:\n");
			scanf("%s", input);
			getchar();
			if(input[0] == 27) {
				system("clear");
				exit(0);
			}
			t = atoi(input);
			if((t) < 1 || (t) > 6) {
				printf("Wrong choice code for question type entered\n");
			}
	}while(t < 1 || t > 6);
	(node->q).type_of_ques = t;/*Question type*/
	(node->q).id = ++(*id);/*Question ID*/
	switch(t) {
		case _MCQ:/*MCQ*/
		case _MAQ:
			printf("\nEnter the question text :\n");
			
			scanf("%[^\n]%*c", (node->q).ques.mcq.qs.ques);/*Reading question*/
			(node->q).ques.mcq.qs.q_length = strlen((node->q).ques.mcq.qs.ques);/*storing length of question*/
			printf("\nHow many options: \n");
			scanf("%d", &no);
			(node->q).ques.mcq.no_of_opt = no;/*No of options*/
			option_ptr opt_ptr;/*option list*/
			opt_ptr.start = NULL;
			printf("\nLabels\n");
			for(i = 0; i < no; i++) {
				printf("%6c : ", (i+1+64));
				addOption(&opt_ptr);			
			}
			(node->q).ques.mcq.opts = opt_ptr.start;/*adding option*/
			getchar();

			/*new question created*/ 
			if(t == _MAQ) {
				(node->q).ques.maq.ans = (char*)malloc(sizeof(char) * no + 1);
				printf("\nCorrect option(label)(separated with ,) : \n");
				j = 0;
				while((ch = getchar()) != '\n' ) {
					if(ch != ',' && ch != ' ' && j < no) {
						ch = toupper(ch);
						if((ch - 'A') >= no || (ch - 'A') < 0) {
							printf("Wrong label: %c   discarded\n", ch);
							
						}
						else {
							(node->q).ques.maq.ans[j] = ch;
						 	j++;
						}
						
					}					
				}
				(node->q).ques.maq.ans[j] = '\0';
				
			}
			else {/*for MCQ*/
				do{
					printf("\nCorrect option(label) : \n");
					scanf("%c", &ch);/*Answer*/
					getchar();
					ch = toupper(ch);
					if((ch - 'A') >= no || (ch - 'A') < 0) {
						printf("Please select a valid label\n");
					}
				}while((ch - 'A') >= no || (ch - 'A') < 0);	
				 ((node->q).ques.mcq.ans) = ch;	
			}
				
			break;
		case _NAQ:
			printf("\nEnter the question string:\n");
			scanf("%[^\n]%*c", (node->q).ques.naq.qs.ques);/*Reading question*/
			(node->q).ques.naq.qs.q_length = strlen((node->q).ques.naq.qs.ques);/*storing length of question*/
			printf("\nEnter correct answer :\n");
			scanf("%lf", &((node->q).ques.naq.ans));
			getchar();
			break;
		case _MTP:
			printf("How many pairs:");
			scanf("%d", &no);
			getchar();
			if(no != 0){
				printf("\nNOTE : Enter the option + ENTER followed by corresponding answer\n\n");
			}
			(node->q).ques.mtp.no_of_pairs = no;
			(node->q).ques.mtp.pairs = NULL;
			for(i = 0; i < no; i++) {
				printf("Pair %c :", (i+1+64));
				addPairs(&((node->q).ques.mtp));			
			}
			break;
		case _ESSAY:
			printf("\nEnter the title of Essay:\n");
			scanf("%[^\n]s", (node->q).ques.essay.qs.ques);
			getchar();
			printf("\nAny specific information to user while writing essay:\n");
			scanf("%[^\n]s", (node->q).ques.essay.graderinfo);
			getchar();
			break;
		case _TF:
			printf("\nEnter a Statement:\n");
			scanf("%[^\n]s", (node->q).ques.truefalse.qs.ques);
			getchar();
			printf("\nTrue or False (t | f):\n");
			scanf("%c", &ch);
			getchar();
			ch = tolower(ch);
			if(ch == 't') {
				(node->q).ques.truefalse.ans = 1;
				printf("\nEnter feedback when \"false\" is entered :\n");
			}
			else{
				(node->q).ques.truefalse.ans = 0;
				printf("\nEnter feedback when \"true\" is entered :\n");
			}
			scanf("%[^\n]s", (node->q).ques.truefalse.stmt);
			getchar();
			 
	}
				
	node->next = NULL;
	/*Adding the new node*/
	if(qb->start == NULL) {/*List empty*/
		qb->start = node;/*Points to sam enode*/	
	}
	else {
		qb->curr->next = node;		
	}
	qb->curr = node;
	qb->no_of_question = *id;	
}
void addPairs(MTP* mtp) {
	char str1[50], str2[50];
	pair* node = malloc(sizeof(pair));
	pair* p;
	scanf("%[^\n]s", str1);
	getchar();
	printf("Answer :");
	scanf("%[^\n]s", str2);
	getchar();
	strcpy(node->str1, str1);
	strcpy(node->str2, str2);
	node->next = NULL;
	pair* start = mtp->pairs;
	if(start == NULL) {/*List empty*/
		start = node;	
	}
	else {
		p = start;
		while((p)->next != NULL) {
			(p) = (p)->next;		
		}
		(p)->next = node;
		
	}
	mtp->pairs = start;
}
void addOption(option_ptr* opt_ptr) {
	option* start = opt_ptr->start;
	char str[50];
	option* p;
	getchar();
	scanf("%[^\n]s", str);/*asking option*/
	option* node = (option*) malloc(sizeof(option));
	strcpy(node->opt_str, str); /*option string*/
	node->opt_length = strlen(node->opt_str);/*length*/
	node->next = NULL;
	if(start == NULL) {/*List empty*/
		start = node;	
	}
	else {
		p = start;
		while((p)->next != NULL) {
			(p) = (p)->next;		
		}
		(p)->next = node;
		
	}
	opt_ptr->start = start;
	
}

void freeQuestionLinkedList(question_ll* start) {/*Frees the question linked list recursively*/
	question_ll* temp;
	if(start == NULL) {
		return;
	}
	temp = start;
	start = start->next;
	freeMCQ_MAQ(&(temp->q));
	free(temp);
	freeQuestionLinkedList(start);
}
void freeMCQ_MAQ(Question* q) {/*Depending on the type of question frees memory*/

	if(q->type_of_ques == _MCQ) {
		freeOptions((q->ques).mcq.opts);	
	}
	else if(q->type_of_ques == _MAQ) {
		if((q->ques).maq.ans) {
			free((q->ques).maq.ans);
		}
		freeOptions((q->ques).maq.opts);
	}
	else if(q->type_of_ques == _MTP) {
		freePairs((q->ques).mtp.pairs);
	}

}
void freeOptions(option* start) {/*Frees the option linked list*/
	option* temp;
	if(start == NULL) {
		return;
	}
	temp = start;
	start= start->next;
	free(temp);
	freeOptions(start);
}
void freePairs(pair* start) {/*Frees the pairs linked list*/
	pair* temp;
	if(start == NULL) {
		return;
	}
	temp = start;
	start = start->next;
	free(temp);
	freePairs(start);
}

void readQBstruct(QuestionBank* qb, int showMarks/*boolean flag*/) {
	MCQ mcq;
	MAQ maq;
	NAQ naq;
	MTP mtp;
	pair* pair;
	questionString qs;
	option* opts;
	question_ll* p = qb->start;
	int i =0;
	while(p != NULL) {
		printf(" Q%d: ",(p->q).id);
		switch((p->q).type_of_ques) {
			case _MCQ:
				
				mcq = (p->q).ques.mcq;
				qs = mcq.qs;
				printf("%s%35s\n", qs.ques, "(MCQ)");
			
				if(!mcq.opts)
					printf("Options are not there");
				opts = mcq.opts;
				i =0;
				while(opts != NULL) {
					printf(" %c.%-25s\t", (char)(i + 'A'), opts->opt_str);
					opts = opts->next;
					i++;				
				}
				printf("\nCorrect ans: %c\n\n", mcq.ans);
				break;
			case _MAQ:
				
				maq = (p->q).ques.maq;
				qs = maq.qs;
				printf("%s%35s\n", qs.ques, "(MAQ)");
				
				opts = maq.opts;
				if(!opts)
					printf("Options are not there");
				i = 0;
				while(opts != NULL) {
					printf("%c.%s\t", (i + 'A'), opts->opt_str);
					opts = opts->next;
					i++;				
				}
				printf("\nCorrect ans: %s\n\n", maq.ans);
				break;
			case _NAQ:
			
				naq = (p->q).ques.naq;
				qs = naq.qs;
				printf("%s%35s\n", qs.ques, "(Numerical)");
				printf("Correct ans: %lf\n\n", naq.ans);
				
				break;	
			case _MTP:
				printf("Match the pairs:%35s\n", "(Matching)");
			
				mtp = (p->q).ques.mtp;
				printf("No of pairs: %d\n", mtp.no_of_pairs);
				pair = mtp.pairs;
				
				while(pair != NULL) {
					printf("%-25s\t%-25s\n", pair->str1, pair->str2);
					pair = pair->next;				
				}
				printf("\n");
				break;
			case _ESSAY:
				printf("%s%35s\n", (p->q).ques.essay.qs.ques, "(ESSAY)");
				printf("Grader Information: %s\n\n", (p->q).ques.essay.graderinfo);
				break;	
			case _TF:
				printf("%s%35s\n", (p->q).ques.truefalse.qs.ques, "(True or False)");
				if((p->q).ques.truefalse.ans){
					printf(" Answer: True\n");
					printf(" Feedback if false entered: %s\n\n", (p->q).ques.truefalse.stmt);
				}
				else {
					printf(" Answer: False\n");
					printf(" Feedback if true entered: %s\n\n", (p->q).ques.truefalse.stmt);
				}
		}
		if(showMarks) {
			printf("%50s......Marks[%d]\n", "", p->marks);		
		}
		p = p->next;
		
	}	
}
void storeQB(QuestionBank* qb, char* file, int storemarks) {/*storemarks indicate*/
	FILE* fp = fopen(file, "w");
	MCQ mcq;
	MAQ maq;
	NAQ naq;
	MTP mtp;
	pair* pairs;
	questionString qs;
	option* opts;
	question_ll* p = qb->start;
	if(!fp) {
		perror("Cant open file ");
		return;
	}
	if(!qb) {
		printf("List was empty");
	}
	fwrite(&(qb->no_of_question), sizeof(int), 1, fp);/*no of ques*/
	while(p != NULL) {
		fwrite(&((p->q).id), sizeof(int), 1, fp);
		fwrite(&((p->q).type_of_ques), sizeof(int), 1, fp);
		switch((p->q).type_of_ques) {
			case _MCQ: 
				mcq = (p->q).ques.mcq;
				qs = mcq.qs;
				fwrite(&qs,sizeof(struct questionString), 1, fp);/*Storing questionString structure*/
				opts = mcq.opts;
			
				fwrite(&(mcq.no_of_opt), sizeof(int), 1, fp);/*no of options*/
				while(opts != NULL) {
					fwrite(&(opts->opt_str), sizeof(char) * 50, 1, fp);/*Storing options*/
					opts = opts->next;				
				}
				fwrite(&mcq.ans, sizeof(char), 1, fp);
				break;
			case _MAQ:
				maq = (p->q).ques.maq;
				qs = maq.qs;
				fwrite(&qs,sizeof(struct questionString), 1, fp);/*Storing questionString structure*/
				opts = maq.opts;
			
				fwrite(&(maq.no_of_opt), sizeof(int), 1, fp);
				while(opts != NULL) {
					fwrite(&(opts->opt_str), sizeof(char) * 50, 1, fp);
					opts = opts->next;				
				}
				fwrite(maq.ans, sizeof(char), strlen(maq.ans) + 1, fp);/*storing string with \0*/
				break;
			case _NAQ:
				naq = (p->q).ques.naq;
				fwrite(&naq, sizeof(NAQ), 1, fp);/*Storing NAQ structure*/
				break;
			case _MTP:
				mtp = (p->q).ques.mtp;
				pairs = mtp.pairs;
			
				fwrite(&(mtp.no_of_pairs), sizeof(int), 1, fp);/*No of pairs*/
				while(pairs != NULL) {
					fwrite(pairs, sizeof(pair), 1, fp);
					pairs = pairs->next;				
				}
				break;
			case _ESSAY:
				fwrite(&((p->q).ques.essay), sizeof(ESSAY), 1, fp);
				break;
			case _TF:
				fwrite(&((p->q).ques.truefalse), sizeof(TF), 1, fp);
				break;
						
		}
		if(storemarks == 1) {
			fwrite(&(p->marks), sizeof(int), 1, fp);
		}
		p = p->next;
	}
	printf("Written successfully to %s\n", file);
	printf("Press ENTER\n");
	getchar();
	fclose(fp);
	
}

void storeQuizInfo(QuizSet* qs, char* file) {
	FILE* fp = fopen(file, "w");
	if(fp == NULL) {
		perror("Cannot create a quizinfo file" );
		return;
	}
	if(qs == NULL)
		return;
	fwrite(&(qs->total_marks), sizeof(int), 1, fp);/*total marks*/
	fwrite(&(qs->time), sizeof(int), 1, fp);/*time*/
	fwrite(&((qs->quiz).no_of_question), sizeof(int), 1, fp);/*no of qs*/
	fclose(fp);
}

/*Reads the data from file as question Bank*/
void readQB(char* file_name, int readmarks) {/*readmarks indicate whether to read marks or not*/
	FILE* fp = fopen(file_name, "r");
	pair* p;
	int i, j, k = 0, marks = 0;
	questionString qs;
	NAQ naq;
	ESSAY essay;
	TF tf;
	char str[50], ch;
	if(!fp) {
		perror("There is no question bank or quiz set with specified name");
		printf("\nPress ENTER\n");
		getchar();
		return;
	}
	
	fread(&i, sizeof(int), 1, fp);/*no of ques*/
	printf("%s contains %d questions\n\n", file_name, i);
	while(fread(&i, sizeof(int), 1, fp) != 0) {/*question ID*/
		printf(" Q%d. ", i);
		fread(&i, sizeof(int), 1, fp);/*ques type*/
		switch(i) {
			case _MCQ: 
				fread(&qs,sizeof(struct questionString), 1, fp);/*Storing questionString structure*/
				printf(" %s%20s\n", qs.ques, "(Mutichoice)");
				fread(&i, sizeof(int), 1, fp);/*no of options*/
				j = 0;
				while(j < i) {

					fread(&str, sizeof(char) , 50, fp);/*Reading options*/
					printf(" %c. %s\t", (j + 1 + 64), str);	
					j++;
							
				}
				fread(&ch, sizeof(char), 1, fp);
				
				break;
			case _MAQ:
				fread(&qs,sizeof(struct questionString), 1, fp);/*Storing questionString structure*/
			
				printf(" %s%30s\n", qs.ques, "(Multiple-Answer)");
				
				fread(&k, sizeof(int), 1, fp);/*no of options*/
				j = 0;
				while(j < k) {

					fread(&str, sizeof(char) , 50, fp);/*Reading options*/
					printf(" %c. %s\t", (j + 1 + 64), str);	
					j++;
							
				}
				j = 0;
				do {
					fread(&ch, sizeof(char), 1, fp);/*reading ans till '\0'*/
					str[j] = ch;
					j++;
				}while(ch != '\0');
				/*printf("\n%s\n", str);	*/			
				break;
			case _NAQ:
				fread(&naq, sizeof(NAQ), 1, fp);
				qs = naq.qs;
				printf(" %s%20s\n", qs.ques, "(Numerical)");
				break;	
			case _MTP:
				fread(&i, sizeof(int), 1,fp);
				printf("Match the following pairs:\n");
				p = (pair*)malloc(sizeof(pair));
				if(p == NULL) printf("Memory not allocated");
				for(j = 0; j < i; j++) {
					printf(" %c:", (j+1+64));
					fread(p, sizeof(pair), 1, fp);
					printf("%s\t%s\n", p->str1, p->str2);			
				}
				free(p);
				break;
			case _ESSAY:
				fread(&essay, sizeof(ESSAY), 1, fp);
				qs = essay.qs;
				printf(" %s%20s\n", qs.ques, "(ESSAY)");
				printf("Specific information: %s", essay.graderinfo);
				break;	
			case _TF:
				fread(&tf, sizeof(TF), 1, fp);
				qs = tf.qs;
				printf(" %s%30s\n", qs.ques, "(True False)");
				break;	
		}
		if(readmarks == 1){
			fread(&marks, sizeof(int), 1, fp);
			printf("\n%50s......Marks[%d]\n", "",marks);
		}
		printf("\n\n");
	}
	printf("Press ENTER\n");
	getchar();
	fclose(fp);
}

void addOptionQuiz(option_ptr* opt_ptr, char* str) {
	option* start = opt_ptr->start;
	option* p;
	option* node = (option*) malloc(sizeof(option));
	strcpy(node->opt_str, str); /*option string*/
	node->opt_length = strlen(node->opt_str);/*length*/
	node->next = NULL;
	if(start == NULL) {/*List empty*/
		start = node;	
	}
	else {
		p = start;
		while((p)->next != NULL) {
			(p) = (p)->next;		
		}
		(p)->next = node;
		
	}
	opt_ptr->start = start;
	
}

void addPairsQuiz(MTP* mtp, pair* p) {
	pair* node = malloc(sizeof(pair));
	strcpy(node->str1, p->str1);
	strcpy(node->str2, p->str2);
	node->next = NULL;
	pair* start = mtp->pairs;
	if(start == NULL) {/*List empty*/
		start = node;	
	}
	else {
		p = start;
		while((p)->next != NULL) {
			(p) = (p)->next;		
		}
		(p)->next = node;
		
	}
	mtp->pairs = start;
}
void readQBQuiz(QuizSet* quizset, char* file_name) {
	FILE* fp = fopen(file_name, "r");
	pair* p;
	Quiz* quiz = &(quizset->quiz);
	int i, j, countq = 0/*Question counter*/, marks, totalmarks = 0;
	questionString qs;
	option_ptr opt_ptr;
	NAQ naq;
	ESSAY essay;
	TF tf;
	char str[50], ch, select;
	if(!fp) {
		perror("No such Question Bank available ");
		return;
	}
	fread(&i, sizeof(int), 1, fp);/*no of ques*/
	printf("%s contains %d questions\n\n", file_name, i);
	printf("Press ENTER\n");
	getchar();
	while(fread(&i, sizeof(int), 1, fp)) {/*question ID*/
		printheader("Quizzy");
		printmiddle("Creation of Quiz");
		printmiddle(file_name);
		printf(" Q%d. ",i);
		question_ll* node = (question_ll*)malloc(sizeof(question_ll));
		(node->q).id = (countq + 1);/*Question ID*/
		fread(&i, sizeof(int), 1, fp);/*ques type*/
		(node->q).type_of_ques = i;/*Question type*/
		switch(i) {
			case _MCQ: 
				fread(&qs,sizeof(struct questionString), 1, fp);/*Reading questionString structure*/
				printf("%s%30s\n", qs.ques, "(Multichoice)");
				(node->q).ques.mcq.qs = qs;
				fread(&i, sizeof(int), 1, fp);/*no of options*/
				(node->q).ques.mcq.no_of_opt = i;/*No of options*/
				/*option list*/
				opt_ptr.start = NULL;
				j = 0;
				while(j < i) {

					fread(&str, sizeof(char) , 50, fp);/*Reading options*/
					printf(" %c. %s\n", (j + 1 + 64), str);	
					addOptionQuiz(&opt_ptr, str);
					j++;
							
				}
				(node->q).ques.mcq.opts = opt_ptr.start;/*adding option*/
				fread(&ch, sizeof(char), 1, fp);
				(node->q).ques.mcq.ans = ch;/*Answer*/
				break;
			case _MAQ:
				fread(&qs,sizeof(struct questionString), 1, fp);/*Reading questionString structure*/
				printf("%s%30s\n", qs.ques, "(Multiple Answer)");
				(node->q).ques.mcq.qs = qs;
				fread(&i, sizeof(int), 1, fp);/*no of options*/
				(node->q).ques.mcq.no_of_opt = i;/*No of options*/
				option_ptr opt_ptr;/*option list*/
				opt_ptr.start = NULL;
				j = 0;
				while(j < i) {

					fread(&str, sizeof(char) , 50, fp);/*Reading options*/
					printf(" %c. %s\n", (j + 1 + 64), str);	
					addOptionQuiz(&opt_ptr, str);
					j++;
							
				}
				(node->q).ques.mcq.opts = opt_ptr.start;/*adding option*/
				j = 0;
				do {
					fread(&ch, sizeof(char), 1, fp);/*reading ans till '\0'*/
					str[j] = ch;
					j++;
				}while(ch != '\0');
				(node->q).ques.maq.ans = (char*)malloc(sizeof(char) * i + 1);/*i contains number of options*/
				strcpy((node->q).ques.maq.ans, str);			
				break;
			case _NAQ:
				fread(&naq, sizeof(NAQ), 1, fp);
				qs = naq.qs;
				printf("%s%25s\n", qs.ques, "(Numerical)");
				(node->q).ques.naq = naq;
				break;	
			case _MTP:
				fread(&i, sizeof(int), 1,fp);/*no of pairs*/
				(node->q).ques.mtp.no_of_pairs = i;
				(node->q).ques.mtp.pairs = NULL;
				printf("Match the following pairs:\n");
				p = (pair*)malloc(sizeof(pair));
				for(j = 0; j < i; j++) {
					printf(" %c:", (j+1+64));
					fread(p, sizeof(pair), 1, fp);
					printf("%s%25s\n", p->str1, p->str2);
					addPairsQuiz(&((node->q).ques.mtp), p); /*Adding pairs*/
								
				}
				break;	
			case _ESSAY:
				fread(&essay, sizeof(ESSAY), 1, fp);
				qs = essay.qs;
				printf("%s%25s\n", qs.ques, "(Essay)");
				(node->q).ques.essay = essay;
				break;
			case _TF:
				fread(&tf, sizeof(TF), 1, fp);
				qs = tf.qs;
				printf("%s%25s\n", qs.ques, "(True or false)");
				(node->q).ques.truefalse = tf;
				break;
		}
		printf("\n\n");
		printf("Select above question: (y|n) \n");
		scanf("%c", &select);
		getchar();
		select = tolower(select);
		if(select == 'y') {
			printf("Allocate Marks:\n");
			scanf("%d", &marks);
			totalmarks = totalmarks + marks;
			getchar();
			(node->marks) = marks;
			node->next = NULL;
			/*Adding the new node*/
			if(quiz->start == NULL) {/*List empty*/
				quiz->start = node;/*Points to sam enode*/	
			}
			else {
				quiz->curr->next = node;		
			}
			quiz->curr = node;
			quiz->no_of_question = (++countq);
		}
		else {/*Node not needed*/
			free(node);
		
		}
	}
	quizset->total_marks = totalmarks;
	if(totalmarks) {
		printf("\nAllocate Max Time for this quiz:\n(minutes)");
		scanf("%d", &(quizset->time));
		printf("\nPress ENTER\n");
		getchar();
	}
	fclose(fp);
}

void readquizinfo(char* file) {
	FILE* fp = fopen(file, "r");
	int y;
	if(!fp) {
		perror("There is no Information file present for specified quiz set name");
		printf("(Press Enter)\n");
		getchar();
		return;
	}
	printf("\nInformation of Quiz Set:\n");
	printf("\t%-15s %10s...", "Total Marks","");
	fread(&y, sizeof(int), 1, fp);
	printf("%d\n", y);
	printf("\t%-15s %10s...", "Time Allocated", "");
	
	fread(&y, sizeof(int), 1, fp);
	printf("%d min\n", y);
	printf("\t%-15s %10s...", "No. of questions", "");
	fread(&y, sizeof(int), 1, fp);
	printf("%d \n\n", y);
}
void readQuizSet(char* file) {
	char quizfile[100], infofile[100];
	strcpy(quizfile, "Quiz Set name : ");
	strcat(quizfile, file);
	printmiddle(quizfile);
	strcpy(quizfile, "./Quiz/");
	strcat(quizfile, file);
	strcat(quizfile, ".quiz");
	strcpy(infofile, "./Info/");
	strcat(infofile, file);
	strcat(infofile, ".info");
	
	readquizinfo(infofile);
	readQB(quizfile, 1);/*1 to indicate to read marks also*/
}
/*Fetches only question from quizset*/
question_ll* fetchquiz( char* filename){
	int no_of_ques, i, id, type, temp, j;
	option_ptr opt_ptr;
	questionString qs;
	question_ll* questionarr;
	pair* p;
	NAQ naq;
	ESSAY essay;
	TF tf;
	char str[50], ans;
	FILE* fp = fopen(filename, "r");
	if(!fp) {
		perror("There is no Quiz set available with specified name");
		printf("Press Enter\n");
		getchar();
		return NULL;
	}
	fread(&no_of_ques, sizeof(int), 1, fp);/*no of ques*/
	questionarr = NULL;
	questionarr = (question_ll*)malloc(sizeof(question_ll) * no_of_ques);/*Array created here*/
	for(i = 0; i < no_of_ques; i++) {
		fread(&id, sizeof(int), 1, fp);
		questionarr[i].q.id = id;/*ID*/
		fread(&type, sizeof(int), 1, fp);/*ques type*/
		questionarr[i].q.type_of_ques = type;/*Question type*/
		switch(type) {
			case _MCQ: 
				fread(&qs,sizeof(struct questionString), 1, fp);/*Reading questionString structure*/
				questionarr[i].q.ques.mcq.qs = qs;
				fread(&temp, sizeof(int), 1, fp);/*no of options*/
				questionarr[i].q.ques.mcq.no_of_opt = temp;/*No of options*/
				/*option list*/
				opt_ptr.start = NULL;
				j = 0;
				while(j < temp) {

					fread(&str, sizeof(char) , 50, fp);/*Reading options*/
					addOptionQuiz(&opt_ptr, str);
					j++;
							
				}
				questionarr[i].q.ques.mcq.opts = opt_ptr.start;/*adding option*/
				fread(&ans, sizeof(char), 1, fp);
				questionarr[i].q.ques.mcq.ans = ans;/*Answer*/
				break;
			case _MAQ:
				fread(&qs,sizeof(struct questionString), 1, fp);/*Reading questionString structure*/
				questionarr[i].q.ques.maq.qs = qs;
				fread(&temp, sizeof(int), 1, fp);/*no of options*/
				questionarr[i].q.ques.maq.no_of_opt = temp;/*No of options*/
				/*option list*/
				opt_ptr.start = NULL;
				j = 0;
				while(j < temp) {

					fread(&str, sizeof(char) , 50, fp);/*Reading options*/	
					addOptionQuiz(&opt_ptr, str);
					j++;
							
				}
				questionarr[i].q.ques.maq.opts = opt_ptr.start;/*adding option*/
				j = 0;
				do {
					fread(&ans, sizeof(char), 1, fp);/*reading ans till '\0'*/
					str[j] = ans;
					j++;
				}while(ans != '\0');
				questionarr[i].q.ques.maq.ans = (char*)malloc(sizeof(char) * i + 1);/*i contains number of options*/
				strcpy(questionarr[i].q.ques.maq.ans, str);			
				break;
			case _NAQ:
				fread(&naq, sizeof(NAQ), 1, fp);
				questionarr[i].q.ques.naq = naq;
				break;	
			case _MTP:
				fread(&temp, sizeof(int), 1,fp);/*no of pairs*/
				questionarr[i].q.ques.mtp.no_of_pairs = temp;
				questionarr[i].q.ques.mtp.pairs = NULL;
				p = (pair*)malloc(sizeof(pair));
				for(j = 0; j < temp; j++) {
					fread(p, sizeof(pair), 1, fp);
					addPairsQuiz(&(questionarr[i].q.ques.mtp), p); /*Adding pairs*/
								
				}
				break;	
			case _ESSAY:
				fread(&essay, sizeof(ESSAY), 1, fp);
				questionarr[i].q.ques.essay = essay;
				break;
			case _TF:
				fread(&tf, sizeof(TF), 1, fp);
				questionarr[i].q.ques.truefalse = tf;
				break;
		}
		fread(&temp, sizeof(int), 1, fp);
		questionarr[i].marks = temp;
		questionarr[i].next = NULL;
		
	}
	return questionarr;
}

void fetchquizinfo(StudentInfo* si, char* file){
	char str[100];
	int temp;
	FILE* fp;
	fp = NULL;
	strcpy(str, "./Info/");
	strcat(str, file);
	strcat(str, ".info");
	
	fp = fopen(str, "r");
        if(!fp) {
		perror("Cannot open quiz info file");
		return;
   	}
	
	fread(&temp, sizeof(int), 1, fp);
	si->total_marks = temp;
	fread(&temp, sizeof(int), 1, fp);
	si->totaltime = temp;
	fread(&temp, sizeof(int), 1, fp);
	si->totalques = si-> no_of_ques_left = temp;
	strcpy(si->quizname, file);
	si->score = 0;
	
	
}

char* runquiz(StudentInfo* studentinfo, question_ll* ques) {/*if terminated returns NULL*/
	int n = studentinfo->no_of_ques_left;
	int i = 0, j, no_of_opts, anslen, found, k, left, right, no, manually = 0;
	double *scorearray;
	char ans, ch, *str, temp[50], con[2];
	double naqans, scoretemp = 0;
	MCQ mcq;
	MAQ maq;
	NAQ naq;
	MTP mtp;
	FILE* fp;
	ESSAY essay;
	TF tf;
	pair  *pairs, *pairsarr, *pairsarr2;
	scorearray = (double*) malloc(sizeof(double) * n);
	option* opts;
	questionString qs;
	for(i = 0; i < n; i++) {
		printheader("Quizzy");
		printmiddle(studentinfo->quizname);
		printf("\n");
		printmiddle("(Press Ctrl + t + ENTER key to Suspend the Quiz)");
		printf("%120s%.2f ]\n", "....Score [ ", studentinfo->score);
		printf("%120s%d ]\n", "....Questions Left [ ", studentinfo->no_of_ques_left);
		
		printf("\n");
		printf(" %d: ", ques[i].q.id);
		switch(ques[i].q.type_of_ques) {
			case _MCQ: 
				mcq = ques[i].q.ques.mcq;
				qs = mcq.qs;
				printf("%s\n\n", qs.ques);
				if(!mcq.opts)
					printf("Options are not there");
				opts = mcq.opts;
				printf("\n");
				printf("Select one:\n");
				j = 0;
				while(opts != NULL) {
					printf("  %c.%s\n", (char)(j + 'A'), opts->opt_str);
					j++;
					opts = opts->next;				
				}
				
				printf("\n\nAnswer Label:\n");
				scanf("%s", temp);
				getchar();
				str = checkforterminate(temp);
				if(str == NULL) {//termination
					terminateQuiz(studentinfo, ques, i);
					return NULL;
				}
				ans = toupper(str[0]);
				if(ans == mcq.ans){
					scorearray[i] = ques[i].marks;
					studentinfo->score += ques[i].marks;
				}
				else {
					scorearray[i] = 0;
				}
				(studentinfo->no_of_ques_left)--;
				if(str)
					free(str);
		
				//printf("\nCorrect ans: %c\n\n", mcq.ans);
				break;
			case _MAQ:

				maq = ques[i].q.ques.maq;
				qs = maq.qs;
				printf("%s\n", qs.ques);
				opts = maq.opts;
				no_of_opts = maq.no_of_opt;
				if(!opts)
					printf("Options are not there");
				printf("Choose the correct labels:\n");
				j = 0;
				while(opts != NULL) {
					printf("  %c.%s\n", (char)(j + 'A'), opts->opt_str);
					j++;
					opts = opts->next;				
				}

				printf("\n\n  Enter the labels: %25s\n", "(Press ENTER to confirm your answer)");
				scanf("%[^\n]s", temp);
				
				getchar();
				str = checkforterminate(temp);
				if(str == NULL) {//termination
					terminateQuiz(studentinfo, ques, i);
					return NULL;
				}
				j = 0;
				no = 0;
				anslen = strlen(maq.ans);
				scoretemp = 0;
				while(j < strlen(str)) {/*reading ans*/
					if(str[j] != ',' && str[j] != ' ' && no < no_of_opts) {
						found = 0;
						no++;
						str[j] = toupper(str[j]);
						for(k = 0; k< anslen; k++)
						{
							
							if(maq.ans[k] == str[j]) {
								studentinfo->score += ((float)ques[i].marks / anslen) ;
								found = 1;
								scoretemp += ((float)ques[i].marks / anslen);
								break;
							}
							
						}
						if(!found)
							if(no_of_opts != anslen) {
							  	studentinfo->score -= (float)(ques[i].marks / (float)(no_of_opts - anslen));
								scoretemp -= (float)(ques[i].marks / (float)(no_of_opts - anslen));
							} 
								
					}
					j++;					
				}
				(studentinfo->no_of_ques_left)--;
				if(str)
					free(str);
				scorearray[i] = scoretemp;
				
				//printf("\nCorrect ans: %s\n\n", maq.ans);
				break;
			case _NAQ:
				naq = ques[i].q.ques.naq;
				qs = naq.qs;
				printf("%s\n", qs.ques);
				printf("\n\n Enter the answer (numerical part) :\n");
				scanf("%s", temp);
				getchar();
				str = checkforterminate(temp);
				if(str == NULL) {//termination
					terminateQuiz(studentinfo, ques, i);
					return NULL;
				}
				naqans = (double) atof(str);
				if(naqans == naq.ans) {
					studentinfo->score += (float)ques[i].marks;
					scorearray[i] = (float)ques[i].marks;
				}
				(studentinfo->no_of_ques_left)--;
				if(str)
					free(str);
				//printf("Correct ans: %lf\n\n", naq.ans);
				break;	
			case _MTP:
				mtp = ques[i].q.ques.mtp;
				//printf("No of pairs: %d\n", mtp.no_of_pairs);
				pairsarr = (pair*)malloc(sizeof(pair) * mtp.no_of_pairs);
				pairsarr2 = (pair*)malloc(sizeof(pair) * mtp.no_of_pairs);
				pairs = mtp.pairs;
				printf("Match the pairs:\n");
				j = 0;
				while(pairs != NULL) {/*Copying into arrays*/
					//printf("%c.%-25s\t%d.%-25s\n", (char)(j + 'A'), pairs->str1, (j + 1), pairs->str2);
					pairsarr[j] = *pairs;/*Correct pairs*/
					pairsarr2[j] = *pairs;
					pairs = pairs->next;
					j++;				
				}
				shufflepairs(pairsarr2, mtp.no_of_pairs);/*shuffle the second array*/
				j =0;
				while(j < mtp.no_of_pairs) {
					printf("  %d.%-25s\t%c.%-25s\n", (char)(j + 1), pairsarr2[j].str1, (j + 'A'), pairsarr2[j].str2);
					j++;
				}
				printf("\n\nEnter the pairs separated with commas:(ex: 1 - A, 2 - C)%15s\n", "(Press ENTER to confirm answer)");
				scanf("%[^\n]s", temp);
				getchar();
				str = checkforterminate(temp);
				if(str == NULL) {//termination
					terminateQuiz(studentinfo, ques, i);
					return NULL;
				}
				/////To read pairs////
				j = 0;
				scoretemp = 0;
				while(j < strlen(str)) {
					if(isalpha(str[j]) ||isdigit(str[j])) {
					
						con[0] = str[j];
						con[1] = '\0';
						
						left = atoi(con) - 1;
					
						j++;
						for(; j < strlen(str); j++){
							if(isalpha(str[j]) || isdigit(str[j])){
								if(str[j] == '0'){//no ans given
									
									break;//dont consider marks
								}
								if(isalpha(str[j])){
									str[j] = toupper(str[j]);
									right = str[j] - 'A';
								
									if(!strcmp(pairsarr[left].str2, pairsarr2[right].str2)) {
										studentinfo->score += ((float)ques[i].marks / mtp.no_of_pairs);
										scoretemp += ((float)ques[i].marks / mtp.no_of_pairs);
										break;
									}
									break;	
								}
							}
						}
					}
					j++;
				}
				if(str)
					free(str);
				if(pairsarr)
					free(pairsarr);
				if(pairsarr2)
					free(pairsarr2);
				(studentinfo->no_of_ques_left)--;
				scorearray[i] = scoretemp;
				break;
			case _ESSAY:
				manually = 1;	
				essay = ques[i].q.ques.essay;
				qs = essay.qs;
				fp = fopen("./Quiz/essaytemplate.essay", "w");
				if(!fp) {
					perror("Cannot create a file");
					exit(1);
				}
				fprintf(fp, "%s\n%s\n%s%s.essay\nThis essay carries %d marks\n====START WRITING HERE=====\n", qs.ques, essay.graderinfo, "Save this file with your ID, i.e ", studentinfo->ID, ques[i].marks);
				fclose(fp);
				printf("Write an Essay:\n %s\n", qs.ques);
				printf("\n Grader information for you on writing essay: %s\n\nA gedit will open for you. Follow instruction on it, if any.\n(Press ENTER to open gedit editor)\n", essay.graderinfo);
				ch = getchar();
				sprintf(temp, "%c", ch);
				str = checkforterminate(temp);
				if(str == NULL) {//termination
					terminateQuiz(studentinfo, ques, i);
					return NULL;
				}
				j = system("gedit ./Quiz/essaytemplate.essay");
				if(j == -1) {
					printf("There is some problem while using gedit.\n");
					getchar();
				}
				printf("\nExit the editor to come back to terminal.\n");
				scorearray[i] = 0;
				(studentinfo->no_of_ques_left)--;
				if(str)
					free(str);
				break;
			case _TF:
				tf = ques[i].q.ques.truefalse;
				qs = tf.qs;
				printf("Whether following statment is true or false:\n  Statement: %s\n", qs.ques);
				printf("\nEnter True or false:\n");
				scanf("%s", temp);
				getchar();
				str = checkforterminate(temp);
				if(str == NULL) {//termination
					terminateQuiz(studentinfo, ques, i);
					return NULL;
				}
				str[0] = tolower(temp[0]);
				if(str[0] == 't') {
					if(tf.ans){
						printf("Correct\n");
						scorearray[i] = ques[i].marks;
						studentinfo->score += ques[i].marks;
					}
					else {
						scorearray[i] = 0;
						printf("Incorrect: FALSE: %s\n Press ENTER\n", tf.stmt);
						getchar();
					}
					
				}
				else {/*false*/
					if(!tf.ans){
						printf("Correct\n");
						scorearray[i] = ques[i].marks;
						studentinfo->score += ques[i].marks;
					}
					else {
						scorearray[i] = 0;
						printf("Incorrect: TRUE: %s\n Press ENTER\n", tf.stmt);
						getchar();
					}
				}
				if(str)
					free(str);
				(studentinfo->no_of_ques_left)--;
				break;
		}
		
	}
	(studentinfo->finished) = 1;/*Quiz finished*/
	printmiddle("~~Quiz Set Completed~~");
	printf("\n");
	printmiddle("Press ENTER to view marks");
	getchar();
	printheader("Quizzy Score Table");
	printf("\n\n");
	for(i = 0; i < n; i++) {
		printf("   Question %d :   %.2f\n", (i + 1), scorearray[i]);
	}
	if(manually) {
		printf("\nThere are some questions left to be evaluated manually by the grader\n");
	}
	if(scorearray)
		free(scorearray);
	printf("\nTotal score: %.2f /(out of) %.2f\nPress ENTER\n", studentinfo->score, (float)studentinfo->total_marks);
	getchar();
	return "ab";
}

void terminateQuiz(StudentInfo* studentinfo, question_ll* ques, int j) {
	(studentinfo->finished) = 0;
	char* filename = storeStudentInfo(studentinfo);
	if(filename != NULL) {
		storeLeftQuestions(ques, filename , j, studentinfo->totalques);
		printheader("Quizzy");
		
		printf("\n\nQuiz Suspended...................\nYou can resume the quiz later. Use './project -r %s'\n(Press ENTER)\n", studentinfo->ID);
		getchar();
	}
}

char* storeStudentInfo(StudentInfo* studentinfo) {
	FILE *fp;
	StudentInfo temp;
	char file[100], storefile[100];
	int i = 0, j = 0;
	strcpy(file, "./Quiz/");
	strcat(file, studentinfo->ID);
	strcat(file, ".state");//file name ready
	srand(time(NULL));
	sprintf(storefile, "%ld", random());
	strcat(storefile, ".left.ques");
	//printf("Store file by info %s\n", storefile);
	strcpy(studentinfo->filename, storefile);//left question file
	fp = fopen(file, "r+w");
	if(!fp){
		perror("Cannot create state file");
		getchar();
		return NULL;
  	}
	while((fread(&temp, sizeof(StudentInfo), 1, fp))) {/*Counting the position of structure*/
		if(!strcmp(temp.quizname, studentinfo->quizname)) {
			break;
		}
		i++;
	}
	fseek(fp, 0, SEEK_SET);
	for(j = 0; j < i ; j++)  {/*Move till that position*/
		fread(&temp, sizeof(StudentInfo), 1, fp);
	}
	fwrite(studentinfo, sizeof(StudentInfo), 1, fp);
	fclose(fp);
	//printf("Stored student info\n");
	return studentinfo->filename;
}
/*Stores left question from curr to end*/
void storeLeftQuestions(question_ll* ques, char* filename, int curr, int end){
	char file[100];
	int j;
	MCQ mcq;
	MAQ maq;
	NAQ naq;
	MTP mtp;
	pair* pairs;
	questionString qs;
	option* opts;
	int no_of_ques = end - curr;
	strcpy(file, "./Quiz/");
	strcat(file, filename);//filename ready
	FILE* fp = fopen(file, "w");//opening file
	printf("Storing in the file : %s\n", file);
	if(!fp){
		perror("Cannot create left question file");
		getchar();
		return;
  	}
	fwrite(&no_of_ques, sizeof(int), 1, fp);/*no of ques*/
	j = curr;
	while(j < end) {
		fwrite(&(ques[j].q.id), sizeof(int), 1, fp);
		fwrite(&(ques[j].q.type_of_ques), sizeof(int), 1, fp);
		switch(ques[j].q.type_of_ques) {
			case _MCQ: 
				mcq = ques[j].q.ques.mcq;
				qs = mcq.qs;
				fwrite(&qs,sizeof(struct questionString), 1, fp);/*Storing questionString structure*/

				opts = mcq.opts;
			
				fwrite(&(mcq.no_of_opt), sizeof(int), 1, fp);/*no of options*/
				while(opts != NULL) {
					fwrite(&(opts->opt_str), sizeof(char) * 50, 1, fp);/*Storing options*/
					opts = opts->next;				
				}
				fwrite(&mcq.ans, sizeof(char), 1, fp);
				break;
			case _MAQ:
				maq = ques[j].q.ques.maq;
				qs = maq.qs;
				fwrite(&qs,sizeof(struct questionString), 1, fp);/*Storing questionString structure*/
				opts = maq.opts;
			
				fwrite(&(maq.no_of_opt), sizeof(int), 1, fp);
				while(opts != NULL) {
					fwrite(&(opts->opt_str), sizeof(char) * 50, 1, fp);
	
					opts = opts->next;				
				}
				fwrite(maq.ans, sizeof(char), strlen(maq.ans) + 1, fp);/*storing string with \0*/
				break;
			case _NAQ:
				naq = ques[j].q.ques.naq;
				fwrite(&naq, sizeof(NAQ), 1, fp);/*Storing NAQ structure*/
				break;
			case _MTP:
				mtp = ques[j].q.ques.mtp;
				pairs = mtp.pairs;
			
				fwrite(&(mtp.no_of_pairs), sizeof(int), 1, fp);/*No of pairs*/
				while(pairs != NULL) {
					fwrite(pairs, sizeof(pair), 1, fp);
					pairs = pairs->next;				
				}
				break;
			case _ESSAY:
				fwrite(&(ques[j].q.ques.essay), sizeof(ESSAY), 1, fp);
				break;
			case _TF:
				
				fwrite(&(ques[j].q.ques.truefalse), sizeof(TF), 1, fp);
				break;
						
		}
		
		fwrite(&(ques[j].marks), sizeof(int), 1, fp);
		j++;
	}
	fclose(fp);
}

int getNoOfPendingQuiz(StudentInfo si){
	FILE *fp;
	StudentInfo stud;
	char temp[100];
	int counter = 0;
	strcpy(temp, "./Quiz/");
	strcat(temp, si.ID);
	strcat(temp, ".state");
	//printf("Reading from %s\n", temp);
	fp = fopen(temp, "r");
	if(!fp)
		return -1;

	while((fread(&stud, sizeof(StudentInfo), 1, fp))) {
	
		if(stud.finished == 0) {
			counter++;
			//printf("Counter %d",counter);
		} 
	}
	fclose(fp);
	return counter;
}

char* selectPendingQuiz(StudentInfo si, int no, int restore) {/*Give names user selected file*/
	FILE *fp;
	char** array;
	StudentInfo stud;
	char temp[100], *ans;
	int counter = 0, i;
	strcpy(temp, "./Quiz/");
	strcat(temp, si.ID);
	strcat(temp, ".state");
	array = (char**) malloc(sizeof(char*) * no);
	for(i = 0; i < no; i++){
		array[i] = (char*) malloc(sizeof(char) * 20);
	}
	//printf("Reading from %s\n", temp);
	fp = fopen(temp, "r");
	printf("\n\n");
	if(!fp)
		return NULL;

	while((fread(&stud, sizeof(StudentInfo), 1, fp))) {
		if(stud.finished == 0) {
			counter++;
			strcpy(array[counter - 1], stud.quizname);
			printf("%d.%s\n", counter, stud.quizname);
			
		} 
	}
	
	fclose(fp);
	if(restore == 1) {
		do{
			printf("\nEnter your choice from above quiz set:\n");
			scanf("%d", &i);
			getchar();
			if(i > counter || i < 1)
				printf("Invalid serial number\n");
		}while(i > counter || i < 1);
		i = i - 1;
	    	ans = (char*)malloc(sizeof(char) * (strlen(array[i]) + 1));
		strcpy(ans, array[i]);
	}
	else {
		return NULL;
	}
	for(i = 0; i < no; i++){
		if(array[i]!= NULL)
			free(array[i]);
	}
	if(array != NULL)
			free(array);
	return ans;

}

void modifyStudentInfo(StudentInfo* si, char* file){
	FILE *fp;
	StudentInfo stud;
	char temp[100];

	strcpy(temp, "./Quiz/");
	strcat(temp, si->ID);
	strcat(temp, ".state");
	//printf("Reading from %s\n", temp);
	fp = fopen(temp, "r");
	if(!fp)
		return;
	while((fread(&stud, sizeof(StudentInfo), 1, fp))) {
		if(stud.finished == 0 && !strcmp(stud.quizname, file)) {
			si->finished = stud.finished;//DATA copying
			si->score = stud.score;
			si->no_of_ques_left = stud.no_of_ques_left;
			si->totalques = stud.totalques;
			si->totaltime = stud.totaltime;
			si->total_marks = stud.total_marks;
			strcpy(si->ID , stud.ID);
			strcpy(si->quizname , stud.quizname);
			strcpy(si->filename , stud.filename);
			//printf("Student info modified successfully....\n");
			return;
		} 
	}
	fclose(fp);

}


char* convertintoString(int a){
	if(a == _MCQ || a == _MAQ)
		return "multichoice";
	else if(a == _NAQ)
		return "numerical";
	else if(a == _MTP)
		return "matching";
	else if(a == _ESSAY)
		return "essay";
	else if(a == _TF)
		return "truefalse";
	return NULL;
}

int found(int* arr, int n, int find){
	int i;
	for(i = 0; i < n; i++) {
		if(arr[i] == find)
			return 1;
	}
	return 0;
}
void export2(char* filename, int tpresent, char* target) {
	char temp[100], idstr[6], **optionstr, str[20], correctfrac[20], incorrectfrac[20], ch;
	int anslen, *arr;
	NAQ naq;
	ESSAY essay;
	TF tf;
	pair* p;
	int id;
	int i, j, ans;
	questionString qs;
	
	FILE* fp;
	mxml_node_t *xml, *quiz, *question, *category, *name, *questiontext, *text, *genfd, *answer;
	strcpy(temp, "./QuestionBank/");/*./QuestionBank/filename.qb*/
	strcat(temp, filename);
	strcat(temp, ".qb");
	fp = fopen(temp, "r");
	//printf("%s\n", file_name);
	
	if(!fp) {
		perror("No such question bank found");
		printf("Press ENTER key\n");
		getchar();
		return;
	}
	xml = mxmlNewXML("1.0");//Xml tag
	quiz = mxmlNewElement(xml, "quiz");/*questionbank*/
	//mxmlElementSetAttr(questionbank, "name", filename);/*name attr*/
	fread(&i, sizeof(int), 1, fp);/*READ no of ques*/
	question = mxmlNewElement(quiz, "question");
	mxmlElementSetAttr(question, "type", "category");
	category = mxmlNewElement(question, "category");
	text = mxmlNewElement(category, "text");
	mxmlNewText(text, 0, filename);
	
	while(fread(&id, sizeof(int), 1, fp) != 0) {/*READ question ID*/
		question = mxmlNewElement(quiz,"question");/*question tag*/
		fread(&i, sizeof(int), 1, fp);/*READ ques type*/
		mxmlElementSetAttr(question, "type", convertintoString(i));
		name = mxmlNewElement(question, "name");
		text = mxmlNewElement(name, "text");
		sprintf(idstr, "%d", id);
		strcpy(temp, "Question : ");
		strcat(temp, idstr);
		mxmlNewText(text, 0, temp);

		switch(i) {
			case _MCQ: 
				fread(&qs,sizeof(struct questionString), 1, fp);/*Storing questionString structure*/
				questiontext = mxmlNewElement(question, "questiontext");
				mxmlElementSetAttr(questiontext, "format", "html");
				text = mxmlNewElement(questiontext, "text");
				mxmlNewCDATA(text, qs.ques);//here
				genfd = mxmlNewElement(question, "generalfeedback");
				mxmlElementSetAttr(genfd, "format", "html");
				text = mxmlNewElement(genfd, "text");
				genfd = mxmlNewElement(question, "shownumcorrect");
				genfd = mxmlNewElement(question, "defaultgrade");
				mxmlNewReal(genfd, 1.0);
				genfd = mxmlNewElement(question, "penalty");
				mxmlNewReal(genfd, (double)1/3);
				genfd = mxmlNewElement(question, "hidden");
				mxmlNewInteger(genfd, 0);
				genfd = mxmlNewElement(question, "single");
				mxmlNewText(genfd, 0, "true");
				genfd = mxmlNewElement(question, "shuffleanswers");
				mxmlNewText(genfd, 0, "true");
				genfd = mxmlNewElement(question, "answernumbering");
				mxmlNewText(genfd, 0, "abc");
				genfd = mxmlNewElement(question, "correctfeedback");
				mxmlElementSetAttr(genfd, "format", "html");
				text = mxmlNewElement(genfd, "text");
				mxmlNewText(text, 0, "Your answer is Correct.");
				genfd = mxmlNewElement(question, "partiallycorrectfeedback");
				mxmlElementSetAttr(genfd, "format", "html");
				text = mxmlNewElement(genfd, "text");
				genfd = mxmlNewElement(question, "incorrectfeedback");
				mxmlElementSetAttr(genfd, "format", "html");
				text = mxmlNewElement(genfd, "text");
				mxmlNewText(text, 0, "Your answer is Incorrect.");

				fread(&i, sizeof(int), 1, fp);/*no of options*/
				optionstr = (char**)malloc(sizeof(char*) * i);
				for(j = 0; j < i; j++) {
					optionstr[j] = (char*)malloc(sizeof(char) * 50);
				}
				j = 0;
				while(j < i) {

					fread(optionstr[j], sizeof(char) , 50, fp);/*Reading options*/
					j++;
							
				}
				fread(&ch, sizeof(char), 1, fp);
				ans = (int) ch - 'A';
				j = 0;
				while(j < i) {

					answer = mxmlNewElement(question, "answer");
					mxmlElementSetAttr(answer, "format", "html");
					if(j == ans) {
						mxmlElementSetAttr(answer, "fraction", "100");
					}
					else {
						mxmlElementSetAttr(answer, "fraction", "0");
					}
					text = mxmlNewElement(answer, "text");
					mxmlNewCDATA(text, optionstr[j]);
					genfd = mxmlNewElement(answer, "feedback");
					mxmlElementSetAttr(genfd, "format", "html");
					text = mxmlNewElement(genfd, "text");
					j++;
							
				}
				for(j = 0; j < i; j++) {
					free(optionstr[j]);
				}
				break;
			case _MAQ: 
				fread(&qs,sizeof(struct questionString), 1, fp);/*Storing questionString structure*/
				questiontext = mxmlNewElement(question, "questiontext");
				mxmlElementSetAttr(questiontext, "format", "html");
				text = mxmlNewElement(questiontext, "text");
				mxmlNewCDATA(text,  qs.ques);//here
				genfd = mxmlNewElement(question, "generalfeedback");
				mxmlElementSetAttr(genfd, "format", "html");
				text = mxmlNewElement(genfd, "text");
				genfd = mxmlNewElement(question, "shownumcorrect");
				genfd = mxmlNewElement(question, "defaultgrade");
				mxmlNewReal(genfd, 1.0);
				genfd = mxmlNewElement(question, "penalty");
				mxmlNewReal(genfd, (double)1/3);
				genfd = mxmlNewElement(question, "hidden");
				mxmlNewInteger(genfd, 0);
				genfd = mxmlNewElement(question, "single");
				mxmlNewText(genfd, 0, "false");
				genfd = mxmlNewElement(question, "shuffleanswers");
				mxmlNewText(genfd, 0, "true");
				genfd = mxmlNewElement(question, "answernumbering");
				mxmlNewText(genfd, 0, "abc");
				genfd = mxmlNewElement(question, "correctfeedback");
				mxmlElementSetAttr(genfd, "format", "html");
				text = mxmlNewElement(genfd, "text");
				mxmlNewText(text, 0, "Your answer is Correct.");
				genfd = mxmlNewElement(question, "partiallycorrectfeedback");
				mxmlElementSetAttr(genfd, "format", "html");
				text = mxmlNewElement(genfd, "text");
				mxmlNewText(text, 0, "Your answer is partially correct.");
				genfd = mxmlNewElement(question, "incorrectfeedback");
				mxmlElementSetAttr(genfd, "format", "html");
				text = mxmlNewElement(genfd, "text");
				mxmlNewText(text, 0, "Your answer is Incorrect.");

				fread(&i, sizeof(int), 1, fp);/*no of options*/
				optionstr = (char**)malloc(sizeof(char*) * i);
				for(j = 0; j < i; j++) {
					optionstr[j] = (char*)malloc(sizeof(char) * 50);
				}
				j = 0;
				while(j < i) {

					fread(optionstr[j], sizeof(char) , 50, fp);/*Reading options*/
					j++;
							
				}
				arr = (int*) malloc(sizeof(int) * i);
				
				j = 0;
				do {
					fread(&ch, sizeof(char), 1, fp);/*reading ans till '\0'*/
					str[j] = ch;
					arr[j] = ch - 'A';
					j++;
				}while(ch != '\0');
				anslen = strlen(str);
				sprintf(correctfrac, "%.5f", ((float)1/anslen) * 100);
				if(i != anslen)
					sprintf(incorrectfrac, "%.5f", -((float)1/(i - anslen)) * 100);
				else
					sprintf(incorrectfrac, "%f", (float)0);
				j = 0;
				while(j < i) {

					answer = mxmlNewElement(question, "answer");
					mxmlElementSetAttr(answer, "format", "html");
					if(found(arr, anslen, j) == 1) {
						mxmlElementSetAttr(answer, "fraction", correctfrac);
					}
					else {
						mxmlElementSetAttr(answer, "fraction", incorrectfrac);
					}
					text = mxmlNewElement(answer, "text");
					mxmlNewCDATA(text, optionstr[j]);
					genfd = mxmlNewElement(answer, "feedback");
					mxmlElementSetAttr(genfd, "format", "html");
					text = mxmlNewElement(genfd, "text");
					j++;
							
				}
				for(j = 0; j < i; j++) {
					if(optionstr[j])
						free(optionstr[j]);
				}
				if(arr)
					free(arr);
				break;
			case _NAQ:
			
				fread(&naq, sizeof(NAQ), 1, fp);
				qs = naq.qs;
				questiontext = mxmlNewElement(question, "questiontext");
				mxmlElementSetAttr(questiontext, "format", "html");
				text = mxmlNewElement(questiontext, "text");
				mxmlNewCDATA(text,qs.ques);
				genfd = mxmlNewElement(question, "generalfeedback");
				mxmlElementSetAttr(genfd, "format", "html");
				text = mxmlNewElement(genfd, "text");
				genfd = mxmlNewElement(question, "defaultgrade");
				mxmlNewReal(genfd, 1.0);
				genfd = mxmlNewElement(question, "penalty");
				mxmlNewReal(genfd, (double)1/3);
				genfd = mxmlNewElement(question, "hidden");
				mxmlNewInteger(genfd, 0);
				answer = mxmlNewElement(question, "answer");
				mxmlElementSetAttr(answer, "fraction", "100");
				mxmlElementSetAttr(answer, "format", "moodle_auto_format");
				text = mxmlNewElement(answer, "text");
				genfd = mxmlNewElement(question, "shownumcorrect");
				mxmlNewReal(text, naq.ans);
				genfd = mxmlNewElement(answer, "feedback");
				mxmlElementSetAttr(genfd, "format", "html");
				text = mxmlNewElement(genfd, "text");
				genfd = mxmlNewElement(answer, "tolerance");
				mxmlNewInteger(genfd, 0);
				genfd = mxmlNewElement(question, "unitgradingtype");
				mxmlNewInteger(genfd, 0);
				genfd = mxmlNewElement(question, "unitpenalty");
				mxmlNewReal(genfd, 0.1000000);
				genfd = mxmlNewElement(question, "showunits");
				mxmlNewInteger(genfd, 3);
				genfd = mxmlNewElement(question, "unitsleft");
				mxmlNewInteger(genfd, 0);
				break;
			case _MTP:
				questiontext = mxmlNewElement(question, "questiontext");
				mxmlElementSetAttr(questiontext, "format", "html");
				text = mxmlNewElement(questiontext, "text");
				mxmlNewText(text, 0, "match the pairs");
				genfd = mxmlNewElement(question, "generalfeedback");
				mxmlElementSetAttr(genfd, "format", "html");
				text = mxmlNewElement(genfd, "text");
				genfd = mxmlNewElement(question, "defaultgrade");
				mxmlNewReal(genfd, 1.0);
				genfd = mxmlNewElement(question, "penalty");
				mxmlNewReal(genfd, (double)1/3);
				genfd = mxmlNewElement(question, "hidden");
				mxmlNewInteger(genfd, 0);
				genfd = mxmlNewElement(question, "shuffleanswers");
				mxmlNewText(genfd, 0, "true");
				genfd = mxmlNewElement(question, "correctfeedback");
				mxmlElementSetAttr(genfd, "format", "html");
				text = mxmlNewElement(genfd, "text");
				mxmlNewText(text, 0, "Your answer is Correct.");
				genfd = mxmlNewElement(question, "partiallycorrectfeedback");
				mxmlElementSetAttr(genfd, "format", "html");
				text = mxmlNewElement(genfd, "text");
				mxmlNewText(text, 0, "Your answer is partially correct.");
				genfd = mxmlNewElement(question, "incorrectfeedback");
				mxmlElementSetAttr(genfd, "format", "html");
				text = mxmlNewElement(genfd, "text");
				mxmlNewText(text, 0, "Your answer is Incorrect.");
				genfd = mxmlNewElement(question, "shownumcorrect");
				
				
				fread(&i, sizeof(int), 1,fp);
				p = (pair*) malloc(sizeof(pair));

				if(p == NULL) {printf("Memory not allocated");}
				for(j = 0; j < i; j++) {
					fread(p, sizeof(pair), 1, fp);
					genfd = mxmlNewElement(question, "subquestion");
					mxmlElementSetAttr(genfd, "format", "html");
					text = mxmlNewElement(genfd, "text");
					mxmlNewCDATA(text, p->str1);
					answer = mxmlNewElement(genfd, "answer");
					text = mxmlNewElement(answer, "text");
					mxmlNewCDATA(text, p->str2);
								
				}
				free(p);
				break;
			case _ESSAY:
				fread(&essay, sizeof(ESSAY), 1, fp);
				qs = essay.qs;
				questiontext = mxmlNewElement(question, "questiontext");
				mxmlElementSetAttr(questiontext, "format", "html");
				text = mxmlNewElement(questiontext, "text");
				mxmlNewCDATA(text,qs.ques);
				genfd = mxmlNewElement(question, "generalfeedback");
				mxmlElementSetAttr(genfd, "format", "html");
				text = mxmlNewElement(genfd, "text");
				genfd = mxmlNewElement(question, "defaultgrade");
				mxmlNewReal(genfd, 1.0);
				genfd = mxmlNewElement(question, "penalty");
				mxmlNewReal(genfd, (double)1/3);
				genfd = mxmlNewElement(question, "hidden");
				mxmlNewInteger(genfd, 0);
				genfd = mxmlNewElement(question, "responseformat");
				mxmlNewText(genfd, 0, "editor");
				genfd = mxmlNewElement(question, "responserequired");
				mxmlNewInteger(genfd, 1);
   				genfd = mxmlNewElement(question, "responsefieldlines");
				mxmlNewInteger(genfd, 20);
				genfd = mxmlNewElement(question, "attachments");
				mxmlNewInteger(genfd, 0);
				genfd = mxmlNewElement(question, "attachmentsrequired");
				mxmlNewInteger(genfd, 0);
				genfd = mxmlNewElement(question, "graderinfo");
				mxmlElementSetAttr(genfd, "format", "html");
				text = mxmlNewElement(genfd, "text");
				mxmlNewCDATA(text, essay.graderinfo);
				genfd = mxmlNewElement(question, "responsetemplate");
				mxmlElementSetAttr(genfd, "format", "html");
				text = mxmlNewElement(genfd, "text");
				
				break;
			case _TF:
				fread(&tf, sizeof(TF), 1, fp);
				qs = tf.qs;
				questiontext = mxmlNewElement(question, "questiontext");
				mxmlElementSetAttr(questiontext, "format", "html");
				text = mxmlNewElement(questiontext, "text");
				mxmlNewCDATA(text, qs.ques);//here
				genfd = mxmlNewElement(question, "generalfeedback");
				mxmlElementSetAttr(genfd, "format", "html");
				text = mxmlNewElement(genfd, "text");
				genfd = mxmlNewElement(question, "shownumcorrect");
				genfd = mxmlNewElement(question, "defaultgrade");
				mxmlNewReal(genfd, 1.0);
				genfd = mxmlNewElement(question, "penalty");
				mxmlNewReal(genfd, (double)1/3);
				genfd = mxmlNewElement(question, "hidden");
				mxmlNewInteger(genfd, 0);

				answer = mxmlNewElement(question, "answer");
				mxmlElementSetAttr(answer, "format", "moodle_auto_format");
				genfd = mxmlNewElement(answer, "feedback");
				mxmlElementSetAttr(genfd, "format", "html");
				text = mxmlNewElement(genfd, "text");
				if(tf.ans == 1) {
					mxmlElementSetAttr(answer, "fraction", "100");
					mxmlNewCDATA(text, "Correct");
				}
				else {
					mxmlElementSetAttr(answer, "fraction", "0");
					mxmlNewCDATA(text, tf.stmt);
				}
				text = mxmlNewElement(answer, "text");
				mxmlNewText(text, 0, "true");
				
				answer = mxmlNewElement(question, "answer");
				mxmlElementSetAttr(answer, "format", "moodle_auto_format");
				genfd = mxmlNewElement(answer, "feedback");
				mxmlElementSetAttr(genfd, "format", "html");
				text = mxmlNewElement(genfd, "text");
				if(tf.ans == 0) {
					mxmlElementSetAttr(answer, "fraction", "100");
					mxmlNewCDATA(text, "Correct");
				}
				else {
					mxmlElementSetAttr(answer, "fraction", "0");
					mxmlNewCDATA(text, tf.stmt);
				}
				text = mxmlNewElement(answer, "text");
				mxmlNewText(text, 0, "false");
				
				break;
		}
		
	}
	fclose(fp);
	if(tpresent == 0) {
		printf("Enter a name for xml file:");
		scanf("%[^\n]s", filename);
		getchar();
	}
	else{
		filename = (char*) malloc(sizeof(char) * (strlen(target) + 1));
		strcpy(filename, (target));
	}
	strcpy(temp, "./Export_Files/");/*./QuestionBank/filename.qb*/
	strcat(temp, filename);
	strcat(temp, ".xml");
	fp = fopen(temp, "w");
	if(!fp) {
		perror("Cant open file ");
		getchar();
		return;
	}
	mxmlSaveFile(xml, fp, MXML_NO_CALLBACK);
	printf("\nExported successfully to %s.........\nPress ENTER key\n", temp);
	getchar();
	
	fclose(fp);
	mxmlDelete(xml);/*Delete the tree*/
}

char* getfilenameimport(char* str) {
	char* filename, *temp;
	int i, len;
	for(i = strlen(str) - 1; i >= 0; i--) {
		if(str[i] == '/') {
			break;	
		}
	}
	i++;
	temp = &str[i];
	len = strlen(temp);
	filename = (char*)malloc(sizeof(char) * (len + 1));
	strcpy(filename, temp);
	return filename;
}

int getTypeFromString(char* type, char* single) {
	if(!strcmp(type, "multichoice")) {
		if(!strcmp(single, "true"))
			return _MCQ;
		else
			return _MAQ;
	}
	else if(!strcmp(type, "numerical"))
		return _NAQ;
	else if(!strcmp(type, "matching"))
		return _MTP;
	else if(!strcmp(type, "essay"))
		return _ESSAY;
	else if(!strcmp(type, "truefalse"))
		return _TF;
	return -1;		
}

char* removeCDATA(const char* str) {
	char* temp = malloc((strlen(str) + 1) * sizeof(char));
	strcpy(temp, str);
	temp[strlen(str) - 2] = '\0';
	return temp;
}

void removeWhitespaceFromFile(char* file) {
	FILE *fp, *fw; 
	char ch, temp, nexttemp;
	int GTFlag = 0, CDATAFlag = 0;
	fp = fopen(file, "r");
	fw = fopen("temp.xml", "w");
	if(!fp || !fw) {
		perror("Cant open file ");
		getchar();
		return;
	}
	while(fread(&ch, sizeof(char), 1, fp)) {
		if(ch == '>') {
			fwrite(&ch, sizeof(char), 1, fw);
			fseek(fp, -2, SEEK_CUR);
			fread(&temp, sizeof(char), 1, fp);
			if(temp == ']') {
				CDATAFlag = 0;
			}
			fread(&temp, sizeof(char), 1, fp);
			GTFlag = 1;
		}
		else if(ch == '<') {
			fwrite(&ch, sizeof(char), 1, fw);
			if(fread(&temp, sizeof(char), 1, fp)) {
				fwrite(&temp, sizeof(char), 1, fw);
				if(temp == '!') {
					fread(&nexttemp, sizeof(char), 1, fp);
					if(nexttemp == '[') {
						CDATAFlag = 1;/*Found CDATA*/
					}
					
					fwrite(&nexttemp, sizeof(char), 1, fw);
					
				}
				
				
				
			}
			else { }
			GTFlag = 0;
		}
		else if((!CDATAFlag) && (GTFlag && ch == ' ') || (GTFlag && ch == '\n')) {
			continue;
		}
		else {
			fwrite(&ch, sizeof(char), 1, fw);
		}
		
	}
	fclose(fw);
	fclose(fp);
	
}
void import2(char* filename) {/*Assume .xml extension*/
	char tempstr[100], str[100], type[20], *file , ans, *ansstr, val[6];
	const char *frac;
	double fracvalue;
	int i = 0, j = 0, k, t;
	FILE* fp;
	mxml_node_t *tree;
	mxml_node_t *node, *quiz, *temp, *answer, *temp2, *text, *genfd;
	questionString qs;
	NAQ naq;
	TF tf;
	pair pairs;
	ESSAY essay;
	strcpy(tempstr, filename);

	removeWhitespaceFromFile(tempstr);/*remove white space between > and <*/
	fp = fopen("temp.xml", "r");
	if(!fp) {
		perror("Cant open file ");
		getchar();
		return;
	}

	tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);/*Fetching the whole tree*/
	fclose(fp);
		

	if(!tree) {
		errno = EINVAL;
		perror("Not an XML file");
		return;
	}
	quiz = tree->child;//quiz tag
	if(!quiz) {
		perror("While parsing <quiz>tag not  found");
		getchar();
		return;
	}

	node = mxmlFindElement (quiz, tree, "question", "type", "category", MXML_DESCEND_FIRST);
	if(!node) {
		perror("While parsing No <question> with category attribute  found");
		getchar();
		return;
	}
	
	file = getfilenameimport(node->child->child->child->value.text.string);
	printf(" Question Bank name: %s\n", file);
	strcpy(str, file);/*name of qb*/
	strcpy(tempstr, "./QuestionBank/");
	strcat(tempstr, str);
	/*free(file);*/
	strcat(tempstr, ".qb");
	fp = fopen(tempstr , "w");
	if(!fp) {
		perror("Cant open file ");
		getchar();
		return;
	}
	j = 0;
	//printf(" Number of questions: %d\n", j);
	fwrite(&j, sizeof(int), 1, fp);/*no of ques*/
	i = 0;
	for(node = mxmlFindElement(quiz, tree, "question", "type", NULL, MXML_DESCEND_FIRST); node != NULL; node = mxmlGetNextSibling(node), i++) {/*node is question tag*/

		if(!(mxmlElementGetAttr(node, "type"))) {
			i--;
			continue;
		}
		strcpy(type, mxmlElementGetAttr(node, "type"));
		if(!type) {
			perror("While parsing  no type attribute  found");
			getchar();
			return;
		}

		
		if(!strcmp(type, "category")){
			i--;
			continue;
		}
		j = i + 1;
		temp = mxmlFindElement(node, tree, "single", NULL, NULL, MXML_DESCEND_FIRST);
		if(!temp) {
			t = getTypeFromString(type, NULL);
		}
		else {
		
			t = getTypeFromString(type, temp->child->value.text.string);
		}
		
		if(t == -1) {
			printf("Wrong type of question so ignored\nPress ENTER\n");
			i--;
			getchar();
			continue;
		}
		printf("-------------------------------------------------------------------------------------------------------------------------\n");
		printf(" Q%d.", j);
		fwrite(&j, sizeof(int), 1, fp);/*ID*/

		fwrite(&t, sizeof(int), 1, fp);/*type*/
		switch(t) {
			case _MCQ: 
				temp = mxmlFindElement(node, tree, "questiontext", NULL, NULL, MXML_DESCEND_FIRST);
				if(!temp) {
					perror("While parsing  no <questiontext> tag found");
					getchar();
					return;
				}
				temp = temp->child;/*text tag*/
				if(!temp) {
					perror("While parsing  no <text> inside <questiontext> tag found");
					getchar();
					return;
				}
				if(mxmlGetCDATA(temp->child)) {
					strcpy(qs.ques, removeCDATA(mxmlGetCDATA(temp->child)));/*Questiontext*/
				}
				else {
					strcpy(qs.ques, mxmlGetText(temp->child, 0));
				}
				printf("%s\n  Type: %s\n",qs.ques, type);
				fwrite(&qs,sizeof(struct questionString), 1, fp);/*Storing questionString structure*/
				j = 0;
				for(answer = mxmlFindElement(node, tree, "answer", NULL, NULL, MXML_DESCEND_FIRST); answer != NULL; answer = mxmlGetNextSibling(answer)) {
					j++;
				}
				
		
				fwrite(&j, sizeof(int), 1, fp);/*no of options*/
				j = 0;
				for(answer = mxmlFindElement(node, tree, "answer", NULL, NULL, MXML_DESCEND_FIRST); answer != NULL; answer = mxmlGetNextSibling(answer)) {
					temp =  mxmlFindElement(answer, tree, "text", NULL, NULL, MXML_DESCEND_FIRST);/*text tag*/
					if(!temp) {
						perror("While parsing  no <text> inside <answer> tag found");
						getchar();
						return;
					}

					if(mxmlGetCDATA(temp->child)) {/*CDATA is there*/
						fwrite(removeCDATA(mxmlGetCDATA(temp->child)), sizeof(char) * 50, 1, fp);/*Storing options*/
					}
					else {/*CDATA is not there but text is there*/
						fwrite(mxmlGetText(temp->child, 0), sizeof(char) * 50, 1, fp);/*Storing options*/
					}
					frac = mxmlElementGetAttr(answer, "fraction");
					if(!strcmp(frac, "100")) {
						ans = (char)'A' + j;
					}	
					j++;
								
				}
	
				fwrite(&ans, sizeof(char), 1, fp);
				break;
			case _MAQ:
				temp = mxmlFindElement(node, tree, "questiontext", NULL, NULL, MXML_DESCEND_FIRST);
				if(!temp) {
					perror("While parsing  no <questiontext> tag found");
					getchar();
					return;
				}
				temp = temp->child;/*text tag*/
				if(!temp) {
					perror("While parsing  no <text> inside <questiontext> tag found");
					getchar();
					return;
				}
				if(mxmlGetCDATA(temp->child)) {
					strcpy(qs.ques, removeCDATA(mxmlGetCDATA(temp->child)));/*Questiontext*/
				}
				else {
					strcpy(qs.ques, mxmlGetText(temp->child, 0));
				}
				printf("%s\n Type: %s\n",qs.ques, type);
			
				fwrite(&qs,sizeof(struct questionString), 1, fp);/*Storing questionString structure*/
				j = 0;
				for(answer = mxmlFindElement(node, tree, "answer", NULL, NULL, MXML_DESCEND_FIRST); answer != NULL; answer = mxmlGetNextSibling(answer)) {
					j++;
				}
				
				
				fwrite(&j, sizeof(int), 1, fp);/*no of options*/
				ansstr = (char*)malloc(sizeof(char) * (j + 1));
				j = 0;
				k = 0;
				for(answer = mxmlFindElement(node, tree, "answer", NULL, NULL, MXML_DESCEND_FIRST); answer != NULL; answer = mxmlGetNextSibling(answer)) {
					temp =  mxmlFindElement(answer, tree, "text", NULL, NULL, MXML_DESCEND_FIRST);/*text tag*/
					if(!temp) {
						perror("While parsing  no <text> inside <answer> tag found");
						getchar();
						return;
					}
					if(mxmlGetCDATA(temp->child)) {/*CDATA is there*/
						fwrite(removeCDATA(mxmlGetCDATA(temp->child)), sizeof(char) * 50, 1, fp);/*Storing options*/
					}
					else {/*CDATA is not there but text is there*/
						fwrite(mxmlGetText(temp->child, 0), sizeof(char) * 50, 1, fp);/*Storing options*/
					}
					frac = mxmlElementGetAttr(answer, "fraction");
					fracvalue = atof(frac);
					if(fracvalue > 0) {
						 ansstr[k] = (char)'A' + j;
						k++;
					}	
					j++;
								
				}
				ansstr[k] = '\0';
			
				/////////////here////////////////////////////////////////////////////
				fwrite(ansstr, sizeof(char), strlen(ansstr) + 1, fp);/*storing string with \0*/
				free(ansstr);
				break;
			case _NAQ:
				temp = mxmlFindElement(node, tree, "questiontext", NULL, NULL, MXML_DESCEND_FIRST);
				if(!temp) {
					perror("While parsing  no <questiontext> tag found");
					getchar();
					return;
				}
				temp = temp->child;/*text tag*/
				if(!temp) {
					perror("While parsing  no <text> inside <questiontext> tag found");
					getchar();
					return;
				}
				if(mxmlGetCDATA(temp->child)) {
					strcpy(qs.ques, removeCDATA(mxmlGetCDATA(temp->child)));/*Questiontext*/
				}
				else {
					strcpy(qs.ques, mxmlGetText(temp->child, 0));
				}
				printf("%s\n Type: %s\n", qs.ques, type);
				naq.qs = qs;
				answer = mxmlFindElement(node, tree, "answer", "fraction", "100", MXML_DESCEND_FIRST);
				if(!answer) {
					perror("While parsing  no <answer> tag found for naq");
					getchar();
					return;
				}
				answer = mxmlFindElement(answer, tree, "text", NULL, NULL, MXML_DESCEND_FIRST);
				if(!answer) {
					perror("While parsing  no <text> tag inside <answer> found for naq");
					getchar();
					return;
				}
				strcpy(tempstr, answer->child->value.text.string);
				naq.ans = atof(tempstr);/*answer tag*/
				fwrite(&naq, sizeof(NAQ), 1, fp);/*Storing NAQ structure*/
				break;
			case _MTP:
				j = 0;
				for(answer = mxmlFindElement(node, tree, "subquestion", NULL, NULL, MXML_DESCEND_FIRST); answer != NULL; answer = mxmlGetNextSibling(answer)) {
					j++;
				}
				
				printf(" Match the pairs\n Type: %s\n No. of pairs: %d\n", type, j);
				
				
				fwrite(&(j), sizeof(int), 1, fp);/*No of pairs tag*/
				
			
				for(answer = mxmlFindElement(node, tree, "subquestion", NULL, NULL, MXML_DESCEND_FIRST); answer != NULL; answer = mxmlGetNextSibling(answer)) {
					temp =  mxmlFindElement(answer, tree, "text", NULL, NULL, MXML_DESCEND_FIRST);/*ques tag*/
					if(!temp) {
						perror("While parsing  no <text> inside <subquestion> tag found");
						getchar();
						return;
					}
					if(mxmlGetCDATA(temp->child)) {/*CDATA*/
						strcpy(pairs.str1, removeCDATA(mxmlGetCDATA(temp->child)));/*Questiontext*/
					}
					else if(mxmlGetText(temp->child, 0)){/*Text*/
						strcpy(pairs.str1, mxmlGetText(temp->child, 0));
					}
					else {
						strcpy(pairs.str1, "-----");
					}
					
					temp2 =  mxmlFindElement(answer, tree, "answer", NULL, NULL, MXML_DESCEND_FIRST);/*ans tag*/
					if(!node) {
						perror("While parsing  no <answer> inside <subquestion> tag found");
						getchar();
						return;
					}
					if(mxmlGetCDATA(temp2->child->child)) {
						strcpy(pairs.str2, removeCDATA(mxmlGetCDATA(temp2->child->child)));/*Questiontext*/
					}
					else {
						strcpy(pairs.str2, mxmlGetText(temp2->child, 0));
					}
					fwrite(&pairs, sizeof(pair), 1, fp);				
				}
				break;
			case _ESSAY:
				temp = mxmlFindElement(node, tree, "questiontext", NULL, NULL, MXML_DESCEND_FIRST);
				if(!temp) {
					perror("While parsing  no <questiontext> tag found");
					getchar();
					return;
				}
				temp = temp->child;/*text tag*/
				if(!temp) {
					perror("While parsing  no <text> inside <questiontext> tag found");
					getchar();
					return;
				}
				if(mxmlGetCDATA(temp->child)) {
					strcpy(qs.ques, removeCDATA(mxmlGetCDATA(temp->child)));/*Questiontext*/
				}
				else {
					strcpy(qs.ques, mxmlGetText(temp->child, 0));
				}
				printf("%s\n  Type: %s\n",qs.ques, type);
				essay.qs = qs;
				temp = mxmlFindElement(node, tree, "graderinfo", NULL, NULL, MXML_DESCEND_FIRST);
				if(!temp) {
					perror("While parsing  no <graderinfo> tag found");
					getchar();
					return;
				}
				temp = temp->child;/*text tag*/
				if(!temp) {
					perror("While parsing  no <text> inside <graderinfo> tag found");
					getchar();
					return;
				}
				if(mxmlGetCDATA(temp->child)) {
					strcpy(essay.graderinfo, removeCDATA(mxmlGetCDATA(temp->child)));/*Questiontext*/
				}
				else {
					strcpy(essay.graderinfo, mxmlGetText(temp->child, 0));
				}
				fwrite(&essay, sizeof(ESSAY), 1, fp);/*essay wrote*/
				break;
			case _TF:
				temp = mxmlFindElement(node, tree, "questiontext", NULL, NULL, MXML_DESCEND_FIRST);
				if(!temp) {
					perror("While parsing  no <questiontext> tag found");
					getchar();
					return;
				}
				temp = temp->child;/*text tag*/
				if(!temp) {
					perror("While parsing  no <text> inside <questiontext> tag found");
					getchar();
					return;
				}
				if(mxmlGetCDATA(temp->child)) {
					strcpy(qs.ques, removeCDATA(mxmlGetCDATA(temp->child)));/*Questiontext*/
				}
				else {
					strcpy(qs.ques, mxmlGetText(temp->child, 0));
				}
				printf("%s\n  Type: %s\n",qs.ques, type);
				tf.qs = qs;
				temp = mxmlFindElement(node, tree, "answer", "fraction", "100", MXML_DESCEND_FIRST);
				if(!temp) {
					perror("While parsing  no <answer> tag with fraction = 100 found");
					getchar();
					return;
				}
				text = mxmlFindElement(temp, tree, "text", NULL, NULL, MXML_DESCEND_FIRST);
				if(!text) {
					perror("While parsing  no <text> tag for correct answer found");
					getchar();
					return;
				}
				strcpy(val, mxmlGetText(text->child, 0));
				if(tolower(val[0]) == 't') {
					tf.ans = 1;
				}
				else {
					tf.ans = 0;
				}
				temp = mxmlFindElement(node, tree, "answer", "fraction", "0", MXML_DESCEND_FIRST);
				if(!temp) {
					perror("While parsing  no <answer> tag with fraction = 0 found");
					getchar();
					return;
				}
				genfd = mxmlFindElement(temp, tree, "feedback", NULL, NULL, MXML_DESCEND_FIRST);
				if(!genfd) {
					perror("While parsing  no <feedback> tag for incorrect answer found");
					getchar();
					return;
				}
				if(mxmlGetCDATA(genfd->child->child)) {
					strcpy(tf.stmt, removeCDATA(mxmlGetCDATA(genfd->child->child)));/*Questiontext*/
				}
				else if(mxmlGetText(genfd->child->child, 0)){
					strcpy(tf.stmt, mxmlGetText(genfd->child->child, 0));
				}
				else {
					strcpy(tf.stmt, "No reason specified.");
				}
				fwrite(&tf, sizeof(TF), 1, fp);/*essay wrote*/	
				break;
				
						
		}
		
		
		
	}
	printf("-------------------------------------------------------------------------------------------------------------------------\n");
	fseek(fp, 0,SEEK_SET);
	printf("Total questions Imported : %d\n", i);
	fwrite(&i, sizeof(int), 1, fp);
	printf("\nTo view imported Question Bank use './project -vqb %s'\nPress ENTER\n", file);
	getchar();
	if(file)
		free(file);
	fclose(fp);
	mxmlDelete(tree);
}

void displayInstructions(StudentInfo si) {
	printheader("Quizzy");
	printmiddle("Instructions");
	printmiddle("============");
	printf("\n\n     1) You are going to solve quiz set named '%s'.\n\n", si.quizname);
	printf("     2) Total marks allocated for this quiz set : '%d'.\n\n", si.total_marks);
	printf("     3) This set contains '%d' questions.\n\n", si.no_of_ques_left);
	printf("     4) Total time allocated '%d minutes'.\n\n", si.totaltime);
	printf("     5) The quiz set contains questions of type \"Multiple choice\", \"Multiple answer\", \"Numerical\", \"Matching pairs\", \"Essay\", \" Ture or False\".\n\n");
	printf("     6) For multiple answer question, enter answer with commas in between. Ex: A, C, E. Negative marking for wrong choice selection\n\n");
	printf("     7) For numerical question, enter only numerical without units. Ex: 1.20003\n\n");
	printf("     8) For matching pairs, enter the pairs separated with '-'. Ex: 1 - A, 2 -C.\n        To not enter a matching pair use '0'. Ex: 1 - 0\n\n");
	printf("     9) For Essay writing, ensure your system has a \"gedit\" editor, otherwise store essay under directory \"Quiz\" with extension \".essay\"\n\n");
	printf("     10) You cannot reattempt a question again.\n\n");
	printf("     11) To Suspend the Quiz use \"Ctrl + t + ENTER\" before answering the question.\n\n");
	printmiddle("~~ALL THE BEST~~");
}
