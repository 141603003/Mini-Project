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
#ifndef __COMMANDLINE_H
#define __COMMANDLINE_H

typedef struct CMLStates{
	int cmd; /*general*//*0 or 1*/
	int optionarray[30];
	int no_of_options;
	int pointer;
	int cbq;/*-cqb*/
	char* cqbfilename;
	int no_of_cqbfiles;/*0 or 1*/
	int vqb;/*-vqb*/
	char* vqbfilename[10];
	int no_of_vqbfiles;
	int cq;/*-cq*/
	char* cqfilename;
	int no_of_cqfiles;/*0 or 1*/
	int vq;/*-vq*/
	char* vqfilename[10];
	int no_of_vqfiles;
	int x;/*-x*/
	char* source;
	char* target;
	int spresent, tpresent;
	int i;/*-i*/
	char* importfilename;
	int run;/*-run*/
	char* runfilename;
	int no_of_runfiles;/*0 or 1*/
	int p, r;/*-p -r*/
	char* ID;
	
}CMLStates;

void initCMLStates(CMLStates* );
void createStates(CMLStates*, int argc, char* argv[]);
int isOption(char* str);
#endif
