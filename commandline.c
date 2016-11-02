/*****************************************************************************
 * Defines the command line behaviour of quizzy software.
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
#include <string.h>
#include <stdlib.h>
#include "print.h"
#include "commandline.h"

void initCMLStates(CMLStates* s) {
	s->cmd = 0;
	s->pointer = 0;
	s->no_of_options = 0;
	s->cbq = 0;
	s->cqbfilename = NULL;
	s->no_of_cqbfiles = 0;/*0 or 1*/
	s->vqb = 0;/*-vqb*/
	s->no_of_vqbfiles = 0;
	s->cq = 0;/*-cq*/
	s->cqfilename = NULL;
	s->no_of_cqfiles = 0;/*0 or 1*/
	s->vq = 0;/*-vq*/
	s->no_of_vqfiles = 0;
	s->x = 0;/*-x*/
	s->source = NULL;
	s->target = NULL;
	s->spresent = 0;
	s->tpresent = 0;
	s->i = 0;/*-i*/
	s->importfilename = NULL;
	s->run = 0;/*-run*/
	s->runfilename = NULL;
	s->no_of_runfiles = 0;/*0 or 1*/
	s->p = 0;
	s->r = 0;/*-p -r*/
	s->ID = NULL;
}
void createStates(CMLStates* s, int argc, char* argv[]) {
	int i, count = 0, k, curr;
	if(argc == 1) {/*No options*/
		s->cmd = 0;
		return;
	}


	for(i = 1; i < argc; i++) {
		if(!isOption(argv[i])) {
			printf("project: extra argument -- '%s' ignored\n(Press Enter key)\n", argv[i]);
			getchar();
			continue;
		}
		if(!strcmp(argv[i], "-cqb")) {
			
			s->cbq = 1;
			if(((i + 1) < argc) && !isOption(argv[i + 1])) {
				s->no_of_cqbfiles = 1;
				i++;
				s->cqbfilename = (char*) malloc(sizeof(char) * (strlen(argv[i]) + 1));
				strcpy(s->cqbfilename, argv[i]);
			}
			s->optionarray[count] = CREATEQB;
			(s->no_of_options)++;
			count++;
		}
		else if(!strcmp(argv[i], "-vqb")) {
			
			s->vqb = 1;
			k = 0;
			curr = i;
			while(((i + 1) < argc) && !isOption(argv[i + 1])) {
				i++;
				if(k >= 10) {
					printf("Maximum 10 question bank are allowed to view. Only 10 parameters accepted.\n(Press ENTER)");
					getchar();
					break;
				}
				s->vqbfilename[k] = (char*) malloc(sizeof(char) * (strlen(argv[i]) + 1));
				strcpy(s->vqbfilename[k], argv[i]);
				k++;
				s->optionarray[count] = VIEWQB;
				(s->no_of_options)++;
				count++;
			}
			if(i == curr){
				s->optionarray[count] = VIEWQB;
				(s->no_of_options)++;
				count++;
			}
			s->no_of_vqbfiles = k;
			
		}
		else if(!strcmp(argv[i], "-cq")) {
					
			s->cq = 1;
			if(((i + 1) < argc) && !isOption(argv[i + 1])) {
				s->no_of_cqfiles = 1;
				i++;
				s->cqfilename = (char*) malloc(sizeof(char) * (strlen(argv[i]) + 1));
				strcpy(s->cqfilename, argv[i]);
			}
			s->optionarray[count] = CREATEQZ;
			(s->no_of_options)++;
			count++;
		}
		else if(!strcmp(argv[i], "-cqvqb") || !strcmp(argv[i], "-vqbcq") ) {
					
			s->vqb = 1;
			s->cq = 1;
			curr = i;
			if(((i + 1) < argc) && !isOption(argv[i + 1])) {
				s->no_of_cqfiles = 1;
				
				s->cqfilename = (char*) malloc(sizeof(char) * (strlen(argv[i + 1]) + 1));
				strcpy(s->cqfilename, argv[i + 1]);
			}
			s->optionarray[count] = CREATEQZ;
			(s->no_of_options)++;
			count++;
			k = 0;
			while(((i + 1) < argc) && !isOption(argv[i + 1])) {
				i++;
				if(k >= 10) {
					printf("Maximum 10 question bank are allowed to view. Only 10 parameters accepted.\n(Press ENTER)\n");
					getchar();
					break;
				}
				s->vqbfilename[k] = (char*) malloc(sizeof(char) * (strlen(argv[i]) + 1));
				strcpy(s->vqbfilename[k], argv[i]);
				k++;
				s->optionarray[count] = VIEWQB;
				(s->no_of_options)++;
				count++;
			}
			if(i == curr){/*No filename*/
				s->optionarray[count] = VIEWQB;
				(s->no_of_options)++;
				count++;
			}
			s->no_of_vqbfiles = k;
			
		}
		else if(!strcmp(argv[i], "-vq")) {
			s->vq = 1;
			
			k = 0;
			curr = i;
			while(((i + 1) < argc) && !isOption(argv[i + 1])) {
				i++;
				if(k >= 10) {
					printf("Maximum 10 quiz sets are allowed to view. Only 10 parameters accepted.\n(Press ENTER)\n");
					getchar();
					break;
				}
				s->vqfilename[k] = (char*) malloc(sizeof(char) * (strlen(argv[i]) + 1));
				strcpy(s->vqfilename[k], argv[i]);
				k++;
				s->optionarray[count] = VIEWQZ;
				(s->no_of_options)++;
				count++;
				
			}
			if(i == curr){/*No filename*/
				s->optionarray[count] = VIEWQZ;
				(s->no_of_options)++;
				count++;
			}
			s->no_of_vqfiles = k;
			
		}
		else if(!strcmp(argv[i], "-x")) {
			s->x = 1;
			if(((i + 1) < argc) && !isOption(argv[i + 1])) {
				s->spresent = 1;
				i++;
				s->source = (char*) malloc(sizeof(char) * (strlen(argv[i]) + 1));
				strcpy(s->source, argv[i]);
			}
			if(((i + 1) < argc) && !isOption(argv[i + 1])) {
				s->tpresent = 1;
				i++;
				s->target = (char*) malloc(sizeof(char) * (strlen(argv[i]) + 1));
				strcpy(s->target, argv[i]);
			}
			s->optionarray[count] = EXPORT;
			(s->no_of_options)++;
			count++;
		}
		else if(!strcmp(argv[i], "-i")) {
		
			s->i = 1;
			if(((i + 1) < argc) && !isOption(argv[i + 1])) {

				i++;
				s->importfilename = (char*) malloc(sizeof(char) * (strlen(argv[i]) + 1));
				strcpy(s->importfilename, argv[i]);
			}
			else {
				printf("Usage: ./project -i <xml_filename>\n");
				exit(1);
			}
			s->optionarray[count] = IMPORT;
			(s->no_of_options)++;
			count++;
		}
		else if(!strcmp(argv[i], "-run")) {
			s->run = 1;
			
			if(((i + 1) < argc) && !isOption(argv[i + 1])) {
				s->no_of_runfiles = 1;
				i++;
				s->runfilename = (char*) malloc(sizeof(char) * (strlen(argv[i]) + 1));
				strcpy(s->runfilename, argv[i]);
			}
			s->optionarray[count] = RUNQZ;
			(s->no_of_options)++;
			count++;

		}
		else if(!strcmp(argv[i], "-p")) {
			s->p = 1;
			if(((i + 1) < argc) && !isOption(argv[i + 1])) {
				i++;
				s->ID = (char*) malloc(sizeof(char) * (strlen(argv[i]) + 1));
				strcpy(s->ID, argv[i]);
			}
			else {
				printf("Usage: ./project -p <ID>\n");
				exit(1);
			}
			s->optionarray[count] = PENDINGQZ;
			(s->no_of_options)++;
			count++;
		}
		else if(!strcmp(argv[i], "-r")) {
			s->r = 1;
			if(((i + 1) < argc) && !isOption(argv[i + 1])) {
				i++;
				s->ID = (char*) malloc(sizeof(char) * (strlen(argv[i]) + 1));
				strcpy(s->ID, argv[i]);
			}
			else {
				printf("Usage: ./project -r <ID>\n");
				exit(1);
			}
			s->optionarray[count] = PENDINGQZ;
			(s->no_of_options)++;
			count++;
		}
		else {
			printf("project: invalid option -- '%s'\nTry './project -h' for more information\n", argv[i]);
			exit(1);
		}
		
	}
	
	if(count > 0) {
		s->cmd = 1;
	}
}
int isOption(char* str) {
	if(str[0] == '-')
		return 1;
	return 0;
}
