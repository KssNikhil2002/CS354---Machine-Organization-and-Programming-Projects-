////////////////////////////////////////////////////////////////////////////////
// Main File:        p3Heap.c
// This File:        p3Heap.c
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
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020-2023 Deb Deppeler based on work by Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission SPRING 2023, CS354-deppeler
//
///////////////////////////////////////////////////////////////////////////////

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "p3Heap.h"

/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block but only containing size.
 */
typedef struct blockHeader
{

    int size_status;

    /*
     * Size of the block is always a multiple of 8.
     * Size is stored in all block headers and in free block footers.
     *
     * Status is stored only in headers using the two least significant bits.
     *   Bit0 => least significant bit, last bit
     *   Bit0 == 0 => free block
     *   Bit0 == 1 => allocated block
     *
     *   Bit1 => second last bit
     *   Bit1 == 0 => previous block is free
     *   Bit1 == 1 => previous block is allocated
     *
     * Start Heap:
     *  The blockHeader for the first block of the heap is after skip 4 bytes.
     *  This ensures alignment requirements can be met.
     *
     * End Mark:
     *  The end of the available memory is indicated using a size_status of 1.
     *
     * Examples:
     *
     * 1. Allocated block of size 24 bytes:
     *    Allocated Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 25
     *      If the previous block is allocated p-bit=1 size_status would be 27
     *
     * 2. Free block of size 24 bytes:
     *    Free Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 24
     *      If the previous block is allocated p-bit=1 size_status would be 26
     *    Free Block Footer:
     *      size_status should be 24
     */
} blockHeader;

/* Global variable - DO NOT CHANGE NAME or TYPE.
 * It must point to the first block in the heap and is set by init_heap()
 * i.e., the block at the lowest address.
 */
blockHeader *heap_start = NULL;

/* Size of heap allocation padded to round to nearest page size.
 */
int alloc_size;

/*
 * Additional global variables may be added as needed below
 * TODO: add global variables needed by your function
 */
 
/*
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block (payload) on success.
 * Returns NULL on failure.
 *
 * This function must:
 * - Check size - Return NULL if size < 1
 * - Determine block size rounding up to a multiple of 8
 *   and possibly adding padding as a result.
 *
 * - Use BEST-FIT PLACEMENT POLICY to chose a free block
 *
 * - If the BEST-FIT block that is found is exact size match
 *   - 1. Update all heap blocks as needed for any affected blocks
 *   - 2. Return the address of the allocated block payload
 *
 * - If the BEST-FIT block that is found is large enough to split
 *   - 1. SPLIT the free block into two valid heap blocks:
 *         1. an allocated block
 *         2. a free block
 *         NOTE: both blocks must meet heap block requirements
 *       - Update all heap block header(s) and footer(s)
 *              as needed for any affected blocks.
 *   - 2. Return the address of the allocated block payload
 *
 *   Return if NULL unable to find and allocate block for required size
 *
 * Note: payload address that is returned is NOT the address of the
 *       block header.  It is the address of the start of the
 *       available memory for the requesterr.
 *
 * Tips: Be careful with pointer arithmetic and scale factors.
 */
