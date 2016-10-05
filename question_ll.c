#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
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
	char temp, ch;
	question_ll* node = (question_ll*)malloc(sizeof(question_ll));
	printf("Q%d.\n", (*id) + 1);
	printf("Enter the type of Question: %45s","[MCQ 1] [MAQ 2] [NAQ 3] [MTP 4]  \n");
	scanf("%d", &t);
	(node->q).type_of_ques = t;/*Question type*/
	(node->q).id = ++(*id);/*Question ID*/
	switch(t) {
		case _MCQ:/*MCQ*/
		case _MAQ:
			printf("\nEnter the question text :\n");
			getchar();
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
						(node->q).ques.maq.ans[j] = ch;
						j++;
					}					
				}
				(node->q).ques.maq.ans[j] = '\0';
				
			}
			else {/*for MCQ*/
				printf("\nCorrect option(label) : \n");
				scanf("%c", &((node->q).ques.mcq.ans));/*Answer*/		
			}
				
			break;
		case _NAQ:
			printf("\nEnter the question string:\n");
			getchar();
			scanf("%[^\n]%*c", (node->q).ques.naq.qs.ques);/*Reading question*/
			(node->q).ques.naq.qs.q_length = strlen((node->q).ques.naq.qs.ques);/*storing length of question*/
			printf("Enter correct answer :\n");
			scanf("%lf", &((node->q).ques.naq.ans));
			break;
		case _MTP:
			printf("How many pairs:");
			scanf("%d", &no);
			(node->q).ques.mtp.no_of_pairs = no;
			(node->q).ques.mtp.pairs = NULL;
			for(i = 0; i < no; i++) {
				printf("%c:", (i+1+64));
				addPairs(&((node->q).ques.mtp));			
			}
			break;
			
		
				
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
	char str1[20], str2[20];
	pair* node = malloc(sizeof(pair));
	pair* p;
	scanf("%s", str1);
	scanf("%s", str2);
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
	char str[20];
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

