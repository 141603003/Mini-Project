#define _MCQ 1
#define _MAQ 2
#define _NAQ 3
#define _MTP 4
typedef struct questionString {
	char ques[80];
	int q_length;
}questionString;

typedef struct option {
	char opt_str[20];
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

typedef struct Numerical {
	questionString qs;
	double ans;
}NAQ;

typedef struct pair {
	char str1[20], str2[20];
	struct pair* next;
}pair;

typedef struct Match_the_pairs {
	int no_of_pairs;
	pair* pairs;		
}MTP;

union question {
	MCQ mcq;
	MAQ maq;
	NAQ naq;
	MTP mtp;
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

void init(QuestionBank* qb);
void initquizset(QuizSet* qz);
void addQuestion(QuestionBank* qb, int* n);
void storeQB(QuestionBank* qb, char* file, int storemarks);
void addOption(option_ptr* s);
void readQBstruct(QuestionBank* qb, int showMarks/*boolean flag*/);
void readQB(char* file_name);
void addPairs(MTP* mtp);
void readQBQuiz(QuizSet* quizset, char* file_name);
void addPairsQuiz(MTP* mtp, pair* p);
void addOptionQuiz(option_ptr* opt_ptr, char* str);