void *balloc(int size)
{
    // TODO: Your code goes in here.
    
    if(size<1)//checks if size requested is valid
    {
    	return NULL;
	}
	
    // Total size required for the payload and the blockheader 
	int total_blocksize = size + sizeof(blockHeader); 
	
    // caculating the padding required to be added
	int padding = 8 - (total_blocksize % 8);  
	
	//rounding up blocksize to a multiple of 8
	if(total_blocksize % 8 != 0)
	{
		total_blocksize = total_blocksize + padding;
	}
	
    // points to the start of the heap memeory 
	blockHeader *current_block = heap_start;
	
	//initially all are assigned to null since no memory is allocated 
    blockHeader *bestfit_block = NULL;

    blockHeader *next_block = NULL;

    blockHeader *temp_block = NULL;
    
    blockHeader *free_block = NULL;

    blockHeader *prev_block = NULL;

    //used to check if the current block is used(a bit- 1=used and 0=free)
    int curr_bit = 0; 
	
    //used to check if the prev block is used(p bit- 1=used and 0=free)
	int prev_bit = 0; 
	
	int currblock_size;
	
	while(current_block->size_status != 1)
	{
        // stores the size of the current block
		currblock_size = current_block->size_status;
		
        //checks if the current block is allocated
		if((currblock_size & 1) == 1) 
		{
			curr_bit = 1; //assignes 1 since allocated 
			
			currblock_size = currblock_size - 1; 
		}
		else
		{
			curr_bit = 0; //assigning 0 since it is free	
		}
		
        //similarly checks if the previous block is allocated 
		if((currblock_size & 2) == 2) 
		{
        	prev_bit = 1; //assignes 1 since prev block is allocated

            currblock_size = currblock_size - 2;
        }
        else 
		{	  
            prev_bit = 0;  //assigning 0 since it is free
        }
        
        /*checks if the current block is allocated or if its size is less 
        *than required size 
        */
        if(curr_bit == 1 || currblock_size < total_blocksize)  
		{
            prev_block = current_block;
            
            // shifts to the next block if condition satisfied
            current_block = (blockHeader*) ((char*)current_block + currblock_size); 

            continue;
        }
        
        /*checks if size of the current block greater than 
        * size needed to allocate 
        */
        if(currblock_size > total_blocksize) 
		{
            //checks if bestfit is null or bestfit_block size is 
            //greater than current block size
            if(bestfit_block == NULL || currblock_size < bestfit_block -> size_status) 
			{
				bestfit_block = current_block;  
            }
        }
        
        /*checks if size of the current block and the needed 
        * size are the same(best case to allocate memory)
        */
        if(currblock_size == total_blocksize)
        {
        	//we assign bestfit_block to the current block
        	bestfit_block = (blockHeader*)(current_block);

        	//assigning the size of the bestfit block
        	bestfit_block->size_status = total_blocksize;
        	
            //setting a - bit since allocated
        	bestfit_block->size_status = bestfit_block->size_status + 1;
        	
        	//checking if prev block is allocated 
        	if(prev_bit == 1)
        	{
        		//setting p - bit since prev block is allocated
        		bestfit_block->size_status = bestfit_block->size_status + 2;
			}
			
            // creating the next block 
			next_block = (blockHeader*)((char*)current_block + currblock_size); 

            //checking if the next block is the end of the memory
            if(next_block -> size_status != 1){
                
                //if it is not then it sets the p bit to 1.  
                next_block -> size_status += 2;
            }

            return (void*)((char*)bestfit_block + sizeof(blockHeader));;
       	
		}
		
		//case for splitting
		if(bestfit_block != NULL)
		{
			bestfit_block->size_status = total_blocksize;
			
			temp_block = (blockHeader*)bestfit_block;//creating temp block
			
			temp_block->size_status = total_blocksize;
			
            //setting the a bit to 1
			temp_block->size_status = temp_block->size_status + 1;
			
            //checking if the prev block is allocated
			if(prev_block == NULL || (prev_block->size_status & 1) == 1)
			{
                //setting p bit
				temp_block->size_status = temp_block->size_status + 2; 
			}

			if((currblock_size - total_blocksize) % 8 == 0)
            {
                //intializing the free block
                free_block = (blockHeader*)((char*)bestfit_block + total_blocksize);

                //calculating the size of free block(current size - required size)
                free_block -> size_status = currblock_size - total_blocksize;

                //sets the p bit of the free block	
                free_block -> size_status = free_block -> size_status + 2;

                //initializing the footer
                blockHeader *footer = (blockHeader*)((char*)free_block + (currblock_size - total_blocksize) - sizeof(blockHeader));

                //assigning the size of the footer(current block size-required size)
                footer -> size_status = currblock_size - total_blocksize;
            }
            
            return (void*)((char*)temp_block + sizeof(blockHeader));
		}
		else
		{
			break;//if the block of memory was not allocated 
		}
		
        //shifts to the next block of memory
		current_block = (blockHeader*)((char*)current_block + currblock_size);
		
	}
	
	return NULL;

}

