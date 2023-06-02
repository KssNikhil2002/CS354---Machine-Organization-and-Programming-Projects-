////////////////////////////////////////////////////////////////////////////////
// Main File:        division.c
// This File:        division.c
// Other Files:      mySigHandler.c and sendsig.c.
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
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//counter to keep track of the number of completed divisions
int succ_divisions = 0;

/* handler_SIGFPE: 
 * A signal handler function that handles the 
 * SIGFPE signal 
 *
 * It prints a message stating that a divide by 0 
 * operation was attempted.
 * It prints the number of successfully completed 
 * division operations. 
 */
void handler_SIGFPE() 
{
	
    printf("Error: a division by 0 operation was attempted.\n");
    //printing the number of successfull division operations 
    printf("Total number of operations completed successfully: %i\n", succ_divisions);
    printf("The program will be terminated.\n");
    exit(0);
    
}

/* handler_SIGINT: 
 * A signal handler function that handles the 
 * SIGINT signal 
 *
 * prints the number of successfully completed division 
 * operations, and then exit the program using exit(0).
 */
void handler_SIGINT() 
{
    printf("\n");
    ////printing the number of successfull division operations 
    printf("Total number of operations completed successfully: %i\n", succ_divisions);
    printf("The program will be terminated.\n");
    exit(0);
    
}

/*
 * main:
 * It accepts two numbers from the terminal,
 * calcluates the quotient and remainder by 
 * performing an integer division and prints the 
 * results 
 * It also handles divde by 0 expceptions. 
 */ 
int main(int argc, char *argv[])
{
	char buffer[100];
	int num1 = 0; // varibale to store the first integer value
	int num2 = 0; //variable to store the second integer value
	int quotient = 0; //varibale that stores the quotient 
	int rem = 0; // variable that stores the remainder 
	
	//creating a variable of sigaction to register the signal handler 
	struct sigaction Sfpe;
	//initializing sigaction struct so that it is cleared 
    memset(&Sfpe, 0, sizeof(Sfpe));
    Sfpe.sa_handler = handler_SIGFPE;
	//cheking if the binding of the signal handler takes place properly 
    if(sigaction(SIGFPE, &Sfpe, NULL) != 0) 
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
    
    ///infinite loop
    while(1)
    {
    	printf("Enter first integer: ");
    	//reads the input of the first integer entered on the terminal 
    	if(fgets(buffer, 100, stdin) != NULL)
    	{
    		// stores the first integer in the num1 variable
			num1 = atoi(buffer); 
		}	
		else 
		{
	    	printf("Error.");
	    	exit(0);
		}
		
		printf("Enter second integer: ");
		//reads the input of the second integer entered on the terminal
    	if(fgets(buffer, 100, stdin) != NULL)
    	{
    		// stores the second integer in the num2 variable
			num2 = atoi(buffer);
		}	
		else 
		{
	    	printf("Error.");
	    	exit(0);
		}
		
		//performing the division operation
		quotient = num1 / num2;
		//storing the remainder of the two numbers 
        rem = num1 % num2;
        
        printf("%i / %i is %i with a remainder of %i\n", num1, num2, quotient, rem);
        
        //increments the counter 
        succ_divisions++; 
	}
	
	return 0;	
	
	
}
