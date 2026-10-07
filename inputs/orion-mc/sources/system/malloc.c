// MALLOC.C

// malloc - special malloc routines 

// header of standard C - libraries
# include <stddef.h>
# include <string.h>

#include "cal_conf.h"
#include "malloc.h"

// external variables
// --------------------------------------------------------------------------

// local defined variables
//---------------------------------------------------------------------------
#pragma class HB=MEMPOOL
static unsigned char huge *memfree_ptr; 
static unsigned char huge *memEnd;
// memory pool 
// CalMalloc() allocate memory from this buffer
static unsigned char huge mempool[CONFIG_SIZE_POOL];
#pragma default_attributes

// local defined functions
//---------------------------------------------------------------------------
//static UNSIGNED16 getFreeMem(void);

//*******************************************************************
//
// \brief InitCalMalloc - initialize memory allocation system
//
// \attention
// size is not used, use CONFIG_SIZE_POOL
//
// \returns
// passed parameter
void InitCalMalloc(void) //< unused
{
unsigned int *ptr = (unsigned int *)mempool;
unsigned int tmpsize;

  tmpsize = sizeof(mempool);
  memfree_ptr = mempool;
  while(((unsigned long)memfree_ptr % CONFIG_ALIGNMENT) != 0)
    memfree_ptr++;
  memEnd = (unsigned char *)mempool + tmpsize;
  tmpsize = sizeof(mempool)/2 + 1;
  do
    *ptr++ = 0;
  while (--tmpsize);
}

//*******************************************************************
//
// \brief getFreeMem - get the free memory space
//
// \returns
// size of free memory space
/*
static UNSIGNED16 getFreeMem(void)
{
  return((UNSIGNED16)memEnd - (UNSIGNED16)memfree_ptr);
}
*/

//*******************************************************************
//
// \brief CalMalloc - target specific malloc
//
// \returns
// pointer to allocated memory
void *CalMalloc(size_t RequestedSize) //< requested size of heap
{
size_t tmpSize = RequestedSize;

  while((tmpSize % CONFIG_ALIGNMENT) != 0)
  	tmpSize++;

  if((memfree_ptr + tmpSize) > memEnd)
	return(NULL); // return with error
  memfree_ptr += tmpSize;
  return( (void *)(memfree_ptr - tmpSize));
}

//*******************************************************************
//
// \brief CalFree - target specific free
//
// This simply implementation don't free the memory.
void CalFree(void * pBlock) //< pointer to allocated heap
{ 
  pBlock = pBlock; // avoid "unused parameter" warning      
}