/*
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - Update header(s) and footer as needed.
 */
int bfree(void *ptr)
{
    // TODO: Your code goes in here.
    
    if(ptr == NULL)//checks if the ptr is NULL
	{
        return -1;
    }
    
    //creating a pointer pointing to the memeory location to be freed
    blockHeader *current_block = (blockHeader*) ((char*)ptr - sizeof(blockHeader));
	
    // stores the size of the block to be freed
	int currblock_size = current_block -> size_status; 
	
    //used to check if the prev block is used(p bit - 1=used and 0=free)
	int prev_bit = 0; 
	
    //checks if the block is aldready freed 
	if((currblock_size & 1) != 1)
    {
        return -1;
    }
    
    if((currblock_size & 1) == 1)
    {
        currblock_size = currblock_size - 1;
    }

    //checks if the previous block is allocated 
    if((currblock_size & 2) == 2) 
    {
        prev_bit = 1; //assignes 1 since prev block is allocated

        currblock_size = currblock_size - 2;
    }
    else 
    {
    	prev_bit = 0;  //assigning 0 since it is free
	}

    //pointer for the next block of memeory
    blockHeader *next_block = (blockHeader*) ((char*)current_block + currblock_size);
    
    //checks if the size of the block to be freed is a multiple of 8
    if((currblock_size % 8) != 0)
    {
        return -1;
    }
    
    //checks if the ptr is outside the heap memory space 
    if((unsigned int)ptr - sizeof(blockHeader) < (unsigned int)heap_start || (unsigned int)ptr - sizeof(blockHeader) > (unsigned int)heap_start + alloc_size - 8) 
    {
        return -1;
    }

    blockHeader *footer = (blockHeader*) ((char*)current_block + currblock_size - sizeof(blockHeader));

    footer -> size_status = currblock_size;
    
    //checks if the previous block is allocated 
    if(prev_bit == 1)
    {
    	current_block -> size_status = currblock_size + 2;
	}
	else
	{   
		current_block -> size_status = currblock_size; 
	}
	
	//checks if the next block is the end of the heap memeory
	if(next_block -> size_status != 1)
	{
        //sets the p bit to 0;
        next_block -> size_status =  next_block -> size_status - 2;
    }
	
    return 0;
}

/*
 * Function for traversing heap block list and coalescing all adjacent
 * free blocks.
 *
 * This function is used for user-called coalescing.
 * Updated header size_status and footer size_status as needed.
 */
int coalesce()
{
    // TODO: Your code goes in here.
    
    //points to the start of the heap memory
    blockHeader *current_block = heap_start; 
    
    //to access the next memory block
    blockHeader *next_block = NULL; 
    
    //stores the size of next block
    int nextblock_size;
    
    //traversing the memory
    while(current_block -> size_status != 1)
    {
        // stores the size of the current block
    	int currblock_size = current_block->size_status; 
    	
        //used to check if the current block is used(a bit - 1=used and 0=free)
    	int curr_bit = 0; 
	
        //used to check if the prev block is used(p bit - 1=used and 0=free) 
		int prev_bit = 0; 
		
        //checks if the current block is allocated
		if((currblock_size & 1) == 1)  
		{
			curr_bit = 1; //assignes 1 since allocated 
			
			currblock_size = currblock_size - 1; 
		}
		
        //similarly checks if the previous block is allocated 
		if((currblock_size & 2) == 2) 
		{
        	prev_bit = 1; //assignes 1 since prev block is allocated

            currblock_size = currblock_size - 2;
        }
        
        //checks if the current block is used 
        if(curr_bit == 1)
        {
            //shifts to the next block of memeory
        	current_block = (blockHeader*)((char*)current_block + currblock_size);

            continue;	
		}
		
        //initializing next block
		next_block = (blockHeader*)((char*)current_block + currblock_size); 
		
        //checks if the next block is the end of memeory
		if(next_block -> size_status == 1)
        {
            break; //then breaks as no memory to coalesc
        }
        //checks if the next block is allocated or not 
        else if((next_block -> size_status & 1) == 1)
		{
            //if allocated, then shifts the current pointer to the next block
            current_block = next_block;

            continue;
        }
        else //if the next block is also free, then we coalesc 
        {
        	nextblock_size = next_block -> size_status; 
        	
            //size of coalesced block
        	current_block -> size_status = currblock_size + nextblock_size; 
        	
        	//creating a footer for the coalesced block
        	blockHeader *footer = (blockHeader*)((char*)current_block + (current_block -> size_status) - sizeof(blockHeader));
        	
            //assigning the size of the block to footer
        	footer -> size_status = current_block -> size_status; 
        	
        	//checking if the prev block is allocated
        	if(prev_bit == 1)
			{
				current_block -> size_status += 2;//setting the p bit 
			}
		}
		
	}
	return 0;
}

