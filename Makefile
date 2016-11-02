project: main.o question_ll.o print.o commandline.o
	cc  main.o question_ll.o print.o commandline.o -lmxml -lpthread -o project -Wall
main.o: main.c question.h print.h
	cc -c main.c -Wall
question_ll.o: question_ll.c question.h
	cc -c question_ll.c 
print.o: print.c print.h
	cc -c print.c -Wall
commandline.o: commandline.c commandline.h print.h
	cc -c commandline.c -Wall
clean: 
	rm -f *.o *.xml project
	rm -rf Export_Files Quiz QuestionBank Info
