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
#include "question.h"
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
#define RUNQZ 8
#define PENDINGQZ 9 
#define CHANGETOADMIN 10
#define CHANGETOSTUDENT 7

void initialization();
void printheader(char* str);
void initial();
void printmiddle(char* str);
int loginprint();
void validation(int choice, char* si);
void validator();
int printmenu(int choice);
void availablefilename();
char* getfilename();
void shuffle(question_ll* q, int n);
void shufflepairs(pair* q, int n);
char* checkforterminate(char* str);
#endif