/*
 * Function used to initialize the memory allocator.
 * Intended to be called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */
int init_heap(int sizeOfRegion)
{

    static int allocated_once = 0; // prevent multiple myInit calls

    int pagesize;   // page size
    int padsize;    // size of padding when heap size not a multiple of page size
    void *mmap_ptr; // pointer to memory mapped area
    int fd;

    blockHeader *end_mark;

    if (0 != allocated_once)
    {
        fprintf(stderr,
                "Error:mem.c: InitHeap has allocated space during a previous call\n");
        return -1;
    }

    if (sizeOfRegion <= 0)
    {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize from O.S.
    pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    alloc_size = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd)
    {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    mmap_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == mmap_ptr)
    {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }

    allocated_once = 1;

    // for double word alignment and end mark
    alloc_size -= 8;

    // Initially there is only one big free block in the heap.
    // Skip first 4 bytes for double word alignment requirement.
    heap_start = (blockHeader *)mmap_ptr + 1;

    // Set the end mark
    end_mark = (blockHeader *)((void *)heap_start + alloc_size);
    end_mark->size_status = 1;

    // Set size in header
    heap_start->size_status = alloc_size;

    // Set p-bit as allocated in header
    // note a-bit left at 0 for free
    heap_start->size_status += 2;

    // Set the footer
    blockHeader *footer = (blockHeader *)((void *)heap_start + alloc_size - 4);
    footer->size_status = alloc_size;

    return 0;
}

/*
 * Function can be used for DEBUGGING to help you visualize your heap structure.
 * Traverses heap blocks and prints info about each block found.
 *
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts)
 * t_End    : address of the last byte in the block
 * t_Size   : size of the block as stored in the block header
 */
void disp_heap()
{

    int counter;
    char status[6];
    char p_status[6];
    char *t_begin = NULL;
    char *t_end = NULL;
    int t_size;

    blockHeader *current = heap_start;
    counter = 1;

    int used_size = 0;
    int free_size = 0;
    int is_used = -1;

    fprintf(stdout,
            "*********************************** HEAP: Block List ****************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout,
            "---------------------------------------------------------------------------------\n");

    while (current->size_status != 1)
    {
        t_begin = (char *)current;
        t_size = current->size_status;

        if (t_size & 1)
        {
            // LSB = 1 => used block
            strcpy(status, "alloc");
            is_used = 1;
            t_size = t_size - 1;
        }
        else
        {
            strcpy(status, "FREE ");
            is_used = 0;
        }

        if (t_size & 2)
        {
            strcpy(p_status, "alloc");
            t_size = t_size - 2;
        }
        else
        {
            strcpy(p_status, "FREE ");
        }

        if (is_used)
            used_size += t_size;
        else
            free_size += t_size;

        t_end = t_begin + t_size - 1;

        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%4i\n", counter, status,
                p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);

        current = (blockHeader *)((char *)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout,
            "---------------------------------------------------------------------------------\n");
    fprintf(stdout,
            "*********************************************************************************\n");
    fprintf(stdout, "Total used size = %4d\n", used_size);
    fprintf(stdout, "Total free size = %4d\n", free_size);
    fprintf(stdout, "Total size      = %4d\n", used_size + free_size);
    fprintf(stdout,
            "*********************************************************************************\n");
    fflush(stdout);

    return;
}

// end p3Heap.c (Spring 2023)
