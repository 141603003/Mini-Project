Title: Quizzy : A command-line quiz.

Name: Vinay Prakash Desai

MIS No: 141603003

Quizzy is a program which allows one to do following:
1) Create a new question bank
2) Store question bank and load it.
3) Create new quiz from question bank
4) View created quiz sets.
5) Run a quiz
6) Suspend a quiz in between
7) View number of pending quiz set for a user.
8) Resume a suspended quiz
9) Export a created question bank to Moodle XML Format.
10) Import a Moodle XML Format question bank.
	Quizzy works on terminal environment. It can be used with options, to perform specific task or used in an interactive mode (./project ).
Interactive mode consists of Menu with login type. Following is the work done in each module:

1)Creation of Question Bank: 
	Quizzy supports: 
		1) Multiple choice question. (MCQ) with unlimited options
		2) Multiple Answer question. (MAQ) with unlimited options
		3) Numerical. (NAQ)
		4) Match the pairs. (MTP) with unlimited no. of pairs
		5) Essay Writing. (ESSAY)
		6) True or False. (T or F)
	For storing unlimited options, pair and questions linked list is used as data structure. Each question Bank is stored as a Binary file with extension ".qb". This Question bank can be accessed by admin anytime.

2)Creation of Quiz:
	Quizzy allows one to create a quiz set by using one question bank. As well as allocate marks while creating quiz set. Each quiz set is stored as binary file with extension ".quiz" and corresponding information of quiz set is stored in ".info" file.

3)Exporting and Importing: 
	I have used "mini-xml" library for parsing and creating xml file. Quizzy is designed to work with Moodle XML Format. If any problem in importing or exporting quizzy points out the problems. While exporting binary file is translated into xml file.

4)Running a quiz set:
	The created quiz set can be attempted by a user with specific ID(it can be anything). The questions get shuffled up. The pairs of "Match the pairs" also gets shuffled. For multiple answer(MAQ), negative marking is defined. The quiz can be suspended in between while attempting by using (Ctrl + t) followed by ENTER key.

5)Resuming the quiz:
   	The quiz which was suspended using (Ctrl+t+ENTER) can be resumed with left over questions. Here quizzy uses ".state" file and ".left.ques" file. While attempting the suspended quiz, the user can again terminate th quiz using(Ctrl + t +ENTER).

Each and every information required for quizzy is stored by using granule leveled data structures, (like for question text in each question, questionString struct is used). Focus is given to input validation while entering data. All the memory allocated by the malloc function is properly been released using a set of function. Same features available with option mode is available with interactive mode. In interactive mode for exiting (ESC key ) is used.

	The "Information about test cases" file describes the test cases. The Screenshoot folder contains screenshots of both interactive mode and command line option mode. "datastructure.jpg" contains C structures used in project (blue color), way of storing data in files(.qb, .state) (greeen color) with a small example. 

					~~THANK You~~   