void readQBstruct(QuestionBank* qb, int showMarks/*boolean flag*/) {
	MCQ mcq;
	MAQ maq;
	NAQ naq;
	MTP mtp;
	pair* pair;
	questionString qs;
	option* opts;
	question_ll* p = qb->start;
	while(p != NULL) {
		printf("%d: ",(p->q).id);
		switch((p->q).type_of_ques) {
			case _MCQ: printf("MCQ : ");
				mcq = (p->q).ques.mcq;
				qs = mcq.qs;
				printf("%s\n", qs.ques);
				if(!mcq.opts)
					printf("Options are not there");
				opts = mcq.opts;
				
				while(opts != NULL) {
					printf("%s\t", opts->opt_str);
					opts = opts->next;				
				}
				printf("\nCorrect ans: %c\n\n", mcq.ans);
				break;
			case _MAQ:
				printf("MAQ : ");
				maq = (p->q).ques.maq;
				qs = maq.qs;
				printf("%s\n", qs.ques);
				opts = maq.opts;
				if(!opts)
					printf("Options are not there");
				while(opts != NULL) {
					printf("%s\t", opts->opt_str);
					opts = opts->next;				
				}
				printf("\nCorrect ans: %s\n\n", maq.ans);
				break;
			case _NAQ:
				naq = (p->q).ques.naq;
				qs = naq.qs;
				printf("%s\n", qs.ques);
				printf("Correct ans: %lf\n\n", naq.ans);
				break;	
			case _MTP:
				mtp = (p->q).ques.mtp;
				printf("No of pairs: %d\n", mtp.no_of_pairs);
				pair = mtp.pairs;
				
				while(pair != NULL) {
					printf("%s\t%s\n", pair->str1, pair->str2);
					pair = pair->next;				
				}
				printf("\n");
				break;	
		}
		if(showMarks) {
			printf("%50s......Marks[%d]\n", "", p->marks);		
		}
		p = p->next;
		
	}	
}
void storeQB(QuestionBank* qb, char* file) {
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
					fwrite(&(opts->opt_str), sizeof(char) * 20, 1, fp);/*Storing options*/
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
					fwrite(&(opts->opt_str), sizeof(char) * 20, 1, fp);
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
						
		}
		p = p->next;
	}
	printf("Written successfully to %s\n", file);
	getchar();
	fclose(fp);
	
}
void readQB(char* file_name) {
	FILE* fp = fopen(file_name, "r");
	pair* p;
	int i, j, k = 0;
	questionString qs;
	NAQ naq;
	char str[20], ch;
	printf("%s\n", file_name);
	if(!fp) {
		perror("Cant open file ");
		return;
	}
	fread(&i, sizeof(int), 1, fp);/*no of ques*/
	printf("%s contains %d number of questions\n\n", file_name, i);
	while(fread(&i, sizeof(int), 1, fp) != 0) {/*question ID*/
		printf("Q%d. ", i);
		fread(&i, sizeof(int), 1, fp);/*ques type*/
		switch(i) {
			case _MCQ: 
				fread(&qs,sizeof(struct questionString), 1, fp);/*Storing questionString structure*/
				printf("%s\n", qs.ques);
				fread(&i, sizeof(int), 1, fp);/*no of options*/
				j = 0;
				while(j < i) {

					fread(&str, sizeof(char) , 20, fp);/*Reading options*/
					printf("%c. %s\t", (j + 1 + 64), str);	
					j++;
							
				}
				fread(&ch, sizeof(char), 1, fp);
				
				break;
			case _MAQ:
				fread(&qs,sizeof(struct questionString), 1, fp);/*Storing questionString structure*/
			
				printf("%s\n", qs.ques);
				
				fread(&k, sizeof(int), 1, fp);/*no of options*/
				j = 0;
				while(j < k) {

					fread(&str, sizeof(char) , 20, fp);/*Reading options*/
					printf("%c. %s\t", (j + 1 + 64), str);	
					j++;
							
				}
				j = 0;
			/*	do {
					fread(&ch, sizeof(char), 1, fp);/*reading ans till '\0'*/
			/*		str[j] = ch;
					j++;
				}while(ch != '\0');
				printf("\n%s\n", str);	*/			
				break;
			case _NAQ:
				fread(&naq, sizeof(NAQ), 1, fp);
				qs = naq.qs;
				printf("%s \n", qs.ques);
				break;	
			case _MTP:
				fread(&i, sizeof(int), 1,fp);
				printf("Match the following pairs:\n");
				p = (pair*)malloc(sizeof(pair));
				//if(p == NULL) printf("Memory allocated");
				for(j = 0; j < i; j++) {
					printf("%c:", (j+1+64));
					fread(p, sizeof(pair), 1, fp);
					printf("%s\t%s\n", p->str1, p->str2);			
				}
				break;	
		}
		printf("\n\n");
	}
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
	int i, j, k = 0, countq = 0/*Question counter*/, marks, totalmarks = 0;
	questionString qs;
	option_ptr opt_ptr;
	NAQ naq;
	char str[20], ch, select;
	if(!fp) {
		perror("Cant open file ");
		return;
	}
	fread(&i, sizeof(int), 1, fp);/*no of ques*/
	printf("%s contains %d number of questions\n\n", file_name, i);
	getchar();
	while(fread(&i, sizeof(int), 1, fp)) {/*question ID*/
		printheader("Quizzy");
		printmiddle("Creation of Quiz");
		printmiddle(file_name);
		printf("Q%d. ",i);
		question_ll* node = (question_ll*)malloc(sizeof(question_ll));
		(node->q).id = (countq + 1);/*Question ID*/
		fread(&i, sizeof(int), 1, fp);/*ques type*/
		(node->q).type_of_ques = i;/*Question type*/
		switch(i) {
			case _MCQ: 
				fread(&qs,sizeof(struct questionString), 1, fp);/*Reading questionString structure*/
				printf("%s\n", qs.ques);
				(node->q).ques.mcq.qs = qs;
				fread(&i, sizeof(int), 1, fp);/*no of options*/
				(node->q).ques.mcq.no_of_opt = i;/*No of options*/
				/*option list*/
				opt_ptr.start = NULL;
				j = 0;
				while(j < i) {

					fread(&str, sizeof(char) , 20, fp);/*Reading options*/
					printf("%c. %s\t", (j + 1 + 64), str);	
					addOptionQuiz(&opt_ptr, str);
					j++;
							
				}
				(node->q).ques.mcq.opts = opt_ptr.start;/*adding option*/
				fread(&ch, sizeof(char), 1, fp);
				(node->q).ques.mcq.ans = ch;/*Answer*/
				break;
			case _MAQ:
				fread(&qs,sizeof(struct questionString), 1, fp);/*Reading questionString structure*/
				printf("%s\n", qs.ques);
				(node->q).ques.mcq.qs = qs;
				fread(&i, sizeof(int), 1, fp);/*no of options*/
				(node->q).ques.mcq.no_of_opt = i;/*No of options*/
				option_ptr opt_ptr;/*option list*/
				opt_ptr.start = NULL;
				j = 0;
				while(j < i) {

					fread(&str, sizeof(char) , 20, fp);/*Reading options*/
					printf("%c. %s\t", (j + 1 + 64), str);	
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
				printf("%s (%d)\n", qs.ques, qs.q_length);
				(node->q).ques.naq = naq;
				break;	
			case _MTP:
				fread(&i, sizeof(int), 1,fp);/*no of pairs*/
				(node->q).ques.mtp.no_of_pairs = i;
				(node->q).ques.mtp.pairs = NULL;
				printf("Match the following pairs:\n");
				p = (pair*)malloc(sizeof(pair));
				//if(p == NULL) printf("Memory allocated");
				for(j = 0; j < i; j++) {
					printf("%c:", (j+1+64));
					fread(p, sizeof(pair), 1, fp);
					printf("%s\t%s\n", p->str1, p->str2);
					addPairsQuiz(&((node->q).ques.mtp), p); /*Adding pairs*/
								
				}
				break;	
		}
		printf("\n\n");
		printf("Select above question: (y|n) \n");
		scanf("%c", &select);
		getchar();
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
		printf("Allocate Max Time for this quiz:\n(minutes)");
		scanf("%d", &(quizset->time));
		getchar();
	}
	getchar();
	fclose(fp);
}

