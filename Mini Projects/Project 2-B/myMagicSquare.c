///////////////////////////////////////////////////////////////////////////////
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission, CS 354 Spring 2022, Deb Deppeler
////////////////////////////////////////////////////////////////////////////////
   
// add your own File Header information here (as provided in p2A or Commenting guide)
////////////////////////////////////////////////////////////////////////////////
// Main File:        myMagicSquare.c
// This File:        myMagicSquare.c
// Other Files:      (name of all other files if any)
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure that represents a magic square
typedef struct {
    int size;           // dimension of the square
    int **magic_square; // pointer to heap allocated magic square
} MagicSquare;

/* TODO:
 * Prompts the user for the magic square's size, reads it,
 * checks if it's an odd number >= 3 (if not display the required
 * error message and exit), and returns the valid number.
 */
int getSize() 
{
	int n;
    printf("Enter magic square's size (odd integer >=3)\n");
	scanf("%i",&n); //taking user input for size of magic square
	if(n<3)
	{
		printf("Magic square size must be >= 3.");
		exit(1);
	}
	else if(n%2==0)
	{
		printf("Magic square size must be odd.");
		exit(1);
	}
    else
	{
		return n;
	}  
} 
   
/* TODO:
 * Makes a magic square of size n using the 
 * Siamese magic square algorithm or alternate from assignment 
 * or another valid alorithm that produces a magic square.
 *
 * n is the number of rows and columns
 *
 * returns a pointer to the completed MagicSquare struct.
 */
MagicSquare *generateMagicSquare(int n) 
{
	MagicSquare *magic = malloc(sizeof(MagicSquare)); // Allocating Heap memeory for struct pointer 
	if(magic == NULL)
	{
		printf("Error Allocation memeory");
		exit(1);
	}
	magic->size = n; //Assigning size data member to n
	magic->magic_square = malloc(sizeof(int*) * n); //Allocating memory for n rows of the Magic Square
	if(magic->magic_square == NULL)
	{
		printf("Error Allocation memeory");
		exit(1);
	}
	
	for(int i=0;i<n;i++) 
	{
		*((magic->magic_square)+i) = malloc(sizeof(int) * n); //Allocating memory for n columns of the Magic Square
	}

    for(int row = 0; row < n; row++)
    {
        for(int col = 0; col < n; col++)
        {
            *(*((magic->magic_square) + row) + col) = 0; //Assigning each value in the Magic square to 0
        }
    }

    int cur_row = 0; //Top row for number 1
    int cur_col = n/2; //centre column  for number 1

    for(int i=1;i<=n*n;i++)
    {
        *(*((magic->magic_square) + cur_row) + cur_col) = i;
        int temp_row = cur_row - 1; //To move up the Magicsquare
        int temp_col = cur_col + 1; //To move right the Magicsquare
        if(temp_col > n-1) 
		{
			temp_col = 0; //wrapping the column 
		}
		if(temp_row < 0)
		{
			temp_row = n-1; //wrapping the row
		}
        if(*(*((magic->magic_square) + temp_row) + temp_col)==0) // checking if the position is free
        {
            cur_row = temp_row;
            cur_col = temp_col;
        }
        else
        {
            cur_row = cur_row + 1;  //if not free, moving to one row below
        }
		

    }
    return magic;    
} 

/* TODO:  
 * Opens a new file (or overwrites the existing file)
 * and writes the square in the specified format.
 *
 * magic_square the magic square to write to a file
 * filename the name of the output file
 */
void fileOutputMagicSquare(MagicSquare *magic_square, char *filename) 
{
    FILE *ofile = fopen(filename, "w"); //opening ofile in write mode
	if(ofile == NULL)
    {
        printf("Cannot open file\n");
        exit(1);
    }
    int n = magic_square->size;
    fprintf(ofile, "%d\n", n); //writing the size of the Magic Square into the file
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            fprintf(ofile, "%d", *(*(magic_square -> magic_square + i) + j)); //writing the elements of the Magic Square into the file
            if(j != n-1) // For inserting comma's accuratley
            {
                fprintf(ofile,",");
            }
        }
        fprintf(ofile,"\n");//to move to the next line
    } 

    int close = fclose(ofile); //closing the file
    if(close != 0) 
    {
        printf("Error while closing file\n");
        exit(1);
    }
}



/* TODO:
 * Generates a magic square of the user specified size and
 * outputs the square to the output filename.
 * 
 * Add description of required CLAs here
 */
int main(int argc, char **argv) {
    // TODO: Check input arguments to get output filename
    if(argc != 2)
    {
        printf("Incorrect number of CLA");
        exit(1);
    }
    // TODO: Get magic square's size from user
    int magic_size = getSize();
    // TODO: Generate the magic square by correctly interpreting 
    //       the algorithm(s) in the write-up or by writing or your own.  
    //       You must confirm that your program produces a 
    //       Magic Sqare as described in the linked Wikipedia page.
    MagicSquare *magicsq = generateMagicSquare(magic_size);

    // TODO: Output the magic square
    fileOutputMagicSquare(magicsq,argv[1]);

    for(int i=0;i<magicsq->size;i++)
    {
        free(magicsq->magic_square[i]); //freeing the columns of the Magic Square
    }

    free(magicsq->magic_square); //freeing the rows of the Magic Suquare

    free(magicsq);

    return 0;
} 
