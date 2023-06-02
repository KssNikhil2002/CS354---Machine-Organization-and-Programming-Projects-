////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c
// This File:        mySigHandler.c
// Other Files:      division.c and sendsig.c.
// Semester:         CS 354 Lecture 001 Spring 2023
// Instructor:       deppeler
// 
// Author:           Kruthiventi Shyama Subrahmanya Nikhil
// Email:            skruthiventi@wisc.edu
// CS Login:         shyama
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
//////////////////////////// 80 columns wide ///////////////////////////////////
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <time.h>

//counter to keep tally of the number of times it receives SIGUSR1
int usr_counter = 0; 
// number of seconds for the alarm 
int seconds = 4; 

/* handler_SIGALRM: 
 * A signal handler function that handles the 
 * SIGALRM signal 
 *
 * It prints the pid(process id) of the program 
 * and the current time. It also re-arms or restarts 
 * a new alarm
 */
void handler_SIGALRM()
{	
	time_t currenttime; //varible to store the current time 
  	currenttime = time(NULL);
	if((ctime(&currenttime) != NULL) && currenttime!= -1)
  	{
  		//printing the proccess id and the current time
		printf("PID: %i CURRENT TIME: %s", getpid(), ctime(&currenttime));					
	}  
	else
	{
		printf("Error");
	}
	// setting an alarm that will go of "seconds(4)' later
	alarm(seconds);
}

/* handler_SIGUSR1: 
 * A signal handler function that handles the 
 * SIGUSR1 signal 
 *
 * It prints a message on receiving 
 * a SIGUSR1 signal and also increments a global counter to 
 * keep tally of the number of times it receives SIGUSR1
 */
void handler_SIGUSR1()
{
	printf("SIGUSR1 handled and counted!\n");
	//incrementing the counter everytime it recieves a user 
	//defined signal(SIGUSR1)
	usr_counter++;
}

/* handler_SIGINT: 
 * A signal handler function that handles the 
 * SIGINT signal 
 *
 * It prints the number of times it received 
 * the SIGUSR1 signal and then calls exit(0)
 */
void handler_SIGINT()
{
	printf("\n");
	printf("SIGINT handled.\n");
	//printing the number of times it received the SIGUSR1 signal
	printf("SIGUSR1 was handled %d times. Exiting now.\n", usr_counter);
	exit(0);
}


/*
 * main:
 * The main fucntion handles three different types of 
 * signals namely a periodic signal from an alarm, 
 * a keyboard interrupt signal and a user defined signal.
 * It also sets an alarm that will go off 4 seconds later.  
 */ 
int main()
{
	//creating a variable of sigaction to register the signal handler 
	struct sigaction SAlrm;
	//initializing sigaction struct so that it is cleared 
	memset(&SAlrm, 0, sizeof(SAlrm));

	SAlrm.sa_handler = handler_SIGALRM;
	
	//cheking if the binding of the signal handler takes place properly 
	if(sigaction(SIGALRM, &SAlrm, NULL) != 0)
	{
        printf("Failed to set signal handler.\n");
        exit(1);
    }
    
    //creating a variable of sigaction to register the signal handler
    struct sigaction Sint;
    //initializing sigaction struct so that it is cleared 
	memset(&Sint, 0, sizeof(Sint));
	Sint.sa_handler = handler_SIGINT;
	
	//cheking if the binding of the signal handler takes place properly 
	if(sigaction(SIGINT, &Sint, NULL) != 0)
	{
        printf("Failed to set signal handler.\n");
        exit(1);
    }
    
    //creating a variable of sigaction to register the signal handler 
    struct sigaction Susr;
    //initializing sigaction struct so that it is cleared 
	memset(&Susr, 0, sizeof(Susr));
	Susr.sa_handler = handler_SIGUSR1;
	
	//cheking if the binding of the signal handler takes place properly 
	if(sigaction(SIGUSR1, &Susr, NULL) != 0)
	{
        printf("Failed to set signal handler.\n");
        exit(1);
    }
    
    printf("PID and time print every 4 seconds.\n");
    printf("Type Ctrl-C to end the program.\n");
    
    // setting an alarm that will go of "seconds(4)' later
    alarm(seconds);
    
    //infinite loop
	while(1);

	return 0;
}
