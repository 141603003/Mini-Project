main: main.o question_ll.o print.o
	cc  main.o question_ll.o print.o -o main
main.o: main.c question.h print.h
	cc -c main.c
question_ll.o: question_ll.c question.h
	cc -c question_ll.c
print.o: print.c print.h
	cc -c print.c
