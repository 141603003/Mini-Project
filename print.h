#ifndef __PRINT_H
#define __PRINT_H
#define ADMIN 1
#define STUDENT 2
#define CREATEQB 1
#define VIEWQB 2
#define CREATEQZ 3
#define VIEWQZ 4
#define EXPORT 5
#define IMPORT 6
#define RUNQZ 7
#define PENDINGQZ 8 
void initialization();
void printheader(char* str);
void initial();
void printmiddle(char* str);
int loginprint();
void validation(int choice);
void validator();
int printmenu(int choice);
void availablefilename();
char* getfilename();
#endif
