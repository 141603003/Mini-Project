/********************************************************************************
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
#ifndef __QUESTION_H
#define __QUESTION_H
#define _MCQ 1
#define _MAQ 2
#define _NAQ 3
#define _MTP 4
#define _ESSAY 5
#define _TF 6
typedef struct questionString {
	char ques[100];
	int q_length;
}questionString;

typedef struct option {
	char opt_str[50];
	int opt_length;
	struct option* next;
}option;

typedef struct MCQ {
	questionString qs;
	int no_of_opt;
	option* opts;
	char ans;
	 
}MCQ;

typedef struct MAQ {
	questionString qs;
	int no_of_opt;
	option* opts;
	char* ans; /*depends on no_of_option*/	
	
}MAQ;

typedef struct Essay{
	questionString qs;
	char graderinfo[100];
}ESSAY;

typedef struct Numerical {
	questionString qs;
	double ans;
}NAQ;

typedef struct pair {
	char str1[50], str2[50];
	struct pair* next;
}pair;

typedef struct Match_the_pairs {
	int no_of_pairs;
	pair* pairs;		
}MTP;

typedef struct TF{
	questionString qs;
	int ans;
	char stmt[100];
}TF;/*true or false*/
union question {
	MCQ mcq;
	MAQ maq;
	NAQ naq;
	MTP mtp;
	ESSAY essay;
	TF truefalse;
};
typedef struct option_ptr {
	option* start;
}option_ptr;
typedef struct Question {
	int type_of_ques, id;
	union question ques;
}Question;

typedef struct question_ll {
	Question q;
	int marks;
	struct question_ll* next;	
}question_ll;

typedef struct QuestionBank {
	question_ll* start;/**pointer to starting node*/
	question_ll* curr ;/*pointer to current node*/	
	int no_of_question;
}QuestionBank;

typedef QuestionBank Quiz;

typedef struct QuizSet {
	Quiz quiz;
	int total_marks;
	int time;
	
}QuizSet;

typedef struct StudentInfo {
	int finished;	
	char ID[20];
	char quizname[20];
	int no_of_ques_left;
	float score;
	char filename[50];
	int totalques;
	int totaltime;
	int total_marks;
}StudentInfo;
	
void init(QuestionBank* qb);
void initquizset(QuizSet* qz);
void addQuestion(QuestionBank* qb, int* n);
void storeQB(QuestionBank* qb, char* file, int storemarks);
void storeQuizInfo(QuizSet* qs, char* s);
void addOption(option_ptr* s);
void readQBstruct(QuestionBank* qb, int showMarks/*boolean flag*/);
void readQB(char* file_name, int readmarks);
void readQuizSet(char* file);
void addPairs(MTP* mtp);
void readQBQuiz(QuizSet* quizset, char* file_name);
void addPairsQuiz(MTP* mtp, pair* p);
void addOptionQuiz(option_ptr* opt_ptr, char* str);
question_ll* fetchquiz( char* filename);
void fetchquizinfo(StudentInfo* a, char* s);
char* runquiz(StudentInfo* studentinfo, question_ll* ques);
void terminateQuiz(StudentInfo* studentinfo, question_ll* ques, int j);
void storeLeftQuestions(question_ll* ques, char* filename, int curr, int end);
char* storeStudentInfo(StudentInfo* studentinfo);/*stores and tell where to store left question*/
int getNoOfPendingQuiz(StudentInfo si);
char* selectPendingQuiz(StudentInfo si, int no, int restore);
void modifyStudentInfo(StudentInfo* si, char* file);
void import(char* filename) ;
char* convertintoString(int a);
int found(int* arr, int n, int find);
void export2(char* filename, int tpresent, char* target);
char* getfilenameimport(char* str);
int getTypeFromString(char* type,char* single) ;
void import2(char* filename) ;
char* removeCDATA(const char* str);
char* removeExtension(char* file);
void removeWhitespaceFromFile(char* file);
void displayInstructions(StudentInfo si);
void freeQuestionLinkedList(question_ll* start);/*Functions for releasing allocated memory*/
void freeMCQ_MAQ(Question* q);
void freeOptions(option* start);
void freePairs(pair* start);
#endif
