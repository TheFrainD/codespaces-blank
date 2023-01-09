#ifndef TLPI_MEM_H
#define TLPI_MEM_H

#include <stddef.h>

#define NULL ((void *)0)

/*
 * Custom implementation of stdlib.h's malloc()
 * 
 * Allocates 'size' bytes and returns a pointer to the allocated memory.
 * 
 * Notes:
 * The implementation is simillar to stdlib's. Allocates block of memory
 * with metadata.
 * 
 * Metadata after malloc:
 *  8/4 bytes         at least 16/8 bytes to fit prev and next pointers (for free())
 * [      size      |                     data                                      ]
 *                  | <- pointer that is returned to a user
 * Algorithm:
 * 1) Look through list of freed memory and look for block that
 * can fit required size. It uses the first block that is large enough.
 * 
 * 2) If we found free block that fits. We check if we can split it:
 * it has to be large enough to fit 24 bytes(for 64-bit systems) of meta data. 
 * If it's large enough it splits, first part is returned to a user, second 
 * replaces original block
 * 
 * 3) If the block wasn't found or the list is empty new memory block is allocated.
 */
void *malloc(size_t size);


/* 
 * Custom implementation of stdlib.h's free()
 * 
 * Frees the memory space pointed to by ptr, which must have been returned by a previous call to malloc().
 * Otherwise, undefined behavior occurs.
 * 
 * Notes:
 * The implementation is simillar to stdlib's. Uses doubly linked list to keep track of freed 
 * memory blocks. but lacks some functionality, like heap ungrowing. Doesn't avoid fragmentation.
 * 
 * Node structure:
 *       size_t            void *          void *
 * [      size      |      prev      |      next      |   remaining bytes of data  ]
 *                  {                          data                                }
 */
void free(void *ptr);

#endif // TLPI_MEM_H
