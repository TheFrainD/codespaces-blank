#include "mem.h"

#define _DEFAULT_SOURCE
#include <unistd.h>

// space taken up by pointer to prev and next entry in linked list
#define MD_SIZE 2 * sizeof(void *)
#define MD_FULL_SIZE sizeof(struct entry)

struct entry {
    size_t size;
    void *prev;
    void *next;
};

struct entry *head = NULL;

void *malloc(size_t size)
{
    size_t *ptr;
    size_t allocated_size = size < MD_SIZE ? MD_SIZE : size; // we must allocate enough space to fit linked list pointers in
    // first we have to check list of memory blocks released by free()
    // iterate through list and use first fit block
    struct entry *cur = head;
    while (cur != NULL) {
        if (cur->size >= size) {
            ptr = (size_t *)cur;
            // if block is enough size we can split it
            if (cur->size - allocated_size >= MD_FULL_SIZE) {
                struct entry *new_entry = (struct entry *)(((char *)cur) + MD_FULL_SIZE + allocated_size);
                new_entry->size = cur->size - allocated_size;
                cur->size = allocated_size;
                if ((new_entry->next = cur->next) != NULL) {
                    ((struct entry *)cur->next)->prev = new_entry;
                }
                if ((new_entry->prev = cur->prev) != NULL) {
                    ((struct entry *)cur->prev)->next = new_entry;
                }
                if (head == cur) {
                    head = new_entry;
                }
                
                
            }
            return ptr + 1;
        }
        cur = cur->next;
    }

    // no block found
    ptr = (size_t *)sbrk(allocated_size + sizeof(size_t));
    if (ptr == (void *) -1) {
        return NULL;
    }
    *ptr = allocated_size;
    return ptr + 1;
}

void free(void *ptr)
{
    if (ptr == NULL)
        return;
    
    struct entry *entry = (struct entry *)(((size_t *)ptr) - 1);
    entry->prev = NULL;
    entry->next = NULL;
    
    if (head == NULL) {
        head = entry;
        return;
    }

    // place freed block in the tail of the list
    struct entry *cur = head;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    entry->prev = cur;
    cur->next = entry;
}
