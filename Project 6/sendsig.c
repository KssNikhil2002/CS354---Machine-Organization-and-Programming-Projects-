////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c
// This File:        sendsig.c
// Other Files:      division.c and mySigHandler.c 
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
  
/*
 * main:
 * It reads the signal type and process id from the terminal
 * and sends a user defined signal using Kill()
 * to the process with particular pid. 
 */
int main(int argc, char* argv[])
{
	//checking if the number of CLA is 3
    if(argc != 3) 
	{
		printf("Usage: sendsig <signal type> <pid>");
		exit(1);
    }
    
    //checking if the signal type is "-i"(SIGINT)
    if(strcmp(argv[1],"-i") == 0)
    {
    	//checking if the user signal is being sent accuratley 
    	if(kill(atoi(argv[2]) , SIGINT) == -1)
    	{
    		printf("Error.\n");
    		exit(0);
		}
		
	}
	
	//checking if the signal type is "-u"(SIGUSR1)
	else if(strcmp(argv[1],"-u") == 0)
    {
    	//checking if the user signal is being sent accuratley 
    	if(kill(atoi(argv[2]) , SIGUSR1) == -1)
    	{
    		printf("Error.\n");
    		exit(0);
		}
		
	}
	//when the signal type is neither "-i" or "-u"
	else
	{
		printf("Usage: sendsig <signal type> <pid>");
		exit(0);
	}
}
